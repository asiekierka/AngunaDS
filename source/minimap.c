#include "minimap.h"
#include "text.h"
#include "textData.h"
#include "level.h"
#include "subscreen.h"
#include "gfx/overworld_minimap.h"
#include "enemies/itemGfx.h"
#include "item.h"
#include "bgDefines.h"

#define USEMINIMAP 


#define MINIMAP_HEIGHT 10
#define MINIMAP_WIDTH 10
#define OVERWORLD_WIDTH 10
#define OVERWORLD_HEIGHT 7 
#define MINIMAP_CY 3
#define MINIMAP_CX 4
#define MINIMAP_TILE_START  84
#define MINIMAP_TILE_DARK 84
#define MINIMAP_BYTES_PER_MAP 5

#define MAP_PIN_SPRITE_TILE 211

LevelData * g_lastLevel;

typedef struct
{
	int mapItemIndex;
	int x;
	int y;

} PurchasedMapInfo;

struct MapPinDefStruct
{
	int x;
	int y;
	int mapRoomX;
	int mapRoomY;
	const char ** name;
	struct MapPinDefStruct * next;
	struct MapPinDefStruct * prev;
};

typedef struct MapPinDefStruct MapPinDef;


const PurchasedMapInfo purchasedMapInfo[] =
{
	{ITEM_MAP_DNGN_2, 9, 0},
	{ITEM_MAP_DNGN_3, 5, 1},
	{ITEM_MAP_DNGN_4, 1, 2},
	{ITEM_MAP_DNGN_4, 1, 1},
	{0,0,0}
};

#define PIN(x) (&mapPinDefinitions[x])
#define TXT(x) (&pinLocationsText[x])
#define MAPPIN(x) TXT(x),PIN((x)+1),PIN((x)-1)

MapPinDef mapPinDefinitions[] =
{
	[PIN_TREASURE_CAVE] = {5  ,1   ,0  ,0  , TXT(PIN_TREASURE_CAVE), PIN(PIN_SHOP_1), PIN(PIN_DNGN_2)},
	[PIN_DNGN_1]   =      {68 ,16  ,4  ,2  , MAPPIN(PIN_DNGN_1)},
	[PIN_DNGN_3]   =      {89 ,11  ,5  ,1  , MAPPIN(PIN_DNGN_3)},
	[PIN_DNGN_4]   =      {23 ,17  ,1  ,2  , MAPPIN(PIN_DNGN_4)},
	[PIN_DNGN_5]   =      {87 ,0   ,5  ,0  , MAPPIN(PIN_DNGN_5)},
	[PIN_HEALER_1] =      {24 ,1   ,1  ,0  , MAPPIN(PIN_HEALER_1)},
	[PIN_HEALER_2] =      {58 ,17  ,3  ,2  , MAPPIN(PIN_HEALER_2)},
	[PIN_HEALER_3] =      {135,12  ,8  ,1  , MAPPIN(PIN_HEALER_3)},
	[PIN_SHOP_1]   =      {18,42  ,1  ,5  , MAPPIN(PIN_SHOP_1)},
	[PIN_SHOP_2]   =      {69 ,42  ,4  ,5  , MAPPIN(PIN_SHOP_2)},
	[PIN_SHOP_3]   =      {120,4   ,7  ,1  , MAPPIN(PIN_SHOP_3)},
	[PIN_SHOP_4]   =      {151,19  ,9  ,2  , MAPPIN(PIN_SHOP_4)},
	[PIN_WESTERN_PASS_ENTRANCE] =  {51,28  ,3  ,3  , MAPPIN(PIN_WESTERN_PASS_ENTRANCE)},
	[PIN_WESTERN_PASS_EXIT]   =    {45,43  ,2  ,5  , MAPPIN(PIN_WESTERN_PASS_EXIT)},
	[PIN_ROCKY_CAVE]   =           {116,32  ,7  ,4  , MAPPIN(PIN_ROCKY_CAVE)},
	[PIN_OASIS_PATH_ENTRANCE]   =  {154,41  ,9  ,5  , MAPPIN(PIN_OASIS_PATH_ENTRANCE)},
	[PIN_OASIS_PATH_EXIT]   =      {145,37  ,9  ,4  , MAPPIN(PIN_OASIS_PATH_EXIT)},
	[PIN_DNGN_2]   =      {157, 1  ,9  ,0  , TXT(PIN_DNGN_2), PIN(PIN_TREASURE_CAVE), PIN(PIN_OASIS_PATH_ENTRANCE)},
};

