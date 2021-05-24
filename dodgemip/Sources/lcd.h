#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

#include "derivative.h"
#include "pinout.h"

/* Config flags (lcdEntryMode)*/
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

/* Entry mode flags */
#define LCD_DIR      (uint8_t)(1 << 0)
#define LCD_SHIFT    (uint8_t)(1 << 1)

#define LCD_RIGHT    (uint8_t)(0 << 0)
#define LCD_LEFT     (uint8_t)(1 << 0)
#define LCD_SHIFTOFF (uint8_t)(0 << 1)
#define LCD_SHIFTON  (uint8_t)(1 << 1)

/* Word2Screen setting pins */
#define LCD_SET_RS_INDEX (0)
#define LCD_SET_RW_INDEX (1)

#define LCD_SET_RS (uint8_t)(1 << LCD_SET_RS_INDEX)
#define LCD_SET_RW (uint8_t)(1 << LCD_SET_RW_INDEX)


void lcdSetup();
void lcdWord2Screen(uint8_t settings, uint8_t data);

inline void lcdWrite(uint8_t data);
void lcdConfig(uint8_t flags);
inline void lcdEntryMode(uint8_t flags);
inline void lcdDisplayCursor(uint8_t toggle);
inline void lcdClear();
inline void lcdHome();
uint8_t lcdRead();

#endif /* LCD_H_ */
