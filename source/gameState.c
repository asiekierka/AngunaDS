#include <stdint.h>
#include <fat.h>
#include <stdio.h>

#include "gameState.h"
#include "level.h"
#include "misc.h"
#include "item.h"
#include "items.h"
#include "mainChar.h"
#include "hud.h"
#include "enemies.h"
#include "enemyDatabase.h"
#include "minimap.h"


#define GAME_STATE_SLOTS 256
#define TOGGLE_SLOTS 32
#define ITEMS_TO_FIND 36

IN_EWRAM int gameState[GAME_STATE_SLOTS];
IN_EWRAM int toggleState[TOGGLE_SLOTS];

int lastGoodRoomX;
int lastGoodRoomY;
LevelData * lastGoodRoom;

int currentSaveGameSlot;

typedef struct
{
	//positional info
	int roomX;
	int roomY;
	LevelData * room;

	int lastGoodRoomX;
	int lastGoodRoomY;
	LevelData * lastGoodRoom;

	//gameState info
	int gameState[GAME_STATE_SLOTS];
	int toggleState[TOGGLE_SLOTS];
	u32 minimapState[MINIMAP_ARRAY_SIZE];

	//enemy database
	EnemyType * enemyDatabase[ENEMY_DATABASE_SIZE];

	//mainChar info
	int hp;
	int hpMax;
	int mcPower;
	int mcArmor;
	int inventory[NUM_ITEMS];

	int mcGold;
	
	int checksum;
	LevelData * room1;  //for verifying rom status
	
} SaveGameData;

int calcCheckSum(SaveGameData * data);
bool checkCheckSum(SaveGameData * data);
void writeSaveGameToCart(SaveGameData * data, int saveSlot);
void loadSaveGameFromCart(SaveGameData * data, int saveSlot);

bool canSave = false;

void initSaveSystem()
{
	fatInitDefault();
}

void setCurrentSaveGameSlot(int saveSlot)
{
	if (saveSlot != CURRENT_SAVE_SLOT)
	{
		currentSaveGameSlot = saveSlot;
	}
}


void initGameState()
{
	int ctr;
	for (ctr = 0; ctr < GAME_STATE_SLOTS; ctr++)
	{
		setGameState(ctr, 0);
	}
	for (ctr = 0; ctr < TOGGLE_SLOTS; ctr++)
	{
		setToggleState(ctr, 0);
	}

	clearMiniMap();

	setCurrentSaveGameSlot(0);
}

void setGameState(int item, int state)
{
	gameState[item] = state;
}

int getGameState(int item)
{
	return gameState[item];
}

void setToggleState(int item, int state)
{
	toggleState[item] = state;
}

int getToggleState(int item)
{
	return toggleState[item];
}

void toggleToggleState(int item)
{
	if (toggleState[item]) 
	{
		toggleState[item] = 0;
	}
	else
	{
		toggleState[item] = 1;
	}
}

	
extern LevelData level_d1_2_0;
extern LevelData level_ovr_4_2;
void prepareSaveData(SaveGameData * save)
{
	//load up mainchar stats
	save->hp = getMainCharHP();
	save->hpMax = getMainCharMaxHP();
	save->mcPower = getMainCharBasePower();
	save->mcArmor = getMainCharArmor();
	save->mcGold = getMainCharGold();

	//load up location
	save->roomX = getMainCharX();
	save->roomY = getMainCharY();
	save->room = getCurrentLevel();

	
	save->lastGoodRoomX = lastGoodRoomX;
	save->lastGoodRoomY = lastGoodRoomY;
	save->lastGoodRoom = lastGoodRoom;
	
	//we save a pointer to the starting room, so we can
	//tell if a new version of the game has made our pointers change
	save->room1 = &level_d1_2_0;



	int i;
	//loop through inventory and gamestate....
	for(i = 0; i < GAME_STATE_SLOTS; i++)
	{
		save->gameState[i] = getGameState(i);
	}
	for(i = 0; i < TOGGLE_SLOTS; i++)
	{
		save->toggleState[i] = getToggleState(i);
	}
	for(i = 0; i < NUM_ITEMS; i++)
	{
		save->inventory[i] = getItemQty(i);	
	}
	
	//loop through enemy database
	for(i = 0; i < ENEMY_DATABASE_SIZE; i++)
	{
		save->enemyDatabase[i] = enemyAtDatabase(i);
	}

	//loop through and save minimap data
	u32 * minimapData = getMiniMapArray();
	for (i = 0; i < MINIMAP_ARRAY_SIZE; i++)
	{
		save->minimapState[i] = *minimapData++;
	}


	save->checksum = calcCheckSum(save);
}

bool saveGame(int saveSlot)
{
	setLastGoodRoom(getMainCharX(), getMainCharY(), getCurrentLevel());
	
	setCurrentSaveGameSlot(saveSlot);
	
	SaveGameData save;
	
	prepareSaveData(&save);


	writeSaveGameToCart(&save, getCurrentSaveSlot());

	return true;
	
}

