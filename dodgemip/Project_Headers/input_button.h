/**
 * @file: 		input_button.h
 * @revision:	1.0
 * @date:	 	27-May-2021
 * @brief: 		Implements input button handling with interrupts.
 */

#ifndef INPUT_BUTTON_H_
#define INPUT_BUTTON_H_

#include <stdint.h>

#include "derivative.h"
#include "pinout.h"

#define IRQ_PORTA (30)
#define BTN_NOBUTTON   (uint8_t)(0)
#define BTN_UP_PRESSED (uint8_t)(1)
#define BTN_DW_PRESSED (uint8_t)(2)

#define BTN_ACKNOWLEDGED (uint8_t)(1)
#define BTN_PENDING		 (uint8_t)(0)

/**
 * Captures the last pressed key. For the game this is enough since it only has
 * two possible inputs.
 */
typedef struct {
	uint8_t status_word;
	uint8_t ack;
}inputStatus_t;

void ibSetup();



#endif /* INPUT_BUTTON_H_ */

