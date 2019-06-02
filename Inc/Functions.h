/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Functions_H
#define __Functions_H

#include "stm32f769i_discovery.h"



#define TEMP_REFRESH_PERIOD   251    /* Internal temperature refresh period */
#define MAX_CONVERTED_VALUE   4095    /* Max converted value */
#define AMBIENT_TEMP            25    /* Ambient Temperature */
#define VSENS_AT_AMBIENT_TEMP  760    /* VSENSE value (mv) at ambient temperature */
#define AVG_SLOPE               25    /* Avg_Solpe multiply by 10 */
#define VREF                  3300

#define boardX0					50
#define boardY0 				50
#define DIMENSION 				8
#define boardPlaceWidth			50
#define boardPlaceHeight		50

#define PLAYERWHITE				1
#define PLAYERBLACK				0

#define OPTION1X0SUB			100
#define OPTION1Y0				200
#define OPTION1Y0SUM			20
#define OPTION1WIDTH			200
#define OPTION1HEIGHT			60

#define TITLEX0					0
#define TITLEY0					100

#define STATSX0					500
#define STRSIZE					100


typedef enum bool{FALSE, TRUE} BOOL;

typedef struct Piece{
	uint32_t color;
	int posX;
	int posY;
}tPiece;



void printBoard(int * TotalTime);
void gameStats(int player, int possibleMoves);

int placePiece(int x0, int y0, int player);
void transColor(int Xpos, int Ypos, int player);
int countPieces(int player);
BOOL verifyEncapsulate(int Xpos, int Ypos, int player);
BOOL verifyPossibleMoves(int player, int possibleMoves);
BOOL verifyTimeOuts(int player, int * timeOuts);
BOOL verifyFullBoard();
BOOL possiblePlace(int Xpos, int Ypos, int player);
int findPossiblePlaces(int player);

void refreshBoard();

void MENU();
BOOL selectMenuOption(int x, int y, BOOL* robotFlag);

void printTotalGameTime(int timeCount);
int printPlayTime(int timeCount, int player, int* timeOutCount);

BOOL endGame(int player, int possibleMoves, int * timeOuts);
void printEndMessage();

void writeGameInfoSD (char * player, char * reason, char * score);

int robotChoice(int possibleMoves);


#ifdef __cplusplus
extern "C" {
#endif












#ifdef __cplusplus
}
#endif

#endif /* __Functions_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
