/*
 * Functions.c
 *
 *  Created on: May 27, 2019
 *      Author: Utilizador
 */

# include "Functions.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery.h"
#include "main.h"


void printBoard(int x0, int y0, int dimension, int xLength, int yLength){
      for(int i=x0; i<=dimension*xLength; i+=xLength){
          for(int j=y0; j<=dimension*yLength; j+=yLength){

              BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
              BSP_LCD_FillRect(i, j, xLength, yLength);
        	  BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
              BSP_LCD_DrawRect(i, j, xLength, yLength);

              if ( ( i==(dimension/2)*xLength && j == (dimension/2)*yLength ) || (  i==(dimension/2 +1 )*xLength && j == (dimension/2 +1 )*yLength ) ){

            	  BSP_LCD_SetFont(&Font20);
            	  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
            	  BSP_LCD_DrawCircle(i+xLength/2.0, j+yLength/2.0, 20);
            	  BSP_LCD_FillCircle(i+xLength/2, j+yLength/2, 20);
              }
              else if( ( i==(dimension/2+1)*xLength && j == (dimension/2)*yLength ) || (  i==(dimension/2)*xLength && j == (dimension/2 +1 )*yLength ) ){

            	  BSP_LCD_SetFont(&Font20);
            	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            	  BSP_LCD_DrawCircle(i+xLength/2.0, j+yLength/2.0, 20);
            	  BSP_LCD_FillCircle(i+xLength/2, j+yLength/2, 20);


              }
          }
      }
}

int placePiece(int x0, int y0, int player){


	int inBoard=0;
	BSP_LCD_SetFont(&Font20);

	if(player%2==0)
	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	else
	  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);



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

		BSP_LCD_DrawCircle(x0, y0, 20);
		BSP_LCD_FillCircle(x0, y0, 20);
		player++;
	}

	return player;

}
