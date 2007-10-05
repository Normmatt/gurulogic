#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include "game.h"
#include "types.h"
#include "level.h"
#include "defines.h"

u8 tile[256];
u8 rotTile[256];
SDL_Surface* tileset;
SDL_Surface* background;
gBoard Board;

void ReadMapFile(char *filename)
{
     FILE *map;
     map = fopen(filename,"rb");
     fread(tile, 256, 1, map);
     fclose(map);
}

void WriteMapFile(char *filename)
{
     FILE *map;
     map = fopen(filename,"wb");
     fwrite(tile, 256, 1, map);
     fclose(map);
}

void LoadBG(char *filename)
{
     background = SDL_DisplayFormat(IMG_Load(filename));
     if(SDL_SetColorKey(background, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(background->format, 255, 0, 255)) == -1)
     fprintf(stderr, "Warning: colorkey will not be used, reason: %s\n", SDL_GetError());
}

void LoadTileset(char *filename)
{
     tileset = SDL_DisplayFormat(IMG_Load(filename));
}

void FreeTileset()
{
     SDL_FreeSurface(tileset);
}

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

void InitLevel(char* map_name, char* tileset_name)
{
     char temp[256];
     
     sprintf(temp, "background/bg.png");
     LoadBG(temp);
     
     sprintf(temp, "level/%s", map_name);
     ReadMapFile(temp);
     
     sprintf(temp, "tileset/%s", tileset_name);
     LoadTileset(temp);
}

void RotateRight90()
{
     int i, j;
     memset(rotTile,0,256);
     
     if(Board.rotated_right)
     {
          for (i=0; i<16; i++) {
               for (j=0; j<16; j++) {
                    rotTile[i*16+j] = tile[j*16+i];
               }
          }
     }
     else
     {
          for (i=0; i<16; i++) {
               for (j=0; j<16; j++) {
                    rotTile[i*16+j] = tile[(16*16)-1-(j*16+i)];
               }
          }
     }
     
     memset(tile,0,256);
     memcpy(tile,rotTile,256);
}

void RotateLeft90()
{
     int i, j;
     memset(rotTile,0,256);
     
     if(Board.rotated_left)
     {
          for (i=0; i<16; i++) {
               for (j=0; j<16; j++) {
                    rotTile[i*16+j] = tile[(16*16)-1-(j*16+i)];
               }
          }
     }
     else
     {
          for (i=0; i<16; i++) {
               for (j=0; j<16; j++) {
                    rotTile[i*16+j] = tile[j*16+i];
               }
          }
     }
     
     memset(tile,0,256);
     memcpy(tile,rotTile,256);
}

void CheckAimTail()
{
     int y=0;
     int x=((Cannon.Rect.x + 16)-96)/8;
     int j=0;
     
     //for (y=0; y<16; y++) {
     for (y=16; y!=0; y--) {
         if( (tile[(y*16+x)]==0) || (tile[(y*16+x)]==1) )
         {
              Cannon.AimTailOn[j] = 1;
              Cannon.AimRect[j].x = Cannon.Rect.x + 16; 
              Cannon.AimRect[j].y = /*(16*8)-*/((y)*8);
              j++;
         } else {
              Cannon.AimTailOn[j-1] = 2;  
              Cannon.HeadRect = j-1;
              break;
         }
     }
     
}

void DrawAimTail()
{
     int i=0;
     
     for (i=0; i<16; i++) Cannon.AimTailOn[i] = 0;
     
     CheckAimTail();
     
     i=0;
     while(Cannon.AimTailOn[i]!=0)
     {
         if(Cannon.AimTailOn[i+1]==2)
         {
          SDL_BlitSurface(Cannon.AimHeadSurface, 0, screen, &Cannon.AimRect[i]);
          break;
         } else {
          SDL_BlitSurface(Cannon.AimTailSurface, 0, screen, &Cannon.AimRect[i]);
         }
         i++;
     }
}

void DrawLevel()
{
     int row,tile_no=0;
     
     SDL_BlitSurface(background, 0, screen, 0);
     
     //for(row=0; row<16; row++)
     for(row=16; row--;)
     {
         for(tile_no=16*row; tile_no<16*(row+1); tile_no++)
         {
             drawTile(tileset, screen, 8*tile[tile_no], 0, 96+(8*(tile_no-(16*row))), 8+(8*row), 8, 8);
         } 
     }
     
}
