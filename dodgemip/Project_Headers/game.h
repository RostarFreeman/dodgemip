/*
 * game.h
 *
 *  Created on: May 24, 2021
 *      Author: Pau
 */

#ifndef GAME_H_
#define GAME_H_

#include "input_button.h"
#include "lcd.h"

/* Globals */
extern inputStatus_t ibStatus; // Declared in input_button
int wait;
int seed;

/* Timer settings */
#define GAME_TOF_BIT (1 << 7)
#define GAME_INTERRUPT_BIT (1 << 6)

/* Timing */
#define GAME_CLOCKFREQ		(int)(4000000)	// 4 MHz
#define GAME_CPUFREQ 		(int)(48000000) // 48 MHz
#define GAME_UPDATE_RATE 	(int)(30) 		// Frames per second
#define GAME_DIFFTIME		(int)(5)		// Seconds for diff change
#define GAME_MAXDIFF		(int)(5)		// Maximum difficulty

/* Game object on-screen characters */
#define GAME_CHARACTER (uint8_t)(0b11110100)
#define GAME_BLOCK	   (uint8_t)(0b11111111)
#define GAME_FW		   (uint8_t)(0b01111110)
#define GAME_BW		   (uint8_t)(0b01111111)
#define GAME_BLANK	   (uint8_t)(0b00100000)

/* Misc */
#define GAME_LCDSIZE	(32)
#define GAME_NOENTITY	(99)


/* Possible game modes. Implemented as a state machine 
 * 		GAME_READYSCREEN	Initial "press up to play" screen with previous time
 * 		GAME_BLINKING		Blinking entity screen
 * 		GAME_INGAME			Game screen
 * 		GAME_GAMEOVER		Game over screen
 * */
typedef enum {
	GAME_READYSCREEN,
	GAME_BLINKING,
	GAME_INGAME,
	GAME_GAMEOVER,
} states_t;

/* 
 * Structure containing the state of the game. Could be done with a Union for
 * space efficiency, but it works well this way.
 * 		ticks:		 	Game update count.
 * 		state: 		 	Game mode.
 * 		buffer: 	 	LCD output (without game entity, which is rendered afterwards)
 * 		entityPos:	 	Entity placement on screen. GAME_NOENTITY if not present.
 * 		prevTime:	 	Previous survival time.
 * 		difficulty:	 	Number of ticks before obstacle displacement.
 * 		phaseTimer:  	Used to time specific events (scene transition, difficulty
 * 					 	change, blinking).
 * 		updateTimer: 	Timer keeping track of ticks before updating obstacles.
 */
typedef struct {
	unsigned int ticks;
	states_t state;
	uint8_t buffer[GAME_LCDSIZE];
	uint8_t entityPos;
	int prevTime;
	uint8_t difficulty;
	int phaseTimer;
	int updateTimer;
} gamestate_t;


unsigned int random();
void cleanBuffer(gamestate_t* status);
void displaceBuffer(gamestate_t* status);
void generateObstacles(gamestate_t* status);
void setupTimer();
void delay(int i);
void gameLoop();
uint8_t manageInputs();
void cleanBuffer(gamestate_t* status);
void updateGame(gamestate_t* status, uint8_t input);
void drawGame(gamestate_t* status);
void initialiseStatus(gamestate_t* status);
void waitFrame(gamestate_t* status);
void bufferUpToPlay(gamestate_t* status);
void gameOver(gamestate_t* status);

#endif /* GAME_H_ */
