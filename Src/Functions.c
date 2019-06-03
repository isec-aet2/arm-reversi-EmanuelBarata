/*
 * Functions.c
 *
 *  Created on: May 27, 2019
 *      Author: Utilizador
 */

#include "Functions.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery.h"
#include "fatfs.h"
#include <string.h>
#include <stdio.h>


int countPieces(int player){

	uint32_t ColorArray[]={LCD_COLOR_BLACK,LCD_COLOR_WHITE};
	int pieceCounter=0;

	int posX0=boardX0 + boardPlaceWidth/2;
	int posXF= posX0+(DIMENSION*boardPlaceWidth);
	int posY0=boardY0 + boardPlaceHeight/2;
	int posYF= posY0+(DIMENSION*boardPlaceHeight);

	for(int i = posX0; i < posXF; i+=boardPlaceWidth){
		for(int j = posY0; j < posYF; j+=boardPlaceHeight){

			if(BSP_LCD_ReadPixel(i,j) == ColorArray[player]){

				pieceCounter++;


			}

		}
	}

	return pieceCounter;

}


BOOL endGame(int player, int possibleMoves, int * timeOuts){

	int xSize = BSP_LCD_GetXSize();
	int ySize = BSP_LCD_GetYSize();

	int nPieces;
	int wPieces;
	int bPieces;

	char victory[STRSIZE];
	char vicPlayer[STRSIZE];
	char reason[STRSIZE];
	char vicPlayerScore[STRSIZE];

	wPieces = countPieces(PLAYERWHITE);
	bPieces = countPieces(PLAYERBLACK);

	sprintf(victory, "GAME OVER");

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	if(verifyPossibleMoves(player, possibleMoves)){

		if(bPieces > wPieces){
			sprintf(vicPlayer, "WINNER SIRIUS BLACK");
			sprintf(vicPlayerScore, "SCORE: %i",bPieces);

		}
		else{
			sprintf(vicPlayer, "WINNER WHITE SNOW");
			sprintf(vicPlayerScore, "SCORE: %i",wPieces);
		}

		if(verifyFullBoard()==FALSE){
			sprintf(reason, "NO POSSIBLE MOVES!");
		}

		else{
			sprintf(reason, "FULL BOARD PONTUATION");
		}

		BSP_LCD_FillRect(STATSX0, boardY0 , xSize - STATSX0, ySize - boardY0 - 10);

		BSP_LCD_SetTextColor(0xFF606060);

		BSP_LCD_SetFont(&Font20);
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

		BSP_LCD_DisplayStringAt(STATSX0	, boardY0, (uint8_t*) victory , LEFT_MODE);
		BSP_LCD_DisplayStringAt(STATSX0	, boardY0+50, (uint8_t*) vicPlayer, LEFT_MODE);

		BSP_LCD_SetFont(&Font16);

		BSP_LCD_DisplayStringAt(STATSX0	, boardY0+150, (uint8_t*) reason, LEFT_MODE);
		BSP_LCD_DisplayStringAt(STATSX0	, boardY0+200, (uint8_t*) vicPlayerScore, LEFT_MODE);

		writeGameInfoSD (vicPlayer, reason, vicPlayerScore);

		return TRUE;

	}

	if(verifyTimeOuts(!player, timeOuts)){

		if(player==PLAYERWHITE){
			sprintf(vicPlayer, "CONGTRATS WHITE SNOW");
		}
		else{
			sprintf(vicPlayer, "CONGRATS SIRIUS BLACK");
		}

		nPieces = countPieces(player);
		sprintf(vicPlayerScore, "SCORE: %i",nPieces);

		BSP_LCD_FillRect(STATSX0, boardY0 , xSize - STATSX0, ySize - boardY0 - 10);
		sprintf(reason, "OUT OF TIME (3)!");

		BSP_LCD_SetTextColor(0xFF606060);

		BSP_LCD_SetFont(&Font20);
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
		BSP_LCD_DisplayStringAt(STATSX0	, boardY0, (uint8_t*) victory , LEFT_MODE);

		BSP_LCD_SetFont(&Font16);
		BSP_LCD_DisplayStringAt(STATSX0	, boardY0+100, (uint8_t*) vicPlayer, LEFT_MODE);
		BSP_LCD_DisplayStringAt(STATSX0	, boardY0+150, (uint8_t*) reason, LEFT_MODE);
		BSP_LCD_DisplayStringAt(STATSX0	, boardY0+200, (uint8_t*) vicPlayerScore, LEFT_MODE);

		writeGameInfoSD (vicPlayer, reason, vicPlayerScore);

		return TRUE;

	}


	return FALSE;


}


