/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "pinout.h"
#include "lcd.h"
#include "game.h"

void delay(int i) {
	
	for (; i > 0; i--) {}
}

int main(void)
{
	lcdSetup();
	delay(100000);
	lcdFunctionSet(LCD_8BIT | LCD_LORES | LCD_2LINE);
	delay(100000);
	lcdDisplay(LCD_CURSOROFF | LCD_DISPLAYON | LCD_BLINKOFF);
	delay(100000);
	lcdClear();
	delay(1000000);
	lcdHome();
	



	for(;;) {
		int i = 0;
		for (i = 0; i < 32; i++) {
			lcdWrite(GAME_CHARACTER);
		}
		delay(250000);
		for (i = 0; i < 32; i++) {
			lcdWrite(GAME_BLOCK);
		}
		delay(250000);
	}
	
	return 0;
}
