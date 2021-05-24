/*
 * lcd.c
 *
 *  Created on: May 9, 2021
 *      Author: Pau
 */


#include "lcd.h"

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
	
	GPIOC_PDOR &= ~(LCD_E);
}



inline void lcdWrite(uint8_t data) {
	lcdWord2Screen(LCD_SET_RS, data);
}

inline void lcdClear() {
	lcdWord2Screen(0, 0b00000001);
}

inline void lcdHome() {
	lcdWord2Screen(0, 0b00000011);
}

inline void lcdEntryMode(uint8_t flags) {
	uint8_t mode = 0b00100000 | flags;
	lcdWord2Screen(0, mode);
}

inline void lcdDisplayCursor(uint8_t toggle) {
	uint8_t mode = 0b00001000 | toggle;
	lcdWord2Screen(0, mode);
}
