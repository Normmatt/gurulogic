#ifndef LEVEL_H
#define LEVEL_H

void ReadMapFile(char *filename);
void LoadTileset(char *filename);
void FreeTileset();
void InitLevel(char* map_name, char* tileset_name);
void DrawLevel();

void RotateRight90();
void RotateLeft90();

void CheckAimTail();
void DrawAimTail();

typedef struct
{
   unsigned char rotated_left;
   unsigned char rotated_right;
}
gBoard;

extern gBoard Board;

#endif
