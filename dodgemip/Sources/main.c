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
	lcdEntryMode(LCD_8BIT | LCD_LORES | LCD_2LINE);
	delay(100000);
	lcdDisplayCursor(LCD_CURSOROFF | LCD_DISPLAYON | LCD_BLINKOFF);
	delay(100000);
	lcdClear();
	delay(1000000);
	lcdHome();
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_CHARACTER);
	delay(10000);
	lcdWrite(GAME_FW);
	delay(10000);
	lcdWrite(GAME_BW);
	delay(10000);
	lcdWrite(GAME_BLOCK);
	delay(10000);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_CHARACTER);
	delay(10000);
	lcdWrite(GAME_FW);
	delay(10000);
	lcdWrite(GAME_BW);
	delay(10000);
	lcdWrite(GAME_BLOCK);
	delay(10000);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_CHARACTER);
	delay(10000);
	lcdWrite(GAME_FW);
	delay(10000);
	lcdWrite(GAME_BW);
	delay(10000);
	lcdWrite(GAME_BLOCK);
	delay(10000);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_CHARACTER);
	delay(10000);
	lcdWrite(GAME_FW);
	delay(10000);
	lcdWrite(GAME_BW);
	delay(10000);
	lcdWrite(GAME_BLOCK);
	delay(10000);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_CHARACTER);
	delay(10000);
	lcdWrite(GAME_FW);
	delay(10000);
	lcdWrite(GAME_BW);
	delay(10000);
	lcdWrite(GAME_BLOCK);
	delay(10000);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_CHARACTER);
	delay(10000);
	lcdWrite(GAME_FW);
	delay(10000);
	lcdWrite(GAME_BW);
	delay(10000);
	lcdWrite(GAME_BLOCK);
	delay(10000);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_CHARACTER);
	delay(10000);
	lcdWrite(GAME_FW);
	delay(10000);
	lcdWrite(GAME_BW);
	delay(10000);
	lcdWrite(GAME_BLOCK);
	delay(10000);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_CHARACTER);
	delay(10000);
	lcdWrite(GAME_FW);
	delay(10000);
	lcdWrite(GAME_BW);
	delay(10000);
	lcdWrite(GAME_BLOCK);
	delay(10000);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_BLANK);
	delay(10000);
	lcdWrite(GAME_CHARACTER);
	delay(10000);
	lcdWrite(GAME_FW);
	delay(10000);
	lcdWrite(GAME_BW);
	delay(10000);
	lcdWrite(GAME_BLOCK);
	delay(10000);
	
	
	
	for(;;) {}
	
	return 0;
}
