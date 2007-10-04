#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

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

	/* Check and open joystick device */
	if (SDL_NumJoysticks() > 0) {
		joy = SDL_JoystickOpen(0);
		if(!joy) {
			fprintf (stderr, "Couldn't open joystick 0: %s\n", SDL_GetError ());
		}
	}

#ifdef WIN32
	/* Only use Windows code here */
#endif

    DrawLevel("test.map","blocks.png");

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
					break;
				case SDL_KEYUP:
					break;
				case SDL_JOYBUTTONDOWN:
					/* if press Start button, terminate program */
					break;
				case SDL_JOYBUTTONUP:
					break;
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}

		/* Processing */
	}

	return 0;
}
