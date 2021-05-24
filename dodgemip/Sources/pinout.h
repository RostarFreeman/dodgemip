/*
 * pinout.h
 *
 *  Created on: May 9, 2021
 *      Author: Pau
 */

#ifndef PINOUT_H_
#define PINOUT_H_

// All pins are connected to Port C

#define LCD_RS_PIN (16)
#define LCD_RW_PIN (13)
#define LCD_E_PIN (12)

#define LCD_RS (1 << LCD_RS_PIN)
#define LCD_RW (1 << LCD_RW_PIN)
#define LCD_E  (1 << LCD_E_PIN)

#define LCD_DB0_PIN	(11) 
#define LCD_DB1_PIN (10)
#define LCD_DB2_PIN (6)
#define LCD_DB3_PIN (5)
#define LCD_DB4_PIN (4)
#define LCD_DB5_PIN (3)
#define LCD_DB6_PIN (0) 
#define LCD_DB7_PIN (7)

#define LCD_DB0	(1 << LCD_DB0_PIN) 
#define LCD_DB1 (1 << LCD_DB1_PIN)
#define LCD_DB2 (1 << LCD_DB2_PIN)
#define LCD_DB3 (1 << LCD_DB3_PIN)
#define LCD_DB4 (1 << LCD_DB4_PIN)
#define LCD_DB5 (1 << LCD_DB5_PIN)
#define LCD_DB6 (1 << LCD_DB6_PIN) 
#define LCD_DB7 (1 << LCD_DB7_PIN)


#define BTN_UP_PIN (9)
#define BTN_DW_PIN (8)
#define BTN_UP (1 << BTN_UP_PIN)
#define BTN_DW (1 << BTN_DW_PIN)



#endif /* PINOUT_H_ */
