#include "level.h"
#include "bg.h"
#include "text.h"
#include "sound.h"
#include "minimap.h"
#include "misc.h"
#include "enemies.h"
#include "splash.h"
#include "spriteMgr.h"
#include "darkness.h"
#include "gameState.h"
#include "mainChar.h"
#include "characters.h"
#include "gameState.h"
#include "overworld.h"
#include "bgDefines.h"
#include "enemyDatabase.h"

#define LEVELSCALE 3
#define LOADING_BLUR_SPEED 3

#define LEVEL_INIT_NOT_DONE 0
#define LEVEL_INIT_DONE 1
#define LEVEL_INIT_NOW 2

#define BGTILE(x,y) ((y) | ((y) < (x)->minCollisionTile?BGPAL(0):(BGPAL(1))))
#define FGTILE(x,y) ((y) | BGPAL(1))



//u32* OAMmem  		=(u32*)0x7000000;
//u16* VideoBuffer 	=(u16*)0x6000000;
//u16* OAMData		=(u16*)0x6010000;
//u16* BGPaletteMem 	=(u16*)0x5000000;
//u16* OBJPaletteMem 	=(u16*)0x5000200;

IN_EWRAM unsigned short fgMapData[2400];
IN_EWRAM unsigned short bgMapData[2400];

const u8 nullMap[] = {
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
};

const ZoneData nullZone = {
	(u8 *) nullMap
};

int worldPosX;
int worldPosY;

int currentLevelYBuffer;
int currentLevelXBuffer;

Bg bgForeground;

Bg bgMain;

LevelData * currentLevel;

u16 dungeonBgPalApplier(LevelData * level, int tile)
{
	return BGTILE(level, tile);
}

u16 dungeonFgPalApplier(LevelData * level, int tile)
{
	if (tile < 0)
	{
		return 4 | BGPAL(ALL_BLACK_PAL_NUM);
	}
	return FGTILE(level, tile);
}

u16 ovrPalApplier(LevelData * level, int tile)
{
	if (tile < 0)
	{
		return 4 | BGPAL(ALL_BLACK_PAL_NUM);
	}
	return applyOverworldPalette(level, tile);
}

bool isLevelOutdoors()
{
	if ((currentLevel->zone->minimap == null) || (currentLevel->zone->mapNum == NO_MINIMAP))
	{
		return true;
	}
	return false;
}


int getWorldX()
{
	return worldPosX;
}

int getWorldY()
{
	return worldPosY;
}

bool isLevelNarrow()
{
	return (currentLevel->width < SCREEN_BLOCK_WIDTH);
}

bool isLevelShort()
{
	return (currentLevel->height < SCREEN_BLOCK_HEIGHT);
}

int worldToScreenX(int worldX)
{
	return worldX - getWorldX(); 
}

int worldToScreenY(int worldY)
{
	return worldY - getWorldY();	
}

int screenToWorldX(int screenX)
{
	return screenX + getWorldX();
}

int screenToWorldY(int screenY)
{
	return screenY + getWorldY();
}

void resetWorldPosition()
{
	worldPosY = 0;
	worldPosX = 0;
	
}

void calcMiniMapPos(Point * position)
{
	position->x = currentLevel->miniMapX;
	position->y = currentLevel->miniMapY;
}

void calcMiniMapDim(Point * dimension)
{
	dimension->x = divSmall(currentLevel->width, 30);
	dimension->y = divSmall(currentLevel->height, 20);
}

int getMiniMapOffsetX()
{
	return currentLevel->zone->minimapOffx;
}

int getMiniMapOffsetY()
{
	return currentLevel->zone->minimapOffy;
}



