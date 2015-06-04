#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_
#include "misc.h"

#define CURRENT_SAVE_SLOT -1

#define GAMESTATE_ZONE_SAVE_START 200

void initGameState();
void initSaveSystem();
void setGameState(int item, int state);
int getGameState(int item);
void setToggleState(int item, int state);
int getToggleState(int item);
void toggleToggleState(int item);

bool saveGame(int saveSlot);
bool loadGame(int saveSlot);

int getCurrentSaveSlot();
void updateSaveToRespawn();

struct LevelDataStruct;

void setLastGoodRoom(int x, int y, struct LevelDataStruct * room);
bool verifySaveGame(int saveSlot);
void clearZoneBasedGameState();

#endif
