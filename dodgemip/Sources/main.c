/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "pinout.h"
#include "game.h"

#include "input_button.h"
#include "lcd.h"


wait = 1;
seed = 23459237;

unsigned int random() {
	unsigned int value = (( (seed * 424323) + (unsigned int)(TPM0_C0V)) % 89898985);
	seed = value;
	return value;
}

uint8_t manageInputs() {
	if (ibStatus.ack == BTN_PENDING) {
		ibStatus.ack = BTN_ACKNOWLEDGED;
		return ibStatus.status_word;
	}
	return BTN_NOBUTTON;
}

void cleanBuffer(gamestate_t* status) {
	int i;
	for (i = 0; i < GAME_LCDSIZE; i++) {
		status->buffer[i] = GAME_BLANK;
	}
}

void displaceBuffer(gamestate_t* status) {
	int i = 0;
	for (; i < (GAME_LCDSIZE / 2) - 1; i++) {
		status->buffer[i] = status->buffer[i + 1];
		status->buffer[i + (GAME_LCDSIZE / 2)] = status->buffer[i + (GAME_LCDSIZE / 2) + 1];
	}
}

void generateObstacles(gamestate_t* status) {
	int pos;
	for (pos = 0; pos < 2; pos++) {
		unsigned int randvalue = random();
		int position = (pos * 16) + 15;
		int other = (position + 16) % 32;
		
		switch (randvalue % 4) {
		case 1: 	// Block
			if (status->buffer[other] 				!= GAME_BLOCK  
					&& status->buffer[other - 1] 	!= GAME_BLOCK
					&& status->buffer[other - 2] 	!= GAME_BLOCK
					&& status->buffer[position - 1] != GAME_FW)
				status->buffer[position] = GAME_BLOCK;
			else
				status->buffer[position] = GAME_BLANK;
			break;
		case 2:		// Forward
			if (status->buffer[position - 1] 		!= GAME_FW
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

void updateGame(gamestate_t* status, uint8_t input) {
	switch (status->state) {
	case GAME_READYSCREEN:
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
		if (status->phaseTimer == 0) {
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
				// game over
			}
		}
		if (status->buffer[status->entityPos] == GAME_BW) {
			status->buffer[status->entityPos] = GAME_BLANK;
			if (status->entityPos != 0 && status->entityPos != 16) {
				status->entityPos -= 1;
			}
			else {
				// game over
			}
		}
		if (status->buffer[status->entityPos] == GAME_BLOCK) {
			// game over
		}
		if((status->phaseTimer == (GAME_DIFFTIME * GAME_UPDATE_RATE)) && (status->difficulty > GAME_MAXDIFF)) {
			status->difficulty -= 1;
			status->phaseTimer = 0;
		}
		break;
	case GAME_GAMEOVER:
		
		break;
	default:
		break;
	}
}

void waitFrame(gamestate_t* status) {
	while (wait) {
	}
	wait = 1;
	status->ticks += 1;
}

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


void initialiseStatus(gamestate_t* status) {
	status->ticks = 0;
	status->state = GAME_READYSCREEN;
	status->entityPos = GAME_NOENTITY;
	status->prevTime = 0;
	status->phaseTimer = 0;
	status->difficulty = 0;
	status->updateTimer = 0;
	
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
	
	int i = 16;
	for (; i < GAME_LCDSIZE; i++) {
		status->buffer[i] = LCD_CHAR_SPACE;
	}
}


void delay(int i) {
	for (; i > 0; i--) {}
}

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

void FTM0_IRQHandler(void) 
{
	wait = 0;
	TPM0_SC |= GAME_TOF_BIT;   	// Clear TOF 
}

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

int main(void)
{
	// Pin setup for the input button and the LCD
	lcdSetup();
	ibSetup();
	
	// Set up LCD Screen
	lcdFunctionSet(LCD_8BIT | LCD_LORES | LCD_2LINE);
	lcdDisplay(LCD_CURSOROFF | LCD_DISPLAYON | LCD_BLINKOFF);
	lcdClear();
	lcdHome();
	
	// Set up timer
	setupTimer();
	
	// Endless game loop
	gameLoop();
	
	return 0;
}
