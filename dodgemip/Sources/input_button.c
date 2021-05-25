/*
 * input_button.c
 *
 *  Created on: May 24, 2021
 *      Author: Pau
 */


#include "input_button.h"

inputStatus_t ibStatus = {BTN_NOBUTTON, BTN_ACKNOWLEDGED};

/**
 * @function: 	ibSetup
 * @param: 		none
 * @return: 	none
 * @brief: 		Sets up pins for the buttons as inputs.
 */
void ibSetup() {
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	NVIC_ICPR |= (1 << IRQ_PORTA);
	NVIC_ISER |= (1 << IRQ_PORTA);
	
	PORTA_BASE_PTR->PCR[BTN_UP_PIN] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0b1010) | 0x0001 | 0x0002;
	PORTA_BASE_PTR->PCR[BTN_DW_PIN] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0b1010) | 0x0001 | 0x0002;
	
	GPIOA_PDDR &= ~(BTN_UP | BTN_DW);
}

/**
 * @function: 	PORTA_IRQHandler
 * @param: 		none
 * @return: 	none
 * @brief: 		Captures input from buttons and modifies the ibStatus struct.
 */
void PORTA_IRQHandler() {
	if (!(BTN_UP & GPIOA_PDIR)) {
		ibStatus.status_word = BTN_UP_PRESSED;
		ibStatus.ack = BTN_PENDING;
	}
	else if (!(BTN_DW & GPIOA_PDIR)) {
		ibStatus.status_word = BTN_DW_PRESSED;
		ibStatus.ack = BTN_PENDING;
	}
	else {
		ibStatus.status_word = BTN_NOBUTTON;
		ibStatus.ack = BTN_ACKNOWLEDGED;
	}
	PORTA_ISFR |= BTN_UP | BTN_DW;
	return;
}
