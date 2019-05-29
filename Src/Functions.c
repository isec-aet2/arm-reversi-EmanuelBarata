/*
 * Functions.c
 *
 *  Created on: May 27, 2019
 *      Author: Utilizador
 */

#include "Functions.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery.h"


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


void possiblePlace(int Xpos, int Ypos, int player){

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
								return;
							}
						}

				}

			}

		}
	}
}

void findPossiblePlaces(int player){

    for(int i=boardX0; i<=DIMENSION*boardPlaceWidth; i+=boardPlaceWidth){
        for(int j=boardY0 ; j<=DIMENSION*boardPlaceHeight; j+=boardPlaceHeight){

        	possiblePlace(i, j, player);

        }
    }

}


void refreshBoard(){

	for(int i=boardX0; i<boardX0+DIMENSION*boardPlaceWidth; i+=boardPlaceWidth){

		for(int j=boardY0; j<boardY0+DIMENSION*boardPlaceHeight; j+=boardPlaceHeight){

		  	  BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
		      BSP_LCD_DrawRect(i, j, boardPlaceWidth, boardPlaceHeight);
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

					if(Xpos+dx*i*boardPlaceWidth >=  boardX0  && Ypos+dy*i*boardPlaceHeight >= boardY0  && Xpos+dx*i*boardPlaceWidth <= boardX0+DIMENSION*boardPlaceWidth && Ypos+dy*i*boardPlaceHeight <= boardY0+DIMENSION*boardPlaceHeight ){


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

void transColor(int Xpos, int Ypos, int player){

	int advPieces;

	uint32_t ColorArray[]={LCD_COLOR_BLACK,LCD_COLOR_WHITE};

	for(int dx = -1; dx < 2; dx++){
		for(int dy = -1; dy < 2; dy++){

			advPieces=0;

			if(!(dx==0 && dy==0)){

				for(int i=1; i<DIMENSION;i++){

					if(Xpos+dx*i*boardPlaceWidth >=  boardX0  && Ypos+dy*i*boardPlaceHeight >= boardY0  && Xpos+dx*i*boardPlaceWidth <= boardX0+DIMENSION*boardPlaceWidth && Ypos+dy*i*boardPlaceHeight <= boardY0+DIMENSION*boardPlaceHeight ){


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

	//return FALSE;

}


