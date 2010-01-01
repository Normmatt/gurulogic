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

/* The screen surface, joystick device */
SDL_Surface *screen = NULL;
SDL_Joystick *joy = NULL;

void Terminate(void)
{
	SDL_Quit();
}

int main (int argc, char *argv[])
{
	int done;

	/* Initialize SDL */
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER) < 0) {
		fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError ());
		exit (1);
	}
	atexit (Terminate);

	SDL_ShowCursor(SDL_DISABLE);

	/* Set 240x160 16-bits video mode */
	screen = SDL_SetVideoMode (240, 160, 16, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf (stderr, "Couldn't set 240x160x16 video mode: %s\n", SDL_GetError ());
		exit (2);
	}
	
	SDL_WM_SetCaption ("GuruLogic SDL v0.1.1", NULL);

#ifdef WIN32
	/* Only use Windows code here */
#endif

	GameMain("level-1.map");
	/*GameMain("level-2.map");
	GameMain("level-3.map");
	GameMain("level-4.map");
	GameMain("level-5.map");
	GameMain("level-6.map");
	GameMain("level-7.map");
	GameMain("level-8.map");
	GameMain("level-9.map");
	GameMain("level-10.map");*/

	return 0;
}