int findPossiblePlaces(int player){

	int countPossiblePlaces = 0;

    for(int i=boardX0; i<=DIMENSION*boardPlaceWidth; i+=boardPlaceWidth){
        for(int j=boardY0 ; j<=DIMENSION*boardPlaceHeight; j+=boardPlaceHeight){

        	if(possiblePlace(i, j, player)==TRUE){

        		countPossiblePlaces++;

        	}

        }
    }

    return countPossiblePlaces;

}


void gameStats(int player, int possibleMoves){

	int pieceCountW;
	int pieceCountB;

	char title[STRSIZE];
	char playerName[STRSIZE];
	char possibleMovesStr[STRSIZE];
	char timeTotal[STRSIZE];
	char timePlay[STRSIZE];
	char p1Score[STRSIZE];
	char p2Score[STRSIZE];
	char timeArr[STRSIZE];

	int xSize = BSP_LCD_GetXSize();

	pieceCountW = countPieces(PLAYERWHITE);
	pieceCountB = countPieces(PLAYERBLACK);


	sprintf(title,"GAME STATS");

	sprintf(possibleMovesStr,"POSSIBLE MOVES: %i", possibleMoves);

	sprintf(timeTotal,"GAME TIME (S): ");
	sprintf(timePlay, "REMAINING TIME (S):");

	sprintf(p1Score,"SNOW WHITE: %i", pieceCountW);
	sprintf(p2Score,"SIRIUS BLACK: %i", pieceCountB);

	if(player){
		sprintf(playerName,"PLAYER 1: SNOW WHITE");
	}

	else{
		sprintf(playerName,"PLAYER 2: SIRIUS BLACK");
	}

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	BSP_LCD_FillRect(STATSX0+110, boardY0+100, xSize-(STATSX0+110), 20);
	BSP_LCD_FillRect(STATSX0+165, boardY0+150, 50, 20);

	BSP_LCD_FillRect(STATSX0+125, boardY0+300, 50, 20);
	BSP_LCD_FillRect(STATSX0+125, boardY0+350, 50, 20);


	BSP_LCD_SetTextColor(0xFF606060);

	BSP_LCD_SetFont(&Font20);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(STATSX0	, boardY0, (uint8_t*) title, LEFT_MODE);

	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(STATSX0	, boardY0+100, (uint8_t*) playerName, LEFT_MODE);
	BSP_LCD_DisplayStringAt(STATSX0	, boardY0+150, (uint8_t*) possibleMovesStr, LEFT_MODE);
	BSP_LCD_DisplayStringAt(STATSX0	, boardY0+200, (uint8_t*) timeTotal, LEFT_MODE);
	BSP_LCD_DisplayStringAt(STATSX0	, boardY0+250, (uint8_t*) timePlay, LEFT_MODE);
	BSP_LCD_DisplayStringAt(STATSX0	, boardY0+300, (uint8_t*) p1Score, LEFT_MODE);
	BSP_LCD_DisplayStringAt(STATSX0	, boardY0+350, (uint8_t*) p2Score, LEFT_MODE);

	BSP_LCD_FillRect(STATSX0, boardY0+40, 200, 40);
	BSP_LCD_SetBackColor(0xFF606060);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	sprintf(timeArr,"CLEAR GAME TIME");
	BSP_LCD_DisplayStringAt(STATSX0+15	, boardY0+55, (uint8_t*) timeArr, LEFT_MODE);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

}


void MENU(){

	char title[STRSIZE];
	char option1[STRSIZE];
	char option2[STRSIZE];

	int xSize;
	//int YSize;

	BSP_LCD_SetTextColor(0xFF606060);
	BSP_LCD_SetFont(&Font24);

	sprintf(title,"REVERSI ISREVER");
	sprintf(option1,"VS PLAYER");
	sprintf(option2,"VS MACHINE");

	BSP_LCD_DisplayStringAt(TITLEX0	, TITLEY0, (uint8_t *) title, CENTER_MODE);

	xSize=BSP_LCD_GetXSize();
	//YSize=BSP_LCD_GetYSize();

	BSP_LCD_DrawRect(xSize/2-OPTION1X0SUB, OPTION1Y0, OPTION1WIDTH, OPTION1HEIGHT);

	BSP_LCD_SetFont(&Font20);
	BSP_LCD_DisplayStringAt(TITLEX0, OPTION1Y0+OPTION1Y0SUM, (uint8_t *) option1, CENTER_MODE);


	BSP_LCD_DrawRect(xSize/2-OPTION1X0SUB, OPTION1Y0 + 70 , OPTION1WIDTH, OPTION1HEIGHT);

	BSP_LCD_SetFont(&Font20);
	BSP_LCD_DisplayStringAt(TITLEX0, OPTION1Y0+OPTION1Y0SUM + 70, (uint8_t *) option2, CENTER_MODE);


}


