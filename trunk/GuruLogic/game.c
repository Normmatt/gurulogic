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

gCannon Cannon;

void GameMain()
{
    int i = 0;
    int done = 0;
    
    InitLevel("test.map","blocks.png");
    
    Cannon.Surface = SDL_DisplayFormat(IMG_Load("sprites/champ_gun.png"));
    if(SDL_SetColorKey(Cannon.Surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Cannon.Surface->format, 255, 0, 255)) == -1)
    fprintf(stderr, "Warning: colorkey will not be used, reason: %s\n", SDL_GetError());
    
    Cannon.Rect.x = 80;
    Cannon.Rect.y = 136;
    Cannon.Rect.w = 39;
    Cannon.Rect.h = 24;
    
    Cannon.AimHeadSurface = SDL_DisplayFormat(IMG_Load("sprites/aim_head.png"));
    if(SDL_SetColorKey(Cannon.AimHeadSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Cannon.AimHeadSurface->format, 255, 0, 255)) == -1)
    fprintf(stderr, "Warning: colorkey will not be used, reason: %s\n", SDL_GetError());
    
    Cannon.AimTailSurface = SDL_DisplayFormat(IMG_Load("sprites/aim_tail.png"));
    if(SDL_SetColorKey(Cannon.AimTailSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Cannon.AimTailSurface->format, 255, 0, 255)) == -1)
    fprintf(stderr, "Warning: colorkey will not be used, reason: %s\n", SDL_GetError());

    for(i=0; i<16; i++)
    {
    	Cannon.AimRect[i].x = 96;
    	Cannon.AimRect[i].y = 128;
    	Cannon.AimRect[i].w = 8;
    	Cannon.AimRect[i].h = 8;
    }
    
    done = 0;
	while (!done)
	{
		SDL_Event event;

		/* Check for events */
		while (SDL_PollEvent (&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
                    /* if press Ctrl + C, terminate program */
					if ( (event.key.keysym.sym == SDLK_c) && (event.key.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL)) )
						done = 1;
						
					switch(event.key.keysym.sym) 
                    { 
                         case SDLK_z:
                          Board.rotated_left ^= 1;
                          RotateLeft90();    
                          break;
                         case SDLK_x:
                          Board.rotated_right ^= 1;
                          RotateRight90();    
                          break;
                         case SDLK_LEFT:  
                          Cannon.Rect.x -= 8;
                          if(Cannon.Rect.x < 80) Cannon.Rect.x = 80;              
                          break;
                         case SDLK_RIGHT: 
                          Cannon.Rect.x += 8;  
                          if(Cannon.Rect.x > 200) Cannon.Rect.x = 200; 
                          break;
                         case SDLK_DOWN:
                          break;
                         case SDLK_UP:
                          break;
                         default:
                          break;
                    }	
                    break;
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}
		
		/* Clear Screen */
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255,0,255));
		
		/* Processing */
		DrawLevel();
		SDL_BlitSurface(Cannon.Surface, 0, screen, &Cannon.Rect);
		DrawAimTail();
		SDL_Flip(screen);
	}
}
