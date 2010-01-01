#ifndef LEVEL_H
#define LEVEL_H

#define TYPE_BLANK      0
#define TYPE_WHITE      1
#define TYPE_WALL       2
#define TYPE_BLUE       3
#define TYPE_BLUEROLL   4
#define TYPE_BOUNCE     5
#define TYPE_HOLE       6
#define TYPE_RED        7
#define TYPE_REDROLL    8

void ReadMapFile(char *filename);
void LoadTileset(char *filename);
void FreeTileset();
void InitLevel(char* map_name, char* tileset_name);

void DrawLevel();
void DrawMinimap();

void RotateRight90();
void RotateLeft90();

void CheckAimTail();
void DrawAimTail();

int GetBlockCount();
void ShootBlock();
void RetractBlock();

#endif