void scrollWorldY(int amount)
{
	if (isLevelShort())
	{
		return;
	}

	worldPosY += amount;
	if (worldPosY < 0)
	{
		amount = amount - worldPosY;
		worldPosY = 0;
	}
	if (((worldPosY + SCREEN_HEIGHT) >> 3) >= currentLevel->height)
	{
		int old = worldPosY;
		worldPosY = (currentLevel->height << 3) - SCREEN_HEIGHT;
		amount = old - worldPosY;
	}

	if (amount > 0)
	{
		ScrollBgMapDown(&bgMain, currentLevel, worldPosX, worldPosY);
	}
	else
	{
		ScrollBgMapUp(&bgMain, currentLevel, worldPosX, worldPosY);
	}
}


void scrollWorldX(int amount)
{

	if (isLevelNarrow())
	{
		return;
	}


	worldPosX += amount;
	if (worldPosX < 0)
	{
		amount = amount - worldPosX;
		worldPosX = 0;
	}


	if (((worldPosX + SCREEN_WIDTH) / 8) >= currentLevel->width)
	{
		int old = worldPosX;
		worldPosX = (currentLevel->width * 8) - SCREEN_WIDTH;
		amount = old - worldPosX;
	}


	if (amount > 0)
	{
		ScrollBgMapRight(&bgMain, currentLevel, worldPosX, worldPosY);
	}
	else
	{
		ScrollBgMapLeft(&bgMain, currentLevel, worldPosX, worldPosY);
	}
}

int populateBGTiles(LevelData * lvl)
{
	int i;
	int j;
	for(i = 0; i < 32; i++)
	for(j = 0; j < 32; j++)
	{
		bgForeground.mapData[i + j*32] = lvl->backdrop->tileMap[i + j*32];
	}
	
	return 0;
}


int doneLevelInit;

void initLevelLoader()
{
	doneLevelInit = LEVEL_INIT_NOT_DONE;
}


void copyLevelDataToEWRam(LevelData * level)
{
	//copyMem16(bgMapData, level->tileMap, level->width * level->height);
	//copyMem16(fgMapData, level->fgMap, level->width * level->height);
	int sy,sx;
	int width = (level->width);

	u8 * src = (u8*)level->tileMap;
	u16 * dest = bgMapData;
	u16 * destSecond = bgMapData + width;

	u8 * fgSrc = (u8*)level->fgMap;
	u16 * fgDest = fgMapData;
	u16 * fgDestSecond = fgMapData + width;

	int tile;
	int sourceWidth = (width >> 1);
	int sourceHeight = (level->height >> 1);
	for (sy = 0; sy < sourceHeight; sy++)
	{
		for (sx = 0; sx < sourceWidth; sx++)
		{
			
			tile = *src++;
			tile <<= 2;
			*dest++ = tile++;
			*dest++ = tile++;
			*destSecond++ = tile++;
			*destSecond++ = tile++;

			tile = *fgSrc++;
			tile <<= 2;
			*fgDest++ = tile++;
			*fgDest++ = tile++;
			*fgDestSecond++ = tile++;
			*fgDestSecond++ = tile++;

		}

		dest += width;
		destSecond += width;

		fgDest += width;
		fgDestSecond += width;
	}

}


//I shouldn't need to do this nastiness...see comments about bad design below :(
//extern u16* overworld_Tiles[];
//extern u16* overworld2_Tiles[];
//extern u16* overworldRocky_Tiles[];
//extern u16* cave_Tiles[];

extern LevelData level_ovr_0_0;
extern LevelData level_ovr_9_9;


/**
 * sets the new level. 
 * startX, startY is the x,y (pixel level) of where the top-left corner
 * of the screen should be in the level
 */
