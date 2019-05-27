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


#define boardX0					50
#define boardY0 				50
#define DIMENSION 				8
#define boardPlaceWidth			50
#define boardPlaceHeight		50

#define PLAYERWHITE				1
#define PLAYERBLACK				0


typedef struct Piece{
	uint32_t color;
	int posX;
	int posY;
}tPiece;



void printBoard(int x0, int y0, int dimension, int xLength, int yLength);
int placePiece(int x0, int y0, int player);
void possiblePlace(int Xpos, int Ypos, int player);
void findPossiblePlaces(int player);



#ifdef __cplusplus
extern "C" {
#endif












#ifdef __cplusplus
}
#endif

#endif /* __Functions_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
