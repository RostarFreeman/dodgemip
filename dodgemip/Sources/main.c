/**
 * @file: 		main.c
 * @revision:	1.0
 * @date:	 	27-May-2021
 * @brief: 		Implements the game.
 */

#include "derivative.h" /* include peripheral declarations */
#include "pinout.h"
#include "game.h"

#include "input_button.h"
#include "lcd.h"


int wait = 1;
int seed = 23459237;

/**
 * @function: 	random
 * @param: 		none
 * @return: 	value: Random unsigned integer value 
 * @brief: 		Pseudo-random number generator
 */
unsigned int random() {
	unsigned int value = (( (seed * 424323) + (unsigned int)(TPM0_C0V)) % 89898985);
	seed = value;
	return value;
}

/**
 * @function: 	manageInputs
 * @param: 		none
 * @return: 	button: Last pressed button in Port A.
 * @brief: 		Gathers the last button input. Updates global input button state
 * 				as acknowledged (no further reads will be performed on that
 * 				button unless it is pressed again).
 */
uint8_t manageInputs() {
	if (ibStatus.ack == BTN_PENDING) {
		ibStatus.ack = BTN_ACKNOWLEDGED;
		return ibStatus.status_word;
	}
	return BTN_NOBUTTON;
}

/**
 * @function: 	cleanBuffer
 * @param: 		*status: Pointer to the global game state variable.
 * @return: 	none
 * @brief: 		Gathers the last button input. Updates global input button state
 * 				as acknowledged (no further reads will be performed on that
 * 				button unless it is pressed again).
 */
void cleanBuffer(gamestate_t* status) {
	int i;
	for (i = 0; i < GAME_LCDSIZE; i++) {
		status->buffer[i] = GAME_BLANK;
	}
}

/**
 * @function: 	displaceBuffer
 * @param: 		*status: Pointer to the global game state variable.
 * @return: 	none
 * @brief: 		Moves all elements in the game buffer one cell to the left.
 */
void displaceBuffer(gamestate_t* status) {
	int i = 0;
	for (; i < (GAME_LCDSIZE / 2) - 1; i++) {
		status->buffer[i] = status->buffer[i + 1];
		status->buffer[i + (GAME_LCDSIZE / 2)] = status->buffer[i + (GAME_LCDSIZE / 2) + 1];
	}
	status->buffer[15] = GAME_BLANK;
	status->buffer[31] = GAME_BLANK;
}

/**
 * @function: 	generateObstacles
 * @param: 		*status: Pointer to the global game state variable.
 * @return: 	none
 * @brief: 		Generates new obstacles at the rightmost cells of the screen. 
 * 				Checks for the existence of a valid path.
 */
void generateObstacles(gamestate_t* status) {
	int pos;
	for (pos = 0; pos < 2; pos++) {
		unsigned int randvalue = random();
		int position = (pos * 16) + 15;
		int other = (position + 16) % 32;
		
		switch (randvalue % 4) {
		case 1: 	// Block
			if (status->buffer[other]				!= GAME_BLOCK  
					&& status->buffer[other - 1]	!= GAME_BLOCK
					&& status->buffer[other - 2]	!= GAME_BLOCK
					&& status->buffer[position - 1]	!= GAME_FW)
				status->buffer[position] = GAME_BLOCK;
			else
				status->buffer[position] = GAME_BLANK;
			break;
		case 2:		// Forward
			if (status->buffer[position - 1]		!= GAME_FW
					&& status->buffer[position - 2] != GAME_FW
					&& status->buffer[other - 1] 	!= GAME_FW
					&& status->buffer[other - 2] 	!= GAME_FW
					&& status->buffer[position - 1] != GAME_BW
					&& status->buffer[position - 2] != GAME_BW
					&& status->buffer[other - 1] 	!= GAME_BW
					&& status->buffer[other - 2] 	!= GAME_BW
					)
				status->buffer[position] = GAME_FW;
			else
				status->buffer[position] = GAME_BLANK;
			break;
		case 3:		// Backward
			if (status->buffer[position - 1] 		!= GAME_FW
					&& status->buffer[position - 2] != GAME_FW
					&& status->buffer[other - 1] 	!= GAME_FW
					&& status->buffer[other - 2] 	!= GAME_FW
					&& status->buffer[position - 1] != GAME_BW
					&& status->buffer[position - 2] != GAME_BW
					&& status->buffer[other - 1] 	!= GAME_BW
					&& status->buffer[other - 2] 	!= GAME_BW
					)
				status->buffer[position] = GAME_BW;
			else
				status->buffer[position] = GAME_BLANK;
			break;
		default: 	// Space
			status->buffer[position] = GAME_BLANK;
			break;
		}
	}
}