void setLevelWithMosaic(LevelData * level, int startX, int startY, bool forceMosaic)
{
#ifdef ONLY_LEVEL_1
	if (level == (LevelData *)&level_ovr_0_0)
	{
		doDemoOver();
		return;
	}
#endif


	bool muffMosaic = false;

	setLastHitEnemy(null);

	if ((doneLevelInit != LEVEL_INIT_DONE) || 
		(level->tileGfx != currentLevel->tileGfx) ||
		(level->zone != currentLevel->zone) ||
		(forceMosaic))
	{

		if ((!forceMosaic) && (level->zone == currentLevel->zone))
		{
			muffMosaic = true;
		}

		//not sure if I like this, but maybe....
		if (isDark() || (levelWillBeDark(level)))
		{
			muffMosaic = true;
		}

		clearZoneBasedGameState();
		//only blur out if we've already started playing...
		if (doneLevelInit == LEVEL_INIT_DONE)
		{
			disableMainSprites();	
			if (!muffMosaic){
				bgMosaicBlurOut(LOADING_BLUR_SPEED);
			}
		}
		
		disableBg(BG_MAIN);
		disableBg(BG_FOREGROUND);

		loadBGPaletteLine(level->palette, 0);
		loadBGPaletteLine((level->palette+16), 1);
		loadBGPaletteLine((level->palette+32), 2);
		loadBGPaletteLine((level->palette+48), 3);
		loadBGPaletteLine((level->palette+64), 4);
		loadBlankBGPaletteLine(ALL_BLACK_PAL_NUM);
		initBackground(&bgMain, BG_MAIN_NUM);
		bgMain.colorMode = BG_COLOR_16;
		bgMain.size = TEXTBG_SIZE_512x256;
		enableBackground(&bgMain);
		loadBGTiles(&bgMain, (u32*)level->tileGfx, 1024);
		zeroBGMap(&bgMain, 32*2);	
		initBackground(&bgForeground, BG_FOREGROUND_NUM);
		bgForeground.priority = 1;
		bgForeground.mapBlock = 26;
		bgForeground.colorMode = BG_COLOR_16;
		bgForeground.size = TEXTBG_SIZE_512x256;
		enableBackground(&bgForeground);
		bgMain.foreground = &bgForeground;
		doneLevelInit = LEVEL_INIT_NOW;

		disableBg(BG_MAIN);
		disableBg(BG_FOREGROUND);
	}
	if (level->palette != currentLevel->palette)
	{
		loadBGPaletteLine(level->palette, 0);
		loadBGPaletteLine((level->palette+16), 1);
	}


	copyLevelDataToEWRam(level);

	//tick the time for these, so we can 
	//make sure not to reload over one in use
	timeTickForPalette();
	timeTickForSpriteGfxBlocks();

	//clear the bullets when we go offscreen
	initBullets();

	//init any music changes
	if ((currentLevel->zone != level->zone) || (doneLevelInit != LEVEL_INIT_DONE))
	{
		
		playMusic(level->zone->music);
		forceMiniMapUpdate();
	}


	currentLevel = level;

	//chop this evenly so we don't get issues with the screen being off by a small amount
	//gridX,gridY is the tile-level position of the upper-left corner of the screen in the level
	int gridX = startX >> 3;
	int gridY = startY >> 3;

	//don't let the map start too far right/down, and start showing gunk
	//offsetX is how much to offset because too much is showing on the right.
	int offsetX = level->width  - (gridX + SCREEN_BLOCK_WIDTH);
	int offsetY = level->height - (gridY + SCREEN_BLOCK_HEIGHT);
	if (offsetX < 0)
	{
		gridX += offsetX;
	}
	if (gridX == 1)
	{
		gridX = 0;
	}

	if (offsetY < 0)
	{
		gridY += offsetY;
	}


	if (isLevelShort())
	{
		gridY = level->height - SCREEN_HEIGHT / 8;
	}

	if (isLevelNarrow())
	{
		gridX = -1;
	}

	worldPosX = gridX * 8;
	worldPosY = gridY * 8;


	if (gridY == 1)
	{
		gridY = 0;
	}

	currentLevelXBuffer = (level->width * 8) - 24;
	currentLevelYBuffer = (level->height * 8) - 40;

	if (worldPosX >= 0)
	{
		bgMain.x_scroll = worldPosX & 511;
	}
	else
	{
		bgMain.x_scroll = 512 + worldPosX;
	}

	if (worldPosY >= 0)
	{
		bgMain.y_scroll = worldPosY & 255;
	}
	else
	{
		bgMain.y_scroll = 256 + worldPosY;
	}



	
	updateMiniMap();


	if (isLevelOutdoors())
	{
		setPaletteAppliers(ovrPalApplier, ovrPalApplier);
	}
	else
	{
		setPaletteAppliers(dungeonBgPalApplier, dungeonFgPalApplier);
	}
	initScreen(&bgMain, level, gridX, gridY, &bgForeground);

	doDarkIfDark();
	if (doneLevelInit == LEVEL_INIT_NOW)
	{
		enableBg(BG_FOREGROUND);
		enableBg(BG_MAIN);
		if (!muffMosaic)
		{
			bgMosaicBlurIn(LOADING_BLUR_SPEED);
		}
		updateMainChar();
	}

	if (isLevelOutdoors(currentLevel))
	{
		if (currentLevel->respawnRoom)
		{
			setLastGoodRoom(getMainCharX(), getMainCharY(), currentLevel);
		}
	}


	resetGfxStats();
	initEnemies(level->numEnemies, level->enemyStart);
	sortSpriteList();
	frameTick();
	enableBg(BG_FOREGROUND);
	enableBg(BG_MAIN);
	updateSpriteDisplay();

	if (doneLevelInit == LEVEL_INIT_NOW)
	{
		frameTick();
		enableSprites();
		doneLevelInit = LEVEL_INIT_DONE;
	}

	//special case...when we reach ovr_9_9, the game is over, the player has won.
	if (level == (LevelData *)&level_ovr_9_9)
	{
		doCredits();
		return;
	}


}

