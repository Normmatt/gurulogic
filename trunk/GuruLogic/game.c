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

gKeys       Keys;
gVars       Vars;
gCannon     Cannon;
gTime       gmTime;

void GameMain(char *level)
{
    int i = 0;
    int done = 0;
    char temp[256];
    
    InitLevel(level,"blocks.png");
    Vars.Stage = 1;
    Vars.MaxBlocks = GetBlockCount(TYPE_WHITE) - GetBlockCount(TYPE_BLUEROLL)  - GetBlockCount(TYPE_REDROLL);
    Vars.CurrentBlocks = GetBlockCount(TYPE_RED);
    if(Vars.CurrentBlocks==0)
    {
      Vars.CurrentBlocks = Vars.MaxBlocks;
    } else {
      Vars.CurrentBlocks = GetBlockCount(TYPE_WHITE) - GetBlockCount(TYPE_RED);   
    }
    
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

    Vars.Minimap = SDL_DisplayFormat(IMG_Load("sprites/minimap_on.png"));
    if(SDL_SetColorKey(Vars.Minimap, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Vars.Minimap->format, 255, 0, 255)) == -1)
    fprintf(stderr, "Warning: colorkey will not be used, reason: %s\n", SDL_GetError());
 
    /* Reset all 16 AimRects to default values */
    for(i=0; i<16; i++)
    {
    	Cannon.AimRect[i].x = 152;//96;
    	Cannon.AimRect[i].y = 128;
    	Cannon.AimRect[i].w = 8;
    	Cannon.AimRect[i].h = 8;
    }
    
    /* Reset timer values to default */
    memset(&gmTime,0,sizeof(gTime));
    
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
					done = 2;
					break;
				default:
					break;
			}
		}
		#define KEY_WAIT 4
		
		/* Handle Key Input */
		if(Keys.RotateLeft)
        {
        	if(Keys.RotateLeft==KEY_WAIT) 
            {
			    RotateLeft90(); 
        	    Keys.RotateLeft=0;
            }
            Keys.RotateLeft++;
        }
        
        if(Keys.RotateRight)
        {
        	if(Keys.RotateRight==KEY_WAIT)
            {
			    RotateRight90();
        	    Keys.RotateRight=0;
            }
            Keys.RotateRight++;
        }
        
        if(Keys.Left)
        {
            if(Keys.Left==KEY_WAIT)
            {
			    Cannon.Rect.x -= 8;
                if(Cannon.Rect.x < 80) Cannon.Rect.x = 80; 
                Keys.Left=0;
            }
            Keys.Left++;
        }
        
        if(Keys.Right)
        {
            if(Keys.Right==KEY_WAIT)
            {
			    Cannon.Rect.x += 8;
			    if(Cannon.Rect.x > 200) Cannon.Rect.x = 200; 
			    Keys.Right=0;
            }
            Keys.Right++;
        }
		
		if(Keys.Shoot)
        {
            if(Keys.Shoot==KEY_WAIT)
            {
			    ShootBlock();
        	    Keys.Shoot=0;
            }
            Keys.Shoot++;
        }
        
        if(Keys.Retract)
        {
            if(Keys.Retract==KEY_WAIT)
            {
			    RetractBlock();
        	    Keys.Retract=0;
            }
            Keys.Retract++;
        }
        
		/* Clear Screen */
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255,0,255));
		
		/* Draw Level */
		DrawLevel();
		
		/* Draw Cannon on the screen */
		SDL_BlitSurface(Cannon.Surface, 0, screen, &Cannon.Rect);
		
		/* Set up the Aim */
		CheckAimTail();
		
		/* Draw the Aim onto the screen */
		DrawAimTail();
		
		/* Update Timer */
		if(gmTime.mSeconds==60) //Check if mSeconds timer equals one second
        { 
            gmTime.Seconds++;  //Update Seconds timer
            gmTime.mSeconds=0; //Reset mSeconds timer
        } else {
            gmTime.mSeconds++; //Update mSeconds timer
            SDL_Delay(17);     //Delay it for approx 1ms
        }
		if(gmTime.Seconds==60) //Check if Seconds timer equals one Minute
        { 
            gmTime.Minutes++;  //Update Minutes timer
            gmTime.Seconds=0;  //Reset Seconds timer    
        }
		if(gmTime.Minutes==60) //Check if Minutes timer equals one Hour
		{ 
            gmTime.Hours++;    //Update Hours timer
            gmTime.Minutes=0;  //Reset Minutes timer 
            
            //TODO: Add Times up screen    
        }
		
		/* Draw stage number to screen*/
		sprintf(temp,"%02d",Vars.Stage);
		drawText8x8(Vars.nfont,screen,temp,56,8);
		
		/* Draw timer to screen*/
		sprintf(temp,"%02d'%02d\"%02d",gmTime.Minutes,gmTime.Seconds,gmTime.mSeconds);
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
		if((GetBlockCount(TYPE_WHITE)==0) && (Vars.CurrentBlocks==0)) //You win
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
	
	if(done == 2) exit(1);
}