/**
 * @function: 	updateGame
 * @param: 		*status: Pointer to the global game state variable.
 * 				input: Identifier of the last captured key press.
 * @return: 	none
 * @brief: 		Updates the state of the game according to the content of
 * 				the status container.
 */
void updateGame(gamestate_t* status, uint8_t input) {
	switch (status->state) {
	case GAME_READYSCREEN:

		if (input == BTN_UP_PRESSED) {
			cleanBuffer(status);
			
			status->state = GAME_BLINKING;
			status->phaseTimer = 90;
			status->entityPos = 7;
		}
		break;
	case GAME_BLINKING:
		status->phaseTimer -= 1;
		if ((status->phaseTimer % 15) == 0) {
			if (status->entityPos == GAME_NOENTITY) {
				status->entityPos = 7;
			}
			else {
				status->entityPos = GAME_NOENTITY;
			}
		}
		if (status->phaseTimer == 0) {	// Transition
			status->state = GAME_INGAME;
			status->entityPos = 7;
			status->ticks = 0;
			status->difficulty = 30;
			status->phaseTimer = 0;
			status->updateTimer = 0;
		}
		
		break;
	case GAME_INGAME:
		status->phaseTimer += 1;
		status->updateTimer += 1;
		if (input == BTN_UP_PRESSED && status->entityPos > 15) {
			status->entityPos -= 16;
		}
		if (input == BTN_DW_PRESSED && status->entityPos < 16) {
			status->entityPos += 16;
		}
		if (status->updateTimer >= status->difficulty) {
			displaceBuffer(status);
			generateObstacles(status);
			status->updateTimer = 0;
		}
		if (status->buffer[status->entityPos] == GAME_FW) {
			status->buffer[status->entityPos] = GAME_BLANK;
			if (status->entityPos != 15 && status->entityPos != 31) {
				status->entityPos += 1;
			}
			else {
				gameOver(status);
			}
		}
		if (status->buffer[status->entityPos] == GAME_BW) {
			status->buffer[status->entityPos] = GAME_BLANK;
			if (status->entityPos != 0 && status->entityPos != 16) {
				status->entityPos -= 1;
			}
			else { // Transition
				gameOver(status);
			}
		}
		if (status->buffer[status->entityPos] == GAME_BLOCK) { // Transition
			gameOver(status);
		}
		if((status->phaseTimer == (GAME_DIFFTIME * GAME_UPDATE_RATE)) && (status->difficulty > GAME_MAXDIFF)) {
			status->difficulty -= 1;
			status->phaseTimer = 0;
		}
		break;
	case GAME_GAMEOVER:
		status->phaseTimer -= 1;
		
		if (status->phaseTimer <= 0) {
			status->state = GAME_READYSCREEN;
			status->ticks = 0;
			status->entityPos = GAME_NOENTITY;
			status->phaseTimer = 0;
			status->difficulty = 0;
			status->updateTimer = 0;
			bufferUpToPlay(status);
		}
		break;
	default:
		break;
	}
}

/**
 * @function: 	waitFrame
 * @param: 		none
 * @return: 	none
 * @brief: 		Sinks the remaining cycles before the next tick update. Waits
 * 				for the frame timer TPM0 to generate an interrupt and release
 * 				the wait global variable.
 */
void waitFrame(gamestate_t* status) {
	while (wait) {
	}
	wait = 1;
	status->ticks += 1;
}

