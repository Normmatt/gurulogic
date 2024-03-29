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
#include "functions.h"

u8 tile[256];
u8 btile[256];
u8 mmap[256];
SDL_Surface* tileset;
SDL_Surface* background;

void ReadMapFile(char *filename)
{
     FILE *map;
     map = fopen(filename,"rb");
     fread(tile, 256, 1, map);
     memcpy(btile,tile,256);
     memcpy(mmap,tile,256);
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

void RotateLeft90()
{
     int i, j;
     u8 *rotTile;
     rotTile = malloc(256*2 + 1);
     
     for (i=0; i<16; i++) {
          for (j=0; j<16; j++) {
               rotTile[i*16+j] = tile[(j*16+(15-i))];
               rotTile[256+(i*16+j)] = btile[(j*16+(15-i))];
          }
     }
     
     memcpy(tile,rotTile,256);
     memcpy(btile,rotTile+256,256);
     free(rotTile);
}

void RotateRight90()
{
     int i, j;
     u8 *rotTile;
     rotTile = malloc(256*2 + 1);
     
     for (i=0; i<16; i++) {
          for (j=0; j<16; j++) {
               rotTile[i*16+j] = tile[((15-j)*16+i)];
               rotTile[256+(i*16+j)] = btile[((15-j)*16+i)];
          }
     }
     
     memcpy(tile,rotTile,256);
     memcpy(btile,rotTile+256,256);
     free(rotTile);
}

void CheckAimTail()
{
     int y=0;
     int x=(((Cannon.Rect.x + 16)-96)/8);
     int j=0;
     int i=0;
     
     for (i=0; i<16; i++) Cannon.AimTailOn[i] = 0;
     Cannon.HeadRect = 0;
     
     //for (y=0; y<16; y++) {
     for (y=16; y--;) {
         if( (tile[(y*16+x)]==0) || (tile[(y*16+x)]==1) )
         {
              Cannon.AimTailOn[j] = 1;
              Cannon.AimRect[j].x = Cannon.Rect.x + 16; 
              Cannon.AimRect[j].y = /*(16*8)-*/((y+1)*8);
              //printf("Cannon.AimRect[%d].x = %d\n",j,Cannon.AimRect[j].x);
              //printf("Cannon.AimRect[%d].y = %d\n",j,Cannon.AimRect[j].y);
              j++;
         } else {
              Cannon.AimTailOn[j] = 2; 
              Cannon.AimRect[j].x = Cannon.Rect.x + 16; 
              Cannon.AimRect[j].y = /*(16*8)-*/((y+1)*8); 
              Cannon.HeadRect = j;  
              //printf("Cannon.AimRect[Cannon.HeadRect].x = %d\n",Cannon.AimRect[Cannon.HeadRect].x);
              //printf("Cannon.AimRect[Cannon.HeadRect].y = %d\n",Cannon.AimRect[Cannon.HeadRect].y);
              break;
         }
     }
     
}

void DrawAimTail()
{
     int i=0;
     
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

int GetBlockCount(int type)
{
     int i=0;
     int temp=0;
     
     for(i=0; i<256; i++)
     {
         if(tile[i]==type) temp++;
     }
     
     return temp;
}

void ShootBlock()
{
     int x=((Cannon.AimRect[Cannon.HeadRect].x)-96)/8;
     int y=(Cannon.AimRect[Cannon.HeadRect].y)/8;
     
     //printf("Cannon.AimRect[Cannon.HeadRect].x = %d\n",Cannon.AimRect[Cannon.HeadRect].x);
     //printf("Cannon.AimRect[Cannon.HeadRect].y = %d\n",Cannon.AimRect[Cannon.HeadRect].y);
     //printf("X=%d \t Y=%d\n",x,y);
     if((y!=16) && (tile[(y*16+x)]==TYPE_BOUNCE) || (tile[((y-1)*16+x)]==TYPE_BOUNCE)) 
         return; //TODO: Add bounce animations
     
     /*if((y!=16) && (tile[(y*16+x)]==TYPE_REDROLL))
     {
         if(tile[((y-2)*16+x)]==TYPE_WHITE)
         {
             tile[((y-2)*16+x)] = TYPE_BLUEROLL;
         } else {
             tile[((y-2)*16+x)] = TYPE_REDROLL; 
         }
               
         tile[(y*16+x)] =  btile[(y*16+x)] = TYPE_BLANK; 
         tile[((y+1)*16+x)] = TYPE_RED; 
         return;       
     }*/
     if((y!=16) && (tile[((y-1)*16+x)]==TYPE_REDROLL) && (tile[((y-2)*16+x)]!=TYPE_WALL))
     {
         if(tile[((y-2)*16+x)]>=TYPE_WHITE)
         {
             tile[((y-2)*16+x)] = TYPE_BLUEROLL;
         } else {
             tile[((y-2)*16+x)] = TYPE_REDROLL; 
         }
               
         tile[((y-1)*16+x)] =  btile[((y-1)*16+x)] = TYPE_BLANK; 
         
         if(tile[(y*16+x)]!=TYPE_WHITE) 
         {
               tile[(y*16+x)]=TYPE_RED; //Set it to red block 
         } else {
               tile[(y*16+x)]=TYPE_BLUE; //Set it to blue block 
         } 
         Vars.CurrentBlocks--;
         return;   
     }
     
     if((y!=16) && (tile[((y-1)*16+x)]==TYPE_BLUEROLL)  && (tile[((y-2)*16+x)]==TYPE_WALL))
     {
         if(tile[((y+1)*16+x)]!=TYPE_WHITE) 
         {
               tile[((y+1)*16+x)]=TYPE_RED; //Set it to red block 
         } else {
               tile[((y+1)*16+x)]=TYPE_BLUE; //Set it to blue block 
         }        
     }
     
     if((y!=16) && (tile[((y-1)*16+x)]==TYPE_BLUEROLL) && (tile[((y-2)*16+x)]!=TYPE_WALL))
     {
         if(tile[((y-2)*16+x)]>=TYPE_WHITE)
         {
             tile[((y-2)*16+x)] = TYPE_BLUEROLL;
         } else {
             tile[((y-2)*16+x)] = TYPE_REDROLL; 
         }
               
         if(btile[((y-1)*16+x)]==TYPE_WHITE) 
          tile[((y-1)*16+x)] = TYPE_WHITE; 
         else
          tile[((y-1)*16+x)] = TYPE_BLANK; 
          
         if(btile[((y-1)*16+x)]==TYPE_WHITE) 
          btile[((y-1)*16+x)] = TYPE_WHITE; 
         else
          btile[((y-1)*16+x)] = TYPE_BLANK;
         
         if(tile[(y*16+x)]!=TYPE_WHITE) 
         {
               tile[(y*16+x)]=TYPE_RED; //Set it to red block 
         } else {
               tile[(y*16+x)]=TYPE_BLUE; //Set it to blue block 
         } 
         Vars.CurrentBlocks--;
         return;       
     }

     /*if((y!=16) && (tile[(y*16+x)]!=TYPE_BLANK) && (Vars.CurrentBlocks!=0))//Shoot
     {
         if(tile[(y*16+x)]==TYPE_WHITE) 
         {
             if(btile[(y*16+x)]!=TYPE_WHITE) 
               tile[(y*16+x)]=TYPE_RED; //Set it to red block 
             else
               tile[(y*16+x)]=TYPE_BLUE; //Set it to blue block  
             Vars.CurrentBlocks--;
         }                   
     } else {*/
           /* Commented out because it it broke things alot, left here just in case
           /*if( (Vars.CurrentBlocks!=0) && (tile[(y*16+(x+1))]!=0) )
           {
               if(btile[(y*16+x)]!=1) 
                tile[(y*16+x)]=TYPE_RED; //Set it to red block 
               else
                tile[(y*16+x)]=TYPE_BLUE; //Set it to blue block 
               Vars.CurrentBlocks--;
           } 
           if( (Vars.CurrentBlocks!=0) && (tile[(y*16+(x-1))]!=0) )
           {
               if(btile[(y*16+x)]!=1) 
                tile[(y*16+x)]=TYPE_RED; //Set it to red block 
               else
                tile[(y*16+x)]=TYPE_BLUE; //Set it to blue block  
               Vars.CurrentBlocks--;
           } */
           /* Commented out because it broke Stage 10 of Rescue 1
           /*if( (y!=16) && (Vars.CurrentBlocks!=0) && (tile[((y+1)*16+x)]!=0) )
           {
               if(btile[(y*16+x)]!=1) 
                tile[(y*16+x)]=TYPE_RED; //Set it to red block 
               else
                tile[(y*16+x)]=TYPE_BLUE; //Set it to blue block 
               Vars.CurrentBlocks--;
           }*/ 
           if( (y!=16) && (Vars.CurrentBlocks!=0) && (tile[((y-1)*16+x)]!=TYPE_BLANK) )
           {
               if(btile[(y*16+x)]!=TYPE_WHITE) 
                tile[(y*16+x)]=TYPE_RED; //Set it to red block 
               else
                tile[(y*16+x)]=TYPE_BLUE; //Set it to blue block  
               Vars.CurrentBlocks--;
           } 
     //}
}

void RetractBlock()
{
     int x=((Cannon.AimRect[Cannon.HeadRect].x)-96)/8;
     int y=(Cannon.AimRect[Cannon.HeadRect].y)/8;
     
     y--;
     
     //printf("Cannon.AimRect[Cannon.HeadRect].x = %d\n",Cannon.AimRect[Cannon.HeadRect].x);
     //printf("Cannon.AimRect[Cannon.HeadRect].y = %d\n",Cannon.AimRect[Cannon.HeadRect].y);
     //printf("X=%d \t Y=%d\n",x,y);

     if( (tile[(y*16+x)]==TYPE_BLUE) || (tile[(y*16+x)]==TYPE_RED) ) //Retract
     {
         if(btile[(y*16+x)]==TYPE_RED)
          tile[(y*16+x)]=TYPE_BLANK; //Set it to blank block
         else
          tile[(y*16+x)]=btile[(y*16+x)]; //Set it to original block          
         Vars.CurrentBlocks++;                   
     }
}

void DrawMinimap()
{
     int i=0,j=0;
          
     for(i=0; i<16; i++)
     {
         for(j=0; j<16; j++)
         {
             if(mmap[(i*16+j)]!=0)
             {
                 drawTile(Vars.Minimap, screen, 0, 0, 8+(4*j), 16+(4*i), 4, 4);                     
             }
         }
     }
     
}

void DrawLevel()
{
     int row,tile_no=0;
     
     SDL_BlitSurface(background, 0, screen, 0);
     
     DrawMinimap();
     
     //for(row=0; row<16; row++)
     for(row=16; row--;)
     {
         for(tile_no=16*row; tile_no<16*(row+1); tile_no++)
         {
             drawTile(tileset, screen, 8*tile[tile_no], 0, 96+(8*(tile_no-(16*row))), 8+(8*row), 8, 8);
         } 
     }
     
}
