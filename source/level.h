#ifndef _level_h_
#define _level_h_

#include "config.h"
#include "debugBuilds.h"
#include "characters.h"
#include "enemies.h"
#include "roomScriptDefinitions.h"
#include "gameState.h"

#define ZONESAVE(x) ((x) + GAMESTATE_ZONE_SAVE_START)

#define OBJ_MAP 0
#define OBJ_NORMAL 1
#define OBJ_BREAK 2
#define OBJ_PUSH 3
#define OBJ_OBSTACLE 4

#define NO_MINIMAP -1
#define MINIMAP_OVERWORLD 0



//map objects
enum
{
	OBJ_SKEL,
	OBJ_SKULL1,
	OBJ_SKULL2,
	OBJ_POT_DUMP1,
	OBJ_POT_DUMP2,
	OBJ_CHEST,
	OBJ_POT1,
	OBJ_POT2,
	OBJ_POT3,
	OBJ_CRATE1,
	OBJ_CRATE2,
	OBJ_CRATE3,
	OBJ_BONES1,
	OBJ_BONES2,
	OBJ_BONES3,
	OBJ_BONES4,
	OBJ_BONES5,
	OBJ_BUSH,
	OBJ_STAIRS_DOWN,
	OBJ_STAIRS_UP,
	OBJ_CACTUS,
	OBJ_TREE_DEAD_1,
	OBJ_TREE_DEAD_2,
	OBJ_TREE_DEAD_3,
	OBJ_TREE_DEAD_4,
	OBJ_STUMP,
};


typedef struct
{
	int numTiles;
	u8 * tileGfx;
	u8 * tileMap;
	
} BackDropData;



typedef struct
{
	void * newLevel;
	Point start; 
	Point end;
	Point destination;
	bool forceMosaic;
	
} DoorData;

typedef struct
{
	u8 * minimap;
	int mapNum;
	int minimapOffx;
	int minimapOffy;
	int music;
} ZoneData;


typedef struct LevelDataStruct
{
	//these define the map
	int width;
	int height;
	u16 * tileMap;
	u16 * fgMap;

	//this defines the graphics data
	u16 * palette;
	int numTiles;
	u8 * tileGfx;

	int minCollisionTile;
	int waterCollisionTile;

	//this defines the additional informatino
	BackDropData * backdrop;
	int numEnemies;
	EnemyStartData * enemyStart;

	int miniMapX;
	int miniMapY;

	int numDoors;
	DoorData * doors;

	bool respawnRoom;
	bool dark;
	
	ZoneData * zone;

} LevelData;

void initLevelLoader();

int getWorldX();
int getWorldY();

int worldToScreenX(int worldX);
int worldToScreenY(int worldY);
int screenToWorldX(int screenX);
int screenToWorldY(int screenY);

void setLevel(LevelData * level, int startX, int startY);
void scrollWorldX(int amount);
void scrollWorldY(int amount);
void checkDoors(CharacterStatus * character);
bool isClearOfDoor(CharacterStatus * character);
void updateMaps();
void calcMiniMapPos(Point * position);
void calcMiniMapDim(Point * dimension);
int getMiniMapOffsetX();
int getMiniMapOffsetY();
void levelAllEnemiesDead();
void doDarkIfDark();

int getZoneMiniMapChip(int x, int y);
int getTileAtWorldCoord(int x, int y);
void writeTileToWorldCoord(int x, int y, int tile);

bool tileIsWallCollision(int x, int y, int mobility);

void resetWorldPosition();

int getLevelWidth();
int getLevelHeight();
int getLevelXBuffer();
int getLevelYBuffer();
int getLevelMapNum();
bool isLevelOutdoors();

LevelData * getCurrentLevel();

#define ZONE_MAX_X 25
#define ZONE_MAX_Y 25

bool objectIsInGameSpace(int x, int y);

#endif
