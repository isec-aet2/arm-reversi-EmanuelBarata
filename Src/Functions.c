/*
 * Functions.c
 *
 *  Created on: May 27, 2019
 *      Author: Utilizador
 */

# include "Functions.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery.h"


void printBoard(int x0, int y0, int dimension, int xLength, int yLength){
      for(int i=x0; i<=dimension*xLength; i+=xLength){
          for(int j=y0; j<=dimension*yLength; j+=yLength){
        	  BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
              BSP_LCD_DrawRect(i, j, xLength, yLength);
              if ( ( i==(dimension/2)*xLength && j == (dimension/2)*yLength ) || (  i==(dimension/2 +1 )*xLength && j == (dimension/2 +1 )*yLength ) ){

            	  BSP_LCD_SetFont(&Font20);
            	  BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
            	  BSP_LCD_DrawCircle(i+xLength/2.0, j+yLength/2.0, 20);
            	  BSP_LCD_FillCircle(i+xLength/2, j+yLength/2, 20);
              }
              else if( ( i==(dimension/2+1)*xLength && j == (dimension/2)*yLength ) || (  i==(dimension/2)*xLength && j == (dimension/2 +1 )*yLength ) ){

            	  BSP_LCD_SetFont(&Font20);
            	  BSP_LCD_SetTextColor(LCD_COLOR_LIGHTMAGENTA);
            	  BSP_LCD_DrawCircle(i+xLength/2.0, j+yLength/2.0, 20);
            	  BSP_LCD_FillCircle(i+xLength/2, j+yLength/2, 20);


              }
          }
      }
}
