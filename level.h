#ifndef LEVEL_H
#define LEVEL_H

void ReadMapFile(char *filename);
void LoadTileset(char *filename);
void FreeTileset();
void DrawLevel(char* map_name, char* tileset_name);

#endif