void setLevel(LevelData * level, int startX, int startY)
{
	setLevelWithMosaic(level, startX, startY, false);

}

void doDarkIfDark()
{
	if (currentLevel->dark)
	{
		activateDarkMode();
	}
	else
	{
		deactivateDarkMode();
	}
}

void levelAllEnemiesDead(int x, int y)
{
	if (currentLevel->respawnRoom)
	{
		setLastGoodRoom(getMainCharX(), getMainCharY(), currentLevel);
	}
}


void updateMaps()
{
	updateBackground(&bgMain);
	//updateBackground(&bgForeground);
	//updateMiniMap();
}

bool isClearOfDoor(CharacterStatus * character)
{
	int x = character->x;
	int y = character->y;
	int i;
	DoorData * tmp;
	for (i = 0; i < currentLevel->numDoors; i++)
	{
		tmp = &currentLevel->doors[i];

		if ((x >= min(tmp->start.x, (currentLevel->width << 3) + 1)) &&
		    (x <= tmp->end.x) &&
			(y >= min(tmp->start.y, (currentLevel->height << 3) + 1)) &&
			(y <= tmp->end.y + 32))
		{
			return false;
		}
	}

	return true;


}


void checkDoors(CharacterStatus * character)
{
	int x = character->x;
	int y = character->y;
	int i;
	DoorData * tmp;
	for (i = 0; i < currentLevel->numDoors; i++)
	{
		tmp = &currentLevel->doors[i];

		if ((x >= min(tmp->start.x, (currentLevel->width << 3) + 1)) &&
		    (x <= tmp->end.x) &&
			(y >= min(tmp->start.y, (currentLevel->height << 3) + 1)) &&
			(y <= tmp->end.y))
		{
			//do door
			//if we are supposed to preserve X or Y, keep it, else
			//load from the door definition
			int destX = (tmp->destination.x == PRESERVE)?x:tmp->destination.x;
			int destY = (tmp->destination.y == PRESERVE)?y:tmp->destination.y;

			//if it's MAXEDGE, get the far edge
			if (destX == MAXEDGE)
			{
				destX = ((LevelData *)(tmp->newLevel))->width << 3;
			}
			if (destY == MAXEDGE)
			{
				destY = ((LevelData *)(tmp->newLevel))->height << 3;
			}

			//don't let the screen corner dip below 0
			int tmpScrX = max(destX - 50, 0);
			int tmpScrY = max(destY - 50, 0);

			character->x = destX;
			character->y = destY;
			setLevelWithMosaic(tmp->newLevel, tmpScrX , tmpScrY, tmp->forceMosaic );
			//make sure we update the position of our sprites vs the screen
			//...this is the easiest way to force it to update...
			//break out of the loop so we don't accidentally go in another door
			//scrollWorldX(1);
			//scrollWorldY(1);

			i=99;
		}
	
	}
}


