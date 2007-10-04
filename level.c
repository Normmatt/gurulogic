#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include "types.h"
#include "level.h"
#include "defines.h"

u8 tile[256];
SDL_Surface* tileset;
SDL_Surface* background;

void ReadMapFile(char *filename)
{
     FILE *map;
     map = fopen(filename,"rb");
     fread(tile, 256, 1, map);
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

#define draw_row(row) \
   for(tile_no=16*row; tile_no<16*(row+1); tile_no++){ \
   drawTile(tileset, screen, 8*tile[tile_no], 0, 96+(8*(tile_no-(16*row))), 8+(8*row), 8, 8); \
   } 

void DrawLevel(char* map_name, char* tileset_name)
{
     int i,tile_no=0;
     char temp[256];
     
     sprintf(temp, "background/bg.png");
     LoadBG(temp);
     
     sprintf(temp, "level/%s", map_name);
     ReadMapFile(temp);
     
     sprintf(temp, "tileset/%s", tileset_name);
     LoadTileset(temp);
     
     SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255,0,255));
     
     SDL_BlitSurface(background, 0, screen, 0);
     
     for(i=0; i<16; i++) { draw_row(i) }
     
}