MapPinDef * currentMapPin = &mapPinDefinitions[PIN_DNGN_1];


/**
 * Declare the storage for what map rooms we've visited. 
 * (since I'm avoiding malloc/free)
 */
u32 miniMapVisited[] = {0,0,0,0,0,  //d1
						0,0,0,0,0,  //d2
						0,0,0,0,0,  //d3
						0,0,0,0,0,  //d4?
						0,0,0,0,0,  //d5?
						0,0,0,0,0,  //d6?
						0,0,0,0,0,  //d7?
};

u32 * getMiniMapArray()
{
	return miniMapVisited;
}

bool hasMiniMap() 
{
	int mapNum = getLevelMapNum();
	return (mapNum != NO_MINIMAP);
}

void clearMiniMap()
{
	int i;
	for (i = 0; i < MINIMAP_ARRAY_SIZE; i++)
	{
		miniMapVisited[i] = 0;
	}
	currentMapPin = &mapPinDefinitions[PIN_DNGN_1];
}

bool getMiniMapVisited(int x, int y, int mapNum)
{
	//don't jack stuff up if there's no minimap
	if (mapNum == NO_MINIMAP)
	{
		return false;
	}

	//ugly math is mapping from xy coordinates in the bit array of the minimap "been there" state
	u32 isVisited = miniMapVisited[(y >> 1) + (mapNum * MINIMAP_BYTES_PER_MAP)] & (1 << (x + ((y & 1) ? 10:0)));
	if (isVisited == 0)
	{
		return false;
	}
	return true;
}

void setMiniMapVisited(int x, int y, int mapNum)
{
	//don't jack stuff up if there's no minimap
	if (mapNum == NO_MINIMAP)
	{
		return;
	}
	
	//ugly math is mapping from xy coordinates in the bit array of the minimap "been there" state
	miniMapVisited[(y >> 1) + (mapNum * MINIMAP_BYTES_PER_MAP)] |= (1 << (x + ((y & 1) ? 10:0)));
}



int getMiniMapTile(int x, int y, int mapNum)
{
	if (mapNum == MINIMAP_OVERWORLD)
	{
		return MINIMAP_TILE_DARK + 2;

	}
	if ((x < 0) || (y < 0) || (x > ZONE_MAX_X) || (y > ZONE_MAX_Y))
	{
		return MINIMAP_TILE_DARK;
	}

	if (getMiniMapVisited(x, y, mapNum))
	{
		return MINIMAP_TILE_START + getZoneMiniMapChip(x,y);
	}

	return MINIMAP_TILE_DARK;
	
}

void updateMiniMap()
{
#ifdef USEMINIMAP

	int i;
	int j;

	Point pt;
	Point * position = &pt;

	Point dim;
	Point * dimensions = &dim;

	calcMiniMapPos(position);
	calcMiniMapDim(dimensions);

	int mapNum = getLevelMapNum();

	if (mapNum == MINIMAP_OVERWORLD)
	{
		setMiniMapVisited(position->x, position->y, mapNum);
	}
	else
	{
		//set visited for the entire room we're in
		for (i = 0; i < dimensions->x; i++)
		for (j = 0; j < dimensions->y; j++)
		{
			setMiniMapVisited(position->x + i, position->y + j, mapNum);
		}
	}
	
#endif

}

void drawMapPin(int xOrigin, int yOrigin)
{
	int x = currentMapPin->x;
	int y = currentMapPin->y;

	showCustomSprite(7, MAP_PIN_SPRITE_TILE, x + ((xOrigin)  * 8) - 2, y + ((yOrigin - 2) * 8 + 2), SUB_ITEMS_PAL);
	const char * name = *(currentMapPin->name);

	writeSubText(xOrigin + 2, yOrigin - 2, "                 ");
	writeSubText(xOrigin + 2, yOrigin - 2, name);

}

