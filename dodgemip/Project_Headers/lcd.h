/**
 * @file: 		lcd.h
 * @revision:	1.0
 * @date:	 	27-May-2021
 * @brief: 		Implements lcd screen usage and abstraction.
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

#include "derivative.h"
#include "pinout.h"

/* Config flags (lcdFunctionSet)*/
#define LCD_BITMODE  (uint8_t)(1 << 4)
#define LCD_LINEMODE (uint8_t)(1 << 3)
#define LCD_CHARRES  (uint8_t)(1 << 2)

#define LCD_8BIT  (uint8_t)(1 << 4)
#define LCD_4BIT  (uint8_t)(0 << 4)
#define LCD_2LINE (uint8_t)(1 << 3)
#define LCD_1LINE (uint8_t)(0 << 3)
#define LCD_HIRES (uint8_t)(1 << 2)
#define LCD_LORES (uint8_t)(0 << 2)

/* Display Flags (lcdDisplayCursor) */
#define LCD_DISPLAYON  (uint8_t)(1 << 2)
#define LCD_DISPLAYOFF (uint8_t)(0 << 2)
#define LCD_CURSORON   (uint8_t)(1 << 1)
#define LCD_CURSOROFF  (uint8_t)(0 << 1)
#define LCD_BLINKON    (uint8_t)(1 << 0)
#define LCD_BLINKOFF   (uint8_t)(0 << 0)

/* Entry mode flags (lcdEntryMode) */
#define LCD_DIR      (uint8_t)(1 << 1)
#define LCD_SHIFT    (uint8_t)(1 << 0)

#define LCD_RIGHT    (uint8_t)(0 << 1)
#define LCD_LEFT     (uint8_t)(1 << 1)
#define LCD_SHIFTOFF (uint8_t)(0 << 0)
#define LCD_SHIFTON  (uint8_t)(1 << 0)

/* Word2Screen setting pins */
#define LCD_SET_RS_INDEX (0)
#define LCD_SET_RW_INDEX (1)

#define LCD_SET_RS (uint8_t)(1 << LCD_SET_RS_INDEX)
#define LCD_SET_RW (uint8_t)(1 << LCD_SET_RW_INDEX)

/* Character table */
#define LCD_CHAR_A		0b01000001
#define LCD_CHAR_B		0b01000010
#define LCD_CHAR_C		0b01000011
#define LCD_CHAR_D		0b01000100
#define LCD_CHAR_E		0b01000101
#define LCD_CHAR_F		0b01000110
#define LCD_CHAR_G		0b01000111
#define LCD_CHAR_H		0b01001000
#define LCD_CHAR_I		0b01001001
#define LCD_CHAR_J		0b01001010
#define LCD_CHAR_K		0b01001011
#define LCD_CHAR_L		0b01001100
#define LCD_CHAR_M		0b01001101
#define LCD_CHAR_N		0b01001110
#define LCD_CHAR_O		0b01001111

#define LCD_CHAR_P		0b01010000
#define LCD_CHAR_Q		0b01010001
#define LCD_CHAR_R		0b01010010
#define LCD_CHAR_S		0b01010011
#define LCD_CHAR_T		0b01010100
#define LCD_CHAR_U		0b01010101
#define LCD_CHAR_V		0b01010110
#define LCD_CHAR_W		0b01011111
#define LCD_CHAR_X		0b01011000
#define LCD_CHAR_Y		0b01011001
#define LCD_CHAR_Z	    0b01011010

#define LCD_CHAR_0		0b00110000
#define LCD_CHAR_1		0b00110001
#define LCD_CHAR_2		0b00110010
#define LCD_CHAR_3		0b00110011
#define LCD_CHAR_4		0b00110100
#define LCD_CHAR_5		0b00110101
#define LCD_CHAR_6		0b00110110
#define LCD_CHAR_7		0b00110111
#define LCD_CHAR_8		0b00111000
#define LCD_CHAR_9		0b00111001
#define LCD_CHAR_COLON  0b00111010

#define LCD_CHAR_SPACE  0b10100000

/* Control over character placement when writing on the lcd*/
typedef struct {
	int line;
	int column;
}lcdStatus_t;

void lcdSetup();
void lcdWord2Screen(uint8_t settings, uint8_t data);
void lcdWriteDelay();
void lcdDisplacementDelay();

void lcdWrite(uint8_t data);
inline void lcdEntryMode(uint8_t flags);
inline void lcdFunctionSet(uint8_t flags);
inline void lcdDisplay(uint8_t toggle);
inline void lcdClear();
inline void lcdHome();
void lcdPlaceCursor(int position);

#endif /* LCD_H_ */