int placePiece(int x0, int y0, int player){

	int inBoard=0;
	BSP_LCD_SetFont(&Font20);

	for(int i = boardX0, j = boardX0 + boardPlaceWidth; i <= boardX0 * DIMENSION; i+=boardPlaceWidth, j+=boardPlaceWidth){

		if(x0 >= i && x0 < j){
			x0=i+boardPlaceWidth/2;
			inBoard++;
		}
	}

	for(int i = boardY0, j = boardY0 + boardPlaceHeight; i <= boardY0 * DIMENSION; i+=boardPlaceHeight, j+=boardPlaceHeight){

			if(y0 >= i && y0 < j){
				y0=i+boardPlaceHeight/2;
				inBoard++;
			}
	}

	if(inBoard==2){

		if(BSP_LCD_ReadPixel(x0-boardPlaceWidth/2,y0)==LCD_COLOR_LIGHTGREEN){
			if(BSP_LCD_ReadPixel(x0+boardPlaceWidth/2,y0)==LCD_COLOR_LIGHTGREEN){
				if(BSP_LCD_ReadPixel(x0,y0+boardPlaceHeight/2)==LCD_COLOR_LIGHTGREEN){
					if(BSP_LCD_ReadPixel(x0,y0-boardPlaceHeight/2)==LCD_COLOR_LIGHTGREEN){

						if(BSP_LCD_ReadPixel(x0,y0)==LCD_COLOR_LIGHTGRAY){

							transColor(x0-boardPlaceWidth/2, y0-boardPlaceHeight/2, player);

							if(player%2==0){
							  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
							  player=PLAYERWHITE;
							}

							else{
							  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
							  player=PLAYERBLACK;
							}

							BSP_LCD_DrawCircle(x0, y0, 20);
							BSP_LCD_FillCircle(x0, y0, 20);



						}
					}
				}
			}
		}
	}

	return player;

}

BOOL possiblePlace(int Xpos, int Ypos, int player){

	uint32_t ColorArray[]={LCD_COLOR_WHITE,LCD_COLOR_BLACK};

	if(BSP_LCD_ReadPixel(Xpos+boardPlaceWidth/2,Ypos+boardPlaceHeight/2)==LCD_COLOR_LIGHTGRAY){

		for(int dx = -1; dx < 2; dx++){
			for(int dy = -1; dy < 2; dy++){

				if(BSP_LCD_ReadPixel(Xpos+(0.5+dx)*boardPlaceWidth,Ypos+(0.5+dy)*boardPlaceHeight)==ColorArray[player]){

					if(Xpos+(0.5+dx)*boardPlaceWidth > boardX0 && Xpos+(0.5+dx)*boardPlaceWidth < boardX0 + DIMENSION * boardPlaceWidth)
						if(Ypos+(0.5+dy)*boardPlaceHeight > boardY0 && Ypos+(0.5+dy)*boardPlaceHeight < boardY0 + DIMENSION * boardPlaceHeight){

							if(verifyEncapsulate(Xpos,Ypos,player)==TRUE){

								BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGREEN);
								BSP_LCD_DrawRect(Xpos, Ypos, boardPlaceWidth, boardPlaceHeight);

								return TRUE;
							}
						}

				}

			}

		}
	}
	return FALSE;
}

