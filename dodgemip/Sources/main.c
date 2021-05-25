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


int main(void)
{
	lcdSetup();
	ibSetup();
	
	lcdFunctionSet(LCD_8BIT | LCD_LORES | LCD_2LINE);
	lcdDisplay(LCD_CURSOROFF | LCD_DISPLAYON | LCD_BLINKOFF);
	lcdClear();
	lcdHome();
	
	for(;;) {
		int i = 0;
		for (i = 0; i < 32; i++) {
			switch (ibStatus.status_word)
			{
			case BTN_NOBUTTON:
				lcdWrite(GAME_BLANK);
				break;
			case BTN_UP_PRESSED:
				lcdWrite(GAME_FW);
				break;
			case BTN_DW_PRESSED:
				lcdWrite(GAME_BW);
				break;
			default:
				lcdWrite(GAME_BLOCK);
				break;
			}
			delay(25000);
		}
	}
	
	return 0;
}