void drawMiniMapOverworld(int xOrigin, int yOrigin)
{
	loadSubBGPaletteLine((u16*)overworld_minimapPal, ENEMY_DB_PALETTE_NUM);
	loadSubBGPaletteLine((u16*)overworld_minimapPal, MINIMAP_SPECIAL_PALETTE_NUM);
	lightenSubBGPalette(MINIMAP_SPECIAL_PALETTE_NUM,-6,-6,-6);
	loadTilesToPortraitSpace((u16*)overworld_minimapTiles, 180);


	int i,j;


	Point pt;
	Point * position = &pt;

	calcMiniMapPos(position);

	int tile = PORTRAIT_TILE_START;
	for (j = 0; j < 6; j++)
	{
		for (i = 0; i < 10; i++)
		{
			if (getMiniMapVisited(i, j, MINIMAP_OVERWORLD))
			{
				int pal = ENEMY_DB_PAL;
				if ((position->x == i) && ((position->y == j)))
				{
					pal = MINIMAP_SPECIAL_PAL;
				}

				writeSubTileWithPal(i * 2 + xOrigin,     j + yOrigin, tile, pal);
				writeSubTileWithPal(i * 2 + xOrigin + 1, j + yOrigin, tile + 1, pal);
			}
			tile += 2;
		}
	}

	if (isPaused())
	{
		drawMapPin(xOrigin, yOrigin);
	}
	else
	{
		clearCustomSprites();
	}
}


void updateDrawMiniMap()
{
	//don't redraw unless we've switched levels
	LevelData * current = getCurrentLevel();
	if (current == g_lastLevel)
	{
		return;
	}
	g_lastLevel = current;
	showMiniMap();
}

void forceMiniMapUpdate()
{
	g_lastLevel = null;
}

void drawMiniMap()
{

	int center = (MAP_X + MAP_X2) >> 1;
	writeSubText(center - 1, MAP_Y + 1, "Map");
	int xOrigin = center - 5;
	int yOrigin = MAP_Y + 2;

	int mapNum = getLevelMapNum();
	if (mapNum == MINIMAP_OVERWORLD)
	{
		drawMiniMapOverworld(xOrigin - 4, yOrigin + 1);
		return;
	}


	int i;
	int j;

	int offX = getMiniMapOffsetX();
	int offY = getMiniMapOffsetY();

	int rx;
	int ry;

	Point pt;
	Point * position = &pt;

	Point dim;
	Point * dimensions = &dim;

	calcMiniMapPos(position);
	calcMiniMapDim(dimensions);
	
	for (i = 0; i < MINIMAP_WIDTH; i++)
	for (j = 0; j < MINIMAP_HEIGHT; j++)
	{
	
		int tile = getMiniMapTile(i, j, mapNum);

		bool drawn = false;
		if (tile != MINIMAP_TILE_DARK)
		{
			for (rx = position->x; rx < position-> x + dimensions->x; rx++)
			{
				if (rx == i)
				{
					for (ry = position->y; ry < position->y + dimensions->y; ry++)
					{
						if (ry == j)
						{
							writeSubTileWithPal(xOrigin + i + offX, yOrigin + j + offY, tile, MINIMAP_PAL);
							drawn = true;
						}
					}
				}
			}
			if (!drawn)
			{
				writeSubTile(xOrigin + i + offX, yOrigin + j + offY, tile);
			}
		}

	}
}

void miniMapBeenEverywhere()
{
	int i;
	for (i = 0; i < MINIMAP_ARRAY_SIZE; i++)
	{
		miniMapVisited[i] = 0xffffffff;
	}
}

void purchasedMap(int itemNumber)
{
	const PurchasedMapInfo * mapInfo = &purchasedMapInfo[0];

	while (mapInfo->mapItemIndex != 0)
	{
		if (mapInfo->mapItemIndex == itemNumber)
		{
			setMiniMapVisited(mapInfo->x, mapInfo->y, MINIMAP_OVERWORLD);
		}
		mapInfo++;
	}
}

void incrementMapPin()
{
	MapPinDef * nextPin = currentMapPin;

	bool done = false;

	while (!done)
	{
		nextPin = nextPin->next;
		//handle case where our current pin is the only discovered pin
		if (nextPin == currentMapPin)
		{
			return;
		}
		//if we've discovered the next pin, use it....
		int x = nextPin->mapRoomX;
		int y = nextPin->mapRoomY;
		if (getMiniMapVisited(x, y, MINIMAP_OVERWORLD))
		{
			currentMapPin = nextPin;
			done = true;
		}
	}
	
	drawMiniMap();
}

void decrementMapPin()
{
	MapPinDef * nextPin = currentMapPin;

	bool done = false;

	while (!done)
	{
		nextPin = nextPin->prev;
		//handle case where our current pin is the only discovered pin
		if (nextPin == currentMapPin)
		{
			return;
		}
		//if we've discovered the next pin, use it....
		int x = nextPin->mapRoomX;
		int y = nextPin->mapRoomY;
		if (getMiniMapVisited(x, y, MINIMAP_OVERWORLD))
		{
			currentMapPin = nextPin;
			done = true;
		}
	}
	drawMiniMap();
}

