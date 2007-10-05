#ifndef GAME_H
#define GAME_H

void GameMain();

typedef struct
{
   SDL_Rect Rect;
   SDL_Surface* Surface;
   SDL_Surface* AimHeadSurface;
   SDL_Rect AimRect[16];
   SDL_Surface* AimTailSurface;
   unsigned char AimTailOn[16];
   unsigned char HeadRect;
   
}
gCannon;

extern gCannon Cannon;

#endif
