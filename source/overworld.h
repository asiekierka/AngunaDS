#ifndef _OVERWORLD_H_
#include "level.h"

bool isOutdoorWallCollision(LevelData * currentLevel, int x, int y, int mobility);
u16 applyOverworldPalette(LevelData * level, int tile);

#endif