void printBoard(int * pTotalTime){

	int posX0 = boardX0;
	int posXF = boardX0 + DIMENSION*boardPlaceWidth;
	int xLength = boardPlaceWidth;
	int posY0 = boardY0;
	int posYF = boardY0 + DIMENSION*boardPlaceHeight;
	int yLength = boardPlaceHeight;

	*pTotalTime = 0;

	for(int i = posX0; i < posXF; i += xLength){

		for(int j = posY0; j < posYF; j += yLength){

			BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
			BSP_LCD_FillRect(i, j, xLength, yLength);
			BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
			BSP_LCD_DrawRect(i, j, xLength, yLength);

			if ( ( i==(DIMENSION/2)*xLength && j == (DIMENSION/2)*yLength ) || (  i==(DIMENSION/2 +1 )*xLength && j == (DIMENSION/2 +1 )*yLength ) ){

			  BSP_LCD_SetFont(&Font20);
			  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			  BSP_LCD_DrawCircle(i+xLength/2.0, j+yLength/2.0, 20);
			  BSP_LCD_FillCircle(i+xLength/2, j+yLength/2, 20);
			}
			else if( ( i==(DIMENSION/2+1)*xLength && j == (DIMENSION/2)*yLength ) || (  i==(DIMENSION/2)*xLength && j == (DIMENSION/2 +1 )*yLength ) ){

			  BSP_LCD_SetFont(&Font20);
			  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			  BSP_LCD_DrawCircle(i+xLength/2.0, j+yLength/2.0, 20);
			  BSP_LCD_FillCircle(i+xLength/2, j+yLength/2, 20);

			}
		}
	}

}

void printEndMessage(){

	char pbMessage[STRSIZE];
	char pbMessage2[STRSIZE];

	sprintf(pbMessage, "PRESS PUSH BOTTOM");
	sprintf(pbMessage2, "TO RETURN TO MENU");

	BSP_LCD_SetTextColor(0xFF606060);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(STATSX0, boardY0+250, (uint8_t*) pbMessage, LEFT_MODE);
	BSP_LCD_DisplayStringAt(STATSX0, boardY0+270, (uint8_t*) pbMessage2, LEFT_MODE);

}

int printPlayTime(int timeCount, int player, int* timeOutCount){

	int playTime;
	char playTimeStr[STRSIZE];

	playTime=20-(timeCount%20);

	if(playTime==9){

		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(STATSX0+210, boardY0+250, 50, 20);

	}

	BSP_LCD_SetTextColor(0xFF606060);
	BSP_LCD_SetFont(&Font16);
	sprintf(playTimeStr,"%i", playTime);

	BSP_LCD_DisplayStringAt(STATSX0+210, boardY0+250, (uint8_t*) playTimeStr, LEFT_MODE);

	if(playTime==1){

		HAL_Delay(1000);

		timeOutCount[player]++;

		player = !player;

		return player;

	}

	return player;

}


void printTotalGameTime(int timeCount){

	char totTime[STRSIZE];

	BSP_LCD_SetTextColor(0xFF606060);
	BSP_LCD_SetFont(&Font16);
	sprintf(totTime,"%i", timeCount);

	BSP_LCD_DisplayStringAt(STATSX0+160, boardY0+200, (uint8_t*) totTime, LEFT_MODE);


}


void refreshBoard(){

	for(int i=boardX0; i<boardX0+DIMENSION*boardPlaceWidth; i+=boardPlaceWidth){

		for(int j=boardY0; j<boardY0+DIMENSION*boardPlaceHeight; j+=boardPlaceHeight){

		  	  BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
		      BSP_LCD_DrawRect(i, j, boardPlaceWidth, boardPlaceHeight);
		}
	}

}

int robotChoice(int possibleMoves){

	int randPos = rand()%possibleMoves;
	int possCounter=0;

	int posX0=boardX0 + boardPlaceWidth/2;
	int posXF= posX0+(DIMENSION*boardPlaceWidth);
	int posY0=boardY0 + boardPlaceHeight/2;
	int posYF= posY0+(DIMENSION*boardPlaceHeight);

	for(int i = posX0; i < posXF; i+=boardPlaceWidth){
		for(int j = posY0; j < posYF; j+=boardPlaceHeight){


			if(BSP_LCD_ReadPixel(i-boardPlaceWidth/2,j)==LCD_COLOR_LIGHTGREEN){
				if(BSP_LCD_ReadPixel(i+boardPlaceWidth/2,j)==LCD_COLOR_LIGHTGREEN){
					if(BSP_LCD_ReadPixel(i,j+boardPlaceHeight/2)==LCD_COLOR_LIGHTGREEN){
						if(BSP_LCD_ReadPixel(i,j-boardPlaceHeight/2)==LCD_COLOR_LIGHTGREEN){

							if(BSP_LCD_ReadPixel(i,j)==LCD_COLOR_LIGHTGRAY){

								if(possCounter==randPos){

									BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

									HAL_Delay(500);

									BSP_LCD_DrawCircle(i, j, 20);
									BSP_LCD_FillCircle(i, j, 20);

									transColor(i-boardPlaceWidth/2, j-boardPlaceHeight/2, PLAYERBLACK);

									return PLAYERWHITE;

								}
								else
									possCounter++;
							}
						}
					}
				}
			}
		}
	}

	return PLAYERBLACK;
}

