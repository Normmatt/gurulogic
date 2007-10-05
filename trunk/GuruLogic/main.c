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
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
		fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError ());
		exit (1);
	}
	atexit (Terminate);

	SDL_ShowCursor(SDL_DISABLE);

	/* Set 240x160 16-bits video mode */
	screen = SDL_SetVideoMode (240, 160, 16, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf (stderr, "Couldn't set 320x240x16 video mode: %s\n", SDL_GetError ());
		exit (2);
	}

#ifdef WIN32
	/* Only use Windows code here */
#endif

    /*InitLevel("test.map","blocks.png");
    RotateRight90();
    RotateRight90();
	DrawLevel();*/

	GameMain();

	return 0;
}