bool loadGame(int saveSlot)
{
	setCurrentSaveGameSlot(saveSlot);
	
	SaveGameData save;

	loadSaveGameFromCart(&save, getCurrentSaveSlot());
	
	if (!checkCheckSum(&save))
	{
		return false;
	}

	int i;

	initGameState();

	for(i = 0; i < GAME_STATE_SLOTS; i++)
	{
		setGameState(i, save.gameState[i]);
	}
	for(i = 0; i < TOGGLE_SLOTS; i++)
	{
		setToggleState(i, save.toggleState[i]);
	}

	clearInventory();
	for (i = 0; i < NUM_ITEMS; i++)
	{
		adjustInventory(i, save.inventory[i]);
	}

	initEnemyDatabase();
	for (i = 0; i < ENEMY_DATABASE_SIZE; i++)
	{
		setEnemyAtDatabase(i, save.enemyDatabase[i]);	
	}

	//loop through and save minimap data
	clearMiniMap();
	u32 * minimapData = getMiniMapArray();
	for (i = 0; i < MINIMAP_ARRAY_SIZE; i++)
	{
		*minimapData++ = save.minimapState[i];
	}

	loadMCFromSave(save.hp, save.hpMax, save.mcPower, save.mcArmor, save.roomX, save.roomY, save.mcGold);

	int tmpScrX = max(save.roomX - 50, 0);
	int tmpScrY = max(save.roomY - 50, 0);

	setLastGoodRoom(save.lastGoodRoomX, save.lastGoodRoomY, save.lastGoodRoom);

	setLevel(save.room, tmpScrX, tmpScrY);

	if (isLevelOutdoors())
	{
		setLastGoodRoom(save.roomX, save.roomY, save.room);
	}

	killAllActualEnemies();


	return true;
}

int calcCheckSum(SaveGameData * data)
{
	//lame checksum, just sums everything ;-)
	//doesn't prevent cheaters (who cares about them)
	//but does check if data saved correctly
	u32 sum = data->roomX;
	sum += data->roomY;
	sum += (int)data->room;
	int i;
	for (i = 0; i < GAME_STATE_SLOTS; i++)
	{
		sum += data->gameState[i];	
	}
	sum += data->hp;
	sum += data->hpMax;
	sum += data->mcPower;
	sum += data->mcArmor;
	sum += data->mcGold;
	for (i = 0; i < NUM_ITEMS; i++)
	{
		sum += data->inventory[i];	
	}

	return sum;
	
}

void clearSavedEnemyDatabase(SaveGameData * data)
{
	int i;
	for (i = 0; i < ENEMY_DATABASE_SIZE; i++)
	{
		data->enemyDatabase[i] = null;
	}
}

bool checkCheckSum(SaveGameData * data)
{
	if (data->checksum == 0)
	{
		return false;
	}

	if (data->checksum != calcCheckSum(data))
	{
		return false;
	}
	
	//if the data has changed, so our level pointers are off,
	//then restart the player at the exit to dungeon 1.  Location is hard-coded (meh)
	//also, clear the enemy database, because we are using pointers
	if (data->room1 != &level_d1_2_0)
	{
		data->room = &level_ovr_4_2;
		data->roomX = 10 * 16;
		data->roomY = 6  * 16;
		clearSavedEnemyDatabase(data);
	}
	
	return true;
}


void writeSaveGameToCart(SaveGameData * data, int saveSlot)
{
	FILE * file = fopen("/anguna.dat", "wb");
	if (file == null)
	{
		//currently, no error message if this fails....
		return;
	}
	fwrite((u8*)data, sizeof(SaveGameData), 1, file);
	fclose(file);

}


void loadSaveGameFromCart(SaveGameData * data, int saveSlot)
{
	FILE * file = fopen("/anguna.dat", "rb");
	if (file == null)
	{
		file = fopen("/anguna.sav", "rb");
		if (file == null) {
			//if we can't open it, then we need to invalidate the save state.
			//easiest way to do that is by setting the checksum to some random number...
			data->checksum = 1234;
			return;
		}
	}
	fread((void*)data, sizeof(SaveGameData), 1, file);
	fclose(file);

}

void setLastGoodRoom(int x, int y, LevelData * room)
{
	if (lastGoodRoom != room)
	{
		lastGoodRoomX = x;
		lastGoodRoomY = y;
		lastGoodRoom = room;
	}
}

int getCurrentSaveSlot()
{
	return currentSaveGameSlot;
}

void updateSaveToRespawn()
{
	SaveGameData save;
	
	prepareSaveData(&save);
	
	save.roomX = save.lastGoodRoomX;
	save.roomY = save.lastGoodRoomY;
	save.room = save.lastGoodRoom;
	save.hp = save.hpMax;

	save.checksum = calcCheckSum(&save);


	writeSaveGameToCart(&save, getCurrentSaveSlot());
	
}

bool verifySaveGame(int saveSlot)
{
	SaveGameData save;

	loadSaveGameFromCart(&save, saveSlot);

	if (!checkCheckSum(&save))
	{
		return false;
	}

	return true;


}

void clearZoneBasedGameState()
{
	int i;
	for (i = GAMESTATE_ZONE_SAVE_START; i < GAME_STATE_SLOTS; i++)
	{
		setGameState(i, 0);
	}
	
}



