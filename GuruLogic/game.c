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

gKeys    Keys;
gVars    Vars;
gCannon  Cannon;

void GameMain()
{
    int i = 0;
    int done = 0;
    char temp[256];
    
    InitLevel("test.map","blocks.png");
    Vars.Stage = 1;
    Vars.MaxBlocks = GetBlockCount();
    Vars.CurrentBlocks = Vars.MaxBlocks;
    
    Vars.nfont = SDL_DisplayFormat(IMG_Load("sprites/numbers.png"));
    if(SDL_SetColorKey(Vars.nfont, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Vars.nfont->format, 255, 0, 255)) == -1)
    fprintf(stderr, "Warning: colorkey will not be used, reason: %s\n", SDL_GetError());

    
    Cannon.Surface = SDL_DisplayFormat(IMG_Load("sprites/champ_gun.png"));
    if(SDL_SetColorKey(Cannon.Surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Cannon.Surface->format, 255, 0, 255)) == -1)
    fprintf(stderr, "Warning: colorkey will not be used, reason: %s\n", SDL_GetError());
    
    Cannon.Rect.x = 136;//80;
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
    	Cannon.AimRect[i].x = 152;//96;
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
                          	Keys.RotateLeft = 1;     
                          	break;
                         case SDLK_x: 
                          	Keys.RotateRight = 1; 
                          	break;
                         case SDLK_LEFT: 
                          	Keys.Left = 1;              
                          	break;
                         case SDLK_RIGHT:
                          	Keys.Right = 1;  
                          	break;
                         case SDLK_DOWN:
                          	Keys.Retract = 1;
                          	//fprintf(stdout,"Blocks = %02d/%02d\n",Vars.CurrentBlocks,Vars.MaxBlocks);
                          	break;
                         case SDLK_UP:
                          	Keys.Shoot = 1;
                          	//fprintf(stdout,"Blocks = %02d/%02d\n",Vars.CurrentBlocks,Vars.MaxBlocks);
                          	break;
                         default:
                          	break;
                    }	
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym) 
                    { 
                         case SDLK_z: 
                          	Keys.RotateLeft = 0;     
                          	break;
                         case SDLK_x: 
                          	Keys.RotateRight = 0; 
                          	break;
                         case SDLK_LEFT: 
                          	Keys.Left = 0;              
                          	break;
                         case SDLK_RIGHT:
                          	Keys.Right = 0;  
                          	break;
                         case SDLK_DOWN:
                          	Keys.Retract = 0;
                          	break;
                         case SDLK_UP:
                          	Keys.Shoot = 0;
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
		
		/* Handle Key Input */
		if(Keys.RotateLeft)
        {
			Board.rotated_left ^= 1;
        	RotateLeft90(); 
        	SDL_Delay(150); //Delay because its too quick
        }
        
        if(Keys.RotateRight)
        {
			Board.rotated_right ^= 1;
        	RotateRight90();
            SDL_Delay(150); //Delay because its too quick
        }
        
        if(Keys.Left)
        {
			Cannon.Rect.x -= 8;
            if(Cannon.Rect.x < 80) Cannon.Rect.x = 80; 
            SDL_Delay(150); //Delay because its too quick
        }
        
        if(Keys.Right)
        {
			Cannon.Rect.x += 8;
            if(Cannon.Rect.x > 200) Cannon.Rect.x = 200; 
            SDL_Delay(150); //Delay because its too quick
        }
		
		if(Keys.Shoot)
        {
            //TODO
            ShootBlock();
            SDL_Delay(150); //Delay because its too quick
        }
        
        if(Keys.Retract)
        {
            //TODO
            RetractBlock();
            SDL_Delay(150); //Delay because its too quick
        }
        
		/* Clear Screen */
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255,0,255));
		
		/* Get current number of blocks */
		//Vars.CurrentBlocks = GetBlockCount();
		
		/* Draw Level */
		DrawLevel();
		
		/* Draw Cannon on the screen */
		SDL_BlitSurface(Cannon.Surface, 0, screen, &Cannon.Rect);
		
		/* Draw the Aim onto the screen */
		DrawAimTail();
		
		/* Draw stage number to screen*/
		sprintf(temp,"%02d",Vars.Stage);
		drawText8x8(Vars.nfont,screen,temp,56,8);
		
		/* Draw timer to screen*/
		sprintf(temp,"%02d'%02d\"%02d",0,0,0);
		drawText8x8(Vars.nfont,screen,temp,8,96);
		
		/* Draw best time to screen*/
		sprintf(temp,"--'--\"--");
		drawText8x8(Vars.nfont,screen,temp,8,120);
		
		/* Draw current number of blocks to screen*/
		sprintf(temp,"%03d",Vars.CurrentBlocks);
		drawText8x8(Vars.nfont,screen,temp,8,144);
		
		/* Draw max number of blocks to screen */
		sprintf(temp,"%03d",Vars.MaxBlocks);
		drawText8x8(Vars.nfont,screen,temp,48,144);
		
		/* Flip the surface to redraw the screen */
		SDL_Flip(screen);
		
		/* Check if won */
		if(GetBlockCount()==0) //You win
		{
           /* Clear Screen */
		   //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255,0,255)); 
           
           /* Print You win! */                   
           //drawText(Vars.nfont,screen,"You Win!",48,144); 
           
           /* Flip the surface to redraw the screen */
		   //SDL_Flip(screen);
		
           printf("You Win!\n");  
           
           /* Delay because its too quick */
           SDL_Delay(1000*2);
           
           /* Break out of loop */
           done = 1;                 
        }
	}
	
	SDL_FreeSurface(Cannon.Surface);
	SDL_FreeSurface(Cannon.AimHeadSurface);
	SDL_FreeSurface(Cannon.AimTailSurface);
	SDL_FreeSurface(Vars.nfont);
	SDL_FreeSurface(Vars.font);
	SDL_FreeSurface(screen);
}