/**
 * @function: 	drawGame
 * @param: 		*status: Pointer to the global game state variable.
 * @return: 	none
 * @brief: 		Draws the contents of the buffer in status and the game entity.
 */
void drawGame(gamestate_t* status) {
	int i = 0;
	lcdHome();
	for (; i < GAME_LCDSIZE; i++) {
		lcdWrite(status->buffer[i]);
	}
	
	if (status->entityPos != GAME_NOENTITY) {
		lcdPlaceCursor(status->entityPos);
		lcdWrite(GAME_CHARACTER);
	}
}

/**
 * @function: 	gameOver
 * @param: 		*status: Pointer to the global game state variable.
 * @return: 	none
 * @brief: 		Performs the transition to the GAME_GAMEOVER state.
 */
void gameOver(gamestate_t* status) {
	status->state = GAME_GAMEOVER;
	status->prevTime = status->ticks / GAME_UPDATE_RATE;
	status->phaseTimer = 90;
	
	cleanBuffer(status);
	
	status->buffer[3]	= LCD_CHAR_G;
	status->buffer[4]	= LCD_CHAR_A;
	status->buffer[5]	= LCD_CHAR_M;
	status->buffer[6]	= LCD_CHAR_E;
	status->buffer[7]	= LCD_CHAR_SPACE;
	status->buffer[8]	= LCD_CHAR_SPACE;
	status->buffer[9]	= LCD_CHAR_O;
	status->buffer[10]	= LCD_CHAR_V;
	status->buffer[11]	= LCD_CHAR_E;
	status->buffer[12]	= LCD_CHAR_R;
	
	status->entityPos = GAME_NOENTITY;
}

/**
 * @function: 	initialiseStatus
 * @param: 		*status: Pointer to the global game state variable.
 * @return: 	none
 * @brief: 		Initialises a clean GAME_READYSCREEN state for the game to boot
 * 				into.
 */
void initialiseStatus(gamestate_t* status) {
	status->ticks = 0;
	status->state = GAME_READYSCREEN;
	status->entityPos = GAME_NOENTITY;
	status->prevTime = 0;
	status->phaseTimer = 0;
	status->difficulty = 0;
	status->updateTimer = 0;
	
	bufferUpToPlay(status);
	
	int i = 16;
	for (; i < GAME_LCDSIZE; i++) {
		status->buffer[i] = LCD_CHAR_SPACE;
	}
}

/**
 * @function: 	bufferUpToPlay
 * @param: 		*status: Pointer to the global game state variable.
 * @return: 	none
 * @brief: 		Shows the "Press up to play" message on screen
 */
void bufferUpToPlay(gamestate_t* status) {
	cleanBuffer(status);
	
	status->buffer[0]  = LCD_CHAR_P;
	status->buffer[1]  = LCD_CHAR_R;
	status->buffer[2]  = LCD_CHAR_E;
	status->buffer[3]  = LCD_CHAR_S;
	status->buffer[4]  = LCD_CHAR_S;
	status->buffer[5]  = LCD_CHAR_SPACE;
	status->buffer[6]  = LCD_CHAR_U;
	status->buffer[7]  = LCD_CHAR_P;
	status->buffer[8]  = LCD_CHAR_SPACE;
	status->buffer[9]  = LCD_CHAR_T;
	status->buffer[10] = LCD_CHAR_O;
	status->buffer[11] = LCD_CHAR_SPACE;
	status->buffer[12] = LCD_CHAR_P;
	status->buffer[13] = LCD_CHAR_L;
	status->buffer[14] = LCD_CHAR_A;
	status->buffer[15] = LCD_CHAR_Y;
	
	if (status->prevTime != 0) {
		status->buffer[16]  = LCD_CHAR_P;
		status->buffer[17]  = LCD_CHAR_R;
		status->buffer[18]  = LCD_CHAR_E;
		status->buffer[19]  = LCD_CHAR_V;
		status->buffer[20]  = LCD_CHAR_COLON;
		
		status->buffer[21]  = LCD_CHAR_0 + (uint8_t)((status->prevTime / 10000) % 10);
		status->buffer[22]  = LCD_CHAR_0 + (uint8_t)((status->prevTime / 1000) % 10);
		status->buffer[23]  = LCD_CHAR_0 + (uint8_t)((status->prevTime / 100) % 10);
		status->buffer[24]  = LCD_CHAR_0 + (uint8_t)((status->prevTime / 10) % 10);
		status->buffer[25]  = LCD_CHAR_0 + (uint8_t)((status->prevTime / 1) % 10);
		
		status->buffer[26]  = LCD_CHAR_SPACE;
		status->buffer[27]  = LCD_CHAR_S;
		status->buffer[28]  = LCD_CHAR_E;
		status->buffer[29]  = LCD_CHAR_C;
		
		status->prevTime = 0;
	}
}

