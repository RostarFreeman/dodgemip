/*
 * game.h
 *
 *  Created on: May 24, 2021
 *      Author: Pau
 */

#ifndef GAME_H_
#define GAME_H_

#define GAME_TOF_BIT (1 << 7)
#define GAME_INTERRUPT_BIT (1 << 6)

#define GAME_CHARACTER 0b11110100
#define GAME_BLOCK	   0b11111111
#define GAME_FW		   0b01111110
#define GAME_BW		   0b01111111
#define GAME_BLANK	   0b00100000

#define GAME_CLOCKFREQ		(int)(4000000)	// 4 MHz
#define GAME_CPUFREQ 		(int)(48000000) // 48 MHz
#define GAME_UPDATE_RATE 	(int)(24) 		// Frames per second

void setupTimer();
void delay(int i);
void gameLoop();

#endif /* GAME_H_ */
