#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void drawTile(SDL_Surface* srcSurface, SDL_Surface* screenSurface, int srcX, int srcY, int dstX, int dstY, int width, int height);
void drawText8x8(SDL_Surface* font, SDL_Surface* screen, char* a, int x, int y);

#endif