BOOL selectMenuOption(int x, int y, BOOL* robotFlag){

	int xSize=BSP_LCD_GetXSize();

	int pX0 = xSize/2-OPTION1X0SUB;
	int pXF = pX0 + OPTION1WIDTH;
	int pY0 = OPTION1Y0;
	int pYF = OPTION1Y0+OPTION1HEIGHT;

	char option1[STRSIZE];

	int p2X0 = xSize/2-OPTION1X0SUB;
	int p2XF = p2X0 + OPTION1WIDTH;
	int p2Y0 = OPTION1Y0+70;
	int p2YF = p2Y0+OPTION1HEIGHT;

	char option2[STRSIZE];


	if(x > pX0 && x < pXF && y > pY0  && y < pYF ){

			BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
			BSP_LCD_FillRect(pX0, pY0, OPTION1WIDTH, OPTION1HEIGHT);

			BSP_LCD_SetFont(&Font20);
			BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGREEN);

			sprintf(option1,"VS PLAYER");
			BSP_LCD_DisplayStringAt(TITLEX0, pY0+OPTION1Y0SUM, (uint8_t *) option1, CENTER_MODE);

			HAL_Delay(500);

			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			BSP_LCD_FillRect(pX0, TITLEY0, OPTION1WIDTH+50, 100);
			BSP_LCD_FillRect(pX0, pY0, OPTION1WIDTH+1, OPTION1HEIGHT+1);
			BSP_LCD_FillRect(pX0, p2Y0, OPTION1WIDTH+1, OPTION1HEIGHT+1);

			return FALSE;
	}

	else if(x > p2X0 && x < p2XF && y > p2Y0  && y < p2YF ){

		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_FillRect(p2X0, p2Y0, OPTION1WIDTH, OPTION1HEIGHT);

		BSP_LCD_SetFont(&Font20);
		BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);
		BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGREEN);

		sprintf(option2,"VS MACHINE");
		BSP_LCD_DisplayStringAt(TITLEX0, p2Y0+OPTION1Y0SUM, (uint8_t *) option2, CENTER_MODE);

		HAL_Delay(500);

		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(pX0, TITLEY0, OPTION1WIDTH+50, 100);
		BSP_LCD_FillRect(pX0, pY0, OPTION1WIDTH+1, OPTION1HEIGHT+1);
		BSP_LCD_FillRect(p2X0, p2Y0, OPTION1WIDTH+1, OPTION1HEIGHT+1);


		*robotFlag=TRUE;
		return FALSE;

	}

	return TRUE;

}


void transColor(int Xpos, int Ypos, int player){

	int advPieces;

	uint32_t ColorArray[]={LCD_COLOR_BLACK,LCD_COLOR_WHITE};

	for(int dx = -1; dx < 2; dx++){
		for(int dy = -1; dy < 2; dy++){

			advPieces=0;

			if(!(dx==0 && dy==0)){

				for(int i=1; i<DIMENSION;i++){

					if(Xpos+dx*i*boardPlaceWidth >=  boardX0  && Ypos+dy*i*boardPlaceHeight >= boardY0  && Xpos+dx*i*boardPlaceWidth < boardX0+DIMENSION*boardPlaceWidth && Ypos+dy*i*boardPlaceHeight < boardY0+DIMENSION*boardPlaceHeight ){


						if(BSP_LCD_ReadPixel(Xpos+(0.5+dx*i)*boardPlaceWidth, Ypos+(0.5+dy*i)*boardPlaceHeight)==LCD_COLOR_LIGHTGRAY){

							break;
						}

						if(BSP_LCD_ReadPixel(Xpos+(0.5+dx*i)*boardPlaceWidth, Ypos+(0.5+dy*i)*boardPlaceHeight)==ColorArray[!player]){

							advPieces++;

						}

						if(BSP_LCD_ReadPixel(Xpos+(0.5+dx*i)*boardPlaceWidth, Ypos+(0.5+dy*i)*boardPlaceHeight)==ColorArray[player]){

							if(advPieces>0){

								for(int i = 1; i <= advPieces; i++){

									BSP_LCD_SetTextColor(ColorArray[player]);
									BSP_LCD_DrawCircle(Xpos+(0.5+dx*i)*boardPlaceWidth, Ypos+(0.5+dy*i)*boardPlaceHeight, 20);
									BSP_LCD_FillCircle(Xpos+(0.5+dx*i)*boardPlaceWidth, Ypos+(0.5+dy*i)*boardPlaceHeight, 20);


								}

							}

							else{

								break;
							}
						}
					}
				}
			}
		}
	}

}