/**
 * @function: 	delay
 * @param: 		i: Number of cycles to idle.
 * @return: 	none
 * @brief: 		Idles the system for the set amount of cycles.
 */
void delay(int i) {
	for (; i > 0; i--) {}
}

/**
 * @function: 	setupTimer
 * @param: 		none
 * @return: 	none
 * @brief: 		Configures the TPM0 timer to control frame generation.
 */
void setupTimer() {
	MCG_C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK; 	// INTERNAL CLOCK|MCGIRCLK ACTIVE(SET)
	MCG_C2 = MCG_C2_IRCS_MASK;                        	// SELECT FAST INTERNAL REFERENCE CLOCK (1)
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(3);                 	// MCGIRCLK IS SELECTED FOR TPM CLOCK
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK; 				  	// Enable clock to TPM0 
	
	TPM0_SC  = 0;             						  	// Disable timer while configuring
	
	// Set up for 30 fps approx
	TPM0_SC |= TPM_SC_PS(0b010);  						// Prescaler
	TPM0_MOD = TPM_MOD_MOD(16250); 						// Modulo value 
	TPM0_SC |= GAME_TOF_BIT;             				// Clear TOF 
	TPM0_SC |= GAME_INTERRUPT_BIT;       				// Enable timeout interrupt
    TPM0_SC |= TPM_SC_CMOD(0b01);   					// Enable timer free-running mode
    
	NVIC_ICPR |= 0x00020000;
	NVIC_ISER |= 0x00020000;
}

/**
 * @function: 	FTM0_IRQHandler
 * @param: 		none
 * @return: 	none
 * @brief: 		Manages interrupts generated by the TPM0 timer. Unlocks the 
 * 				WaitFrame function.
 */
void FTM0_IRQHandler(void) 
{
	wait = 0;
	TPM0_SC |= GAME_TOF_BIT;   	// Clear TOF 
}

/**
 * @function: 	gameLoop
 * @param: 		none
 * @return: 	none
 * @brief: 		Implements the main game loop: Input capture, status update,
 * 				drawing the status and idling the required amount of frames per 
 * 				tick.
 */
void gameLoop() {
	gamestate_t status;
	initialiseStatus(&status);
	
	for (;;) {
		uint8_t input = manageInputs();
		updateGame(&status, input);
		drawGame(&status);
		
		waitFrame(&status);
	}
}

/**
 * @function: 	main
 * @param: 		none
 * @return: 	none
 * @brief: 		Entry point for the game. Sets up the lcd screen, both input 
 * 				buttons, the interrupt timer and launches the game loop.
 */
int main(void)
{
	// Pin setup for the input button and the LCD
	lcdSetup();
	ibSetup();
	delay(1000000);
	
	// Set up LCD Screen
	lcdFunctionSet(LCD_8BIT | LCD_LORES | LCD_2LINE);
	delay(1000000);
	lcdDisplay(LCD_CURSOROFF | LCD_DISPLAYON | LCD_BLINKOFF);
	delay(100000);
	lcdClear();
	delay(100000);
	lcdHome();
	delay(100000);
	
	// Set up timer
	setupTimer();
	
	// Endless game loop
	gameLoop();
	
	return 0;
}
