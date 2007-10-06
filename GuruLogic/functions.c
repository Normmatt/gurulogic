#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include "game.h"
#include "level.h"
#include "types.h"
#include "defines.h"
#include "functions.h"

void drawTile(SDL_Surface* srcSurface, SDL_Surface* screenSurface, int srcX, int srcY, int dstX, int dstY, int width, int height)
{
	SDL_Rect srcRect;
	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = width;
	srcRect.h = height;

	SDL_Rect dstRect;
	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = width;
	dstRect.h = height;

	SDL_BlitSurface(srcSurface, &srcRect, screenSurface, &dstRect);
}

void drawText8x8(SDL_Surface* font, SDL_Surface* screen, char* a, int x, int y)
{
     int j,width,height,srcX,DestX,DestY;
     
     j=0;
     DestX=x;
     DestY=y;
     
     width=8;
     height=8;
          
     while(j!=strlen(a))
     {
        switch(a[j])
        {
        case '0': srcX = 8*0; break;
        case '1': srcX = 8*1; break;
        case '2': srcX = 8*2; break;
        case '3': srcX = 8*3; break;
        case '4': srcX = 8*4; break;
        case '5': srcX = 8*5; break;
        case '6': srcX = 8*6; break;
        case '7': srcX = 8*7; break;
        case '8': srcX = 8*8; break;
        case '9': srcX = 8*9; break;
        case '-': srcX = 8*10; break;
        case '~': { srcX = 8*11; width=16; } break;
        case '\'': srcX = 8*13; break;
        case '"': srcX = 8*14; break;
        
        default: break;
        }
        
        //srcX = 256 + 8*(a[j]-32);
        
        if(a[j]=='\n') 
        { 
          j++;
          DestY+=height + 1;
          DestX=x + 1;
        } else {
          j++;
          drawTile(font, screen, srcX, 0, DestX, DestY, width, height);
          DestX += width;
        }
     } 
}