BOOL verifyEncapsulate(int Xpos, int Ypos, int player){

	int advPieces;

	uint32_t ColorArray[]={LCD_COLOR_BLACK,LCD_COLOR_WHITE};

	for(int dx = -1; dx < 2; dx++){
		for(int dy = -1; dy < 2; dy++){

			advPieces=0;

			if(!(dx==0 && dy==0)){

				for(int i=1; i<DIMENSION;i++){

					if(Xpos+dx*i*boardPlaceWidth >=  boardX0  && Ypos+dy*i*boardPlaceHeight >= boardY0  && Xpos+dx*i*boardPlaceWidth < boardX0+DIMENSION*boardPlaceWidth && Ypos+dy*i*boardPlaceHeight < boardY0+DIMENSION*boardPlaceHeight ){


						if(BSP_LCD_ReadPixel(Xpos+(0.5+dx*i)*boardPlaceWidth, Ypos+(0.5+dy*i)*boardPlaceHeight)==LCD_COLOR_LIGHTGRAY){

							break;
						}

						if(BSP_LCD_ReadPixel(Xpos+(0.5+dx*i)*boardPlaceWidth, Ypos+(0.5+dy*i)*boardPlaceHeight)==ColorArray[!player]){

							advPieces++;

						}

						if(BSP_LCD_ReadPixel(Xpos+(0.5+dx*i)*boardPlaceWidth, Ypos+(0.5+dy*i)*boardPlaceHeight)==ColorArray[player]){

							if(advPieces>0){

								return TRUE;
							}

							else{

								break;
							}
						}
					}
				}
			}
		}
	}

	return FALSE;

}

BOOL verifyPossibleMoves(int player, int possibleMoves){

	if(possibleMoves==0){

		return TRUE;
	}

	return FALSE;

}

BOOL verifyTimeOuts(int player, int * timeOuts){

	if(timeOuts[player] == 3){

		return TRUE;
	}

	return FALSE;

}

BOOL verifyFullBoard(){

	int x0 = boardX0+boardPlaceWidth/2;
	int y0 = boardY0+boardPlaceHeight/2;
	int xF = boardX0 + DIMENSION*boardPlaceWidth/2;
	int yF = boardY0 + DIMENSION*boardPlaceHeight/2;

	for(int i = x0; i < xF; i += boardPlaceWidth){
		for(int j = y0; j < yF; j += boardPlaceHeight ){

			if(BSP_LCD_ReadPixel(i, j) == LCD_COLOR_LIGHTGRAY){

				return FALSE;

			}

		}

	}
	return TRUE;


}

void writeGameInfoSD (char * player, char * reason, char * score){

	UINT nBytes;
	char string1[STRSIZE*3];

	sprintf(string1, player);

	strcat(string1," ");
	strcat(string1, reason);
	strcat(string1," ");
	strcat(string1,score);
	strcat(string1," ");


    if(f_mount (&SDFatFS, SDPath, 0)!=FR_OK){
        Error_Handler();
        BSP_LCD_SetTextColor(LCD_COLOR_BROWN);
        BSP_LCD_FillCircle(650, 400, 10);
    }

    HAL_Delay(100);

//    if(f_open (&SDFile, "Res.txt", FA_WRITE | FA_CREATE_ALWAYS )!=FR_OK){
//        Error_Handler();
//        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
//        BSP_LCD_FillCircle(660, 400, 10);
//    }


    if(f_open (&SDFile, "Res.txt", FA_OPEN_APPEND | FA_WRITE )!=FR_OK){
        Error_Handler();
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        BSP_LCD_FillCircle(660, 400, 10);
    }

    HAL_Delay(100);

    if(f_write (&SDFile, string1, strlen(string1), &nBytes)!=FR_OK){

        Error_Handler();
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_FillCircle(670, 400, 10);

    }

    f_close (&SDFile);

}






