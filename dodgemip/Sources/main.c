/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "pinout.h"
#include "lcd.h"
#include "game.h"
#include "input_button.h"

extern inputStatus_t ibStatus;


void delay(int i) {
	for (; i > 0; i--) {}
}

void setupTimer() {
	MCG_C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK; 	// INTERNAL CLOCK|MCGIRCLK ACTIVE(SET)
	MCG_C2 = MCG_C2_IRCS_MASK;                        	// SELECT FAST INTERNAL REFERENCE CLOCK (1)
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(3);                 	// MCGIRCLK IS SELECTED FOR TPM CLOCK
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK; 				  	// Enable clock to TPM0 
	
	TPM0_SC  = 0;             						  	// Disable timer while configuring
	
	// Set up for 32 fps approx
	TPM0_SC |= TPM_SC_PS(0b010);  						// Prescaler
	TPM0_MOD = TPM_MOD_MOD(15625); 						// Modulo value 
	TPM0_SC |= GAME_TOF_BIT;             				// Clear TOF 
	TPM0_SC |= GAME_INTERRUPT_BIT;       				// Enable timeout interrupt
    TPM0_SC |= TPM_SC_CMOD(0b01);   					// Enable timer free-running mode
    
	NVIC_ICPR |= 0x00020000;
	NVIC_ISER |= 0x00020000;
}

void FTM0_IRQHandler(void) 
{
	lcdWrite(GAME_CHARACTER);
	TPM0_SC |= GAME_TOF_BIT;   	// Clear TOF 
}

void gameLoop() {
	for (;;) {
		
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
	
	// Endless app loop
	gameLoop();
	
	return 0;
}
