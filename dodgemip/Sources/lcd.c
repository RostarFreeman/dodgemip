/*
 * lcd.c
 *
 *  Created on: May 9, 2021
 *      Author: Pau
 */


#include "lcd.h"

lcdStatus_t line_status = {0, 0};


/**
 * @function: 	lcdSetup
 * @param: 		none
 * @return: 	none
 * @brief: 		Sets up pins for the LCD screen as output.
 */
void lcdSetup() {
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	// Port
	PORTC_BASE_PTR->PCR[LCD_DB0_PIN] = PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[LCD_DB1_PIN] = PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[LCD_DB2_PIN] = PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[LCD_DB3_PIN] = PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[LCD_DB4_PIN] = PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[LCD_DB5_PIN] = PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[LCD_DB6_PIN] = PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[LCD_DB7_PIN] = PORT_PCR_MUX(1);
	
	GPIOC_PDDR |= (LCD_DB0 | LCD_DB1 | LCD_DB2 |
				   LCD_DB3 | LCD_DB4 | LCD_DB5 | 
				   LCD_DB6 | LCD_DB7 );
	
	// Control
	PORTC_BASE_PTR->PCR[LCD_RS_PIN] = PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[LCD_RW_PIN] = PORT_PCR_MUX(1);
	PORTC_BASE_PTR->PCR[LCD_E_PIN] = PORT_PCR_MUX(1);
	
	GPIOC_PDDR |= (LCD_RS | LCD_RW | LCD_E);
	
}

/**
 * @function: 	lcdWord2Screen
 * @param: 		settings: Flags for control pins RS, RW
 * 		   		data: Data to send over DB0-DB7 to the LCD
 * @return: 	none 
 * @brief: 		Abstracts pinout when writing to the lcd
 * 		   		as two register writes. Delays for 2000 cycles.
 */
void lcdWord2Screen(uint8_t settings, uint8_t data) {
	GPIOC_PDOR &= ~(LCD_DB0 | LCD_DB1 | LCD_DB2 | LCD_DB3 | 
					LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7 | 
					LCD_RS  | LCD_RW  | LCD_E);
	
	GPIOC_PDOR |= (((settings & LCD_SET_RS) >> LCD_SET_RS_INDEX) << LCD_RS_PIN) |
				  (((settings & LCD_SET_RW) >> LCD_SET_RW_INDEX) << LCD_RW_PIN);
	
	GPIOC_PDOR |=   LCD_E;
	
	GPIOC_PDOR |= (((data & (1 << 0)) >> 0) << LCD_DB0_PIN) |
				  (((data & (1 << 1)) >> 1) << LCD_DB1_PIN) |
				  (((data & (1 << 2)) >> 2) << LCD_DB2_PIN) |
				  (((data & (1 << 3)) >> 3) << LCD_DB3_PIN) |
				  (((data & (1 << 4)) >> 4) << LCD_DB4_PIN) |
				  (((data & (1 << 5)) >> 5) << LCD_DB5_PIN) |
				  (((data & (1 << 6)) >> 6) << LCD_DB6_PIN) |
				  (((data & (1 << 7)) >> 7) << LCD_DB7_PIN) ;
	
	lcdWriteDelay();
	
	GPIOC_PDOR &= ~(LCD_E);
	
	lcdWriteDelay();
}

/**
 * @function: 	lcdWriteDelay
 * @param: 		none
 * @return: 	none
 * @brief: 		Idles system for 250 cycles
 */
void lcdWriteDelay() {
	int i = 250;
	
	for (; i > 0; i--) {}
}


/**
 * @function: 	lcdDisplacementDelay
 * @param: 		none
 * @return: 	none
 * @brief: 		Idles system for 1500 cycles
 */
void lcdDisplacementDelay() {
	int i = 1500;
	
	for (; i > 0; i--) {}
}

/**
 * @function: 	lcdWrite
 * @param: 		data: Character to be written on the lcd screen
 * @return: 	none
 * @brief: 		Writes character on the lcd screen. Loops over all 32 positions 
 * 				cyclically.
 */
void lcdWrite(uint8_t data) {
	lcdWord2Screen(LCD_SET_RS, data);
	line_status.column += 1;
	if (line_status.column == 16) {
		line_status.column = 0;
		
		if (line_status.line == 0) {
			line_status.line = 1;
			lcdPlaceCursor(16);
		}
		else {
			line_status.line = 0;
			lcdPlaceCursor(0);
		}
	}
}

/**
 * @function: 	lcdClear
 * @param: 		none
 * @return: 	none
 * @brief: 		Clears contents of the screen
 */
inline void lcdClear() {
	lcdWord2Screen(0, 0b00000001);
}

/**
 * @function: 	lcdHome
 * @param: 		none
 * @return: 	none
 * @brief: 		Places cursor on the starting position
 */
inline void lcdHome() {
	lcdWord2Screen(0, 0b00000011);
	line_status.line = 0;
	line_status.column = 0;
	lcdDisplacementDelay();
}

/**
 * @function: 	lcdFunctionSet
 * @param: 		flags: Configuration flags (see lcd.h)
 * @return: 	none
 * @brief: 		Configures operation mode for the lcd screen 
 * 		   		(8bit/4bit, 1/2 lines, 8/10 px characters)
 */
inline void lcdFunctionSet(uint8_t flags) {
	uint8_t mode = 0b00100000 | flags;
	lcdWord2Screen(0, mode);
}

/**
 * @function: 	lcdDisplay
 * @param: 		flags: Configuration flags (see lcd.h)
 * @return: 	none
 * @brief: 		Configures display and the cursor (cursor on/off, 
 * 		   		cursor blink, display stuff on-screen)
 */
inline void lcdDisplay(uint8_t flags) {
	uint8_t mode = 0b00001000 | flags;
	lcdWord2Screen(0, mode);
}


/**
 * @function: 	lcdEntryMode
 * @param: 		flags: Configuration flags (see lcd.h)
 * @return: 	none
 * @brief: 		Configures writing direction and shifting.
 * 				Should not be used without default setting because
 * 				the code is not ready to consider writing on both 
 * 				directions.
 */
inline void lcdEntryMode(uint8_t flags) {
	uint8_t mode = 0b00000100 | flags;
	lcdWord2Screen(0, mode);
}

/**
 * @function: 	lcdPlaceCursor
 * @param: 		position: New placement for the cursor (consider reading
 * 				order and both rows when counting the position)
 * @return: 	none
 * @brief: 		Places cursor on the defined position. Further character
 * 				writes (lcdWrite calls) will be performed there.
 */
void lcdPlaceCursor(int position) {
	uint8_t address;
	if (position >= 16) {
		address = 0xC0 | (position & 0x0F);
		line_status.line = 1;
	}
	else {
		address = 0x80 | (position & 0x0F);
		line_status.line = 0;
	}
	line_status.column = position % 16;
	lcdWord2Screen(0, address);
	lcdDisplacementDelay();
	
}