int getZoneMiniMapChip(int x, int y)
{
	ZoneData * currZone = currentLevel->zone;
	if (currZone == null) 
	{
		return 0;
	}
	
	return currentLevel->zone->minimap[x + y * 10];
}



int getTileAtWorldCoord(int x, int y)
{
	//return currentLevel->tileMap[(x + y * currentLevel->width)];
	return bgMapData[x + (y * currentLevel->width)];
}

void writeTileToWorldCoord(int x, int y, int tile)
{
	bgMapData[x + (y * currentLevel->width)] = tile;
	writeOneTile(&bgMain, currentLevel, x, y, tile);
}



bool isDungeonWallCollision(int x, int y, int mobility)
{

	int tile = getTileAtWorldCoord(x, y);
	int collideAt;
	
	//special case...tiles 0-3 are always empty blank tiles.  They often act as a pit...
	if (tile < 4)
	{
		tile = currentLevel->waterCollisionTile;
	}
	
	//if flying, and not on the edge of the screen, can go over water collisions...
	if ((mobility > MOBILITY_NORMAL) && (x != 0) && (y!= 0) && 
		(x != (currentLevel->width - 1)) && (y != (currentLevel->height - 1)))
	{
		collideAt = currentLevel->minCollisionTile;
	}
	else
	{
		collideAt = currentLevel->waterCollisionTile;
	}
	if (tile >= collideAt)
	{
		return true;
	}
	return false;
	
	
}

//odd special case to handle not walking off south edge of river
extern LevelData level_ovr_6_5;
extern LevelData level_ovr_5_5;

bool tileIsWallCollision(int x, int y, int mobility )
{
	if (mobility == MOBILITY_GHOST)
	{
		return false;
	}

	x = max(0,x);
	x = min(x, currentLevel->width - 1);
	
	
	y = max(0,y);
	y = min(y, currentLevel->height - 1);

	//special case for south edge of river
	if ((currentLevel == &level_ovr_6_5) || (currentLevel == &level_ovr_5_5))
	{
		if (y > currentLevel->height - 2)
		{
			return true;
		}
	}


	if (isLevelOutdoors())
	{
		return isOutdoorWallCollision(currentLevel, x, y, mobility);
	}
	else
	{
		return isDungeonWallCollision(x, y, mobility);
	}
}


int getLevelWidth()
{
	return currentLevel->width;
}

int getLevelHeight()
{
	return currentLevel->height;
}

int getLevelXBuffer()
{
	return currentLevelXBuffer;
}

int getLevelYBuffer()
{
	return currentLevelYBuffer;
}


LevelData * getCurrentLevel()
{
	return currentLevel;	
}

int getLevelMapNum()
{
	int map = currentLevel->zone->mapNum;
	if (map == NO_MINIMAP)
	{
		return map;
	}
	if (map != MINIMAP_OVERWORLD)
	{
		map += 2;
	}
	return currentLevel->zone->mapNum;
}

bool objectIsInGameSpace(int x, int y)
{
	if (x < -64)
	{
		return false;
	}
	if (y < -64)
	{
		return false;
	}
	LevelData * level = getCurrentLevel();
	if (x > (level->width * 8) + 64)
	{
		return false;
	}
	if (y > (level->height * 8) + 64)
	{
		return false;
	}
	return true;
}


