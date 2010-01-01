#ifndef GAME_H
#define GAME_H

void GameMain(char *level);

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

typedef struct
{
//   unsigned char Up;
//   unsigned char Down;
   unsigned char Left;
   unsigned char Right;
   unsigned char RotateLeft;
   unsigned char RotateRight;
   unsigned char Shoot;
   unsigned char Retract;
}
gKeys;

extern gKeys Keys;

typedef struct
{
   unsigned char mSeconds;
   unsigned char Seconds;  
   unsigned char Minutes;
   unsigned char Hours;
}
gTime; 

extern gTime gmTime;

typedef struct
{
   unsigned char Stage;
   unsigned char MaxBlocks;
   unsigned char CurrentBlocks;
   SDL_Surface* nfont;  
   SDL_Surface* font; 
   SDL_Surface* Minimap; 
   //gTime gmTime;  
}
gVars;       

extern gVars Vars;

#endif
