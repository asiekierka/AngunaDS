#include "subscreen.h"
#include "splash.h"
#include "text.h"
#include "misc.h"
#include "bg.h"
#include "mainChar.h"
#include "item.h"
#include "hud.h"
#include "enemies/doorGfx.h"
#include "enemies/itemGfx.h"
#include "items.h"
#include "sound.h"
#include "darkness.h"
#include "textData.h"
#include "gameState.h"
#include "enemies.h"
#include "enemyDatabase.h"
#include "bgDefines.h"
#include "minimap.h"
#include "level.h"
#include "input.h"
#include "platform.h"
#include "text.h"



#define PORTRAIT_HEIGHT 12
#define PORTRAIT_WIDTH 12

#define MAP_SPRITE_TILE 209

#define CHEAT_LEVEL level_d2_1

#define SAVE_CONTINUE 0
#define SAVE_QUIT 1
#define SAVE_CANCEL 2

#define ITEM_COLUMN 22
#define ITEM_NUM_COLUMN ITEM_COLUMN + 2
#define ITEMS_ROW 8

#define SUBSCREEN_NUM_ITEMS 5

#define SS_TEXT_X (SUBSCREEN_X + 1)
#define SS_TEXT_Y (SUBSCREEN_Y - 5) 

#define SUBSCREEN_INVENTORY 0
#define SUBSCREEN_ENEMIES 1
#define SUBSCREEN_MAP 2

extern LevelData CHEAT_LEVEL;

void writeSSTile(int x, int y, int tile);
void hideSubscreen();

Bg ssBg;
Bg topBg;
bool SubscreenShowing; 

bool g_isPaused;

int g_firstItemShowing;
int g_whichSubscreen;

void reshowSelectedSubscreen()
{
	previousSubscreen();
	nextSubscreen();
}


int whichSubscreen()
{
	return g_whichSubscreen;
}

void checkForCheats();

void cyclePauseScreen(int direction)
{
	switch (g_whichSubscreen)
	{
		case SUBSCREEN_INVENTORY:
			if (direction > 0)
			{
				selectNextItem();
			}
			else
			{
				selectPrevItem();
			}
			break;
		case SUBSCREEN_MAP:
			if (direction > 0)
			{
				incrementMapPin();
			}
			else
			{
				decrementMapPin();
			}
			break;
		case SUBSCREEN_ENEMIES:
			if (direction > 0)
			{
				nextEnemyDatabaseEntry();
			}
			else
			{
				prevEnemyDatabaseEntry();
			}
			break;
	}

}


void writeSSTile(int x, int y, int tile)
{
	ssBg.mapData[y*32 + x] = TILEWITHPAL(tile, HUD_PALETTE_NUM);	
}

void writeTopSSTile(int x, int y, int tile)
{
	topBg.mapData[y*32 + x] = TILEWITHPAL(tile, HUD_PALETTE_NUM);	
}


void pauseGame()
{
	g_isPaused = true;
	playGameSfx(SFX_SHOW_SUBSCREEN);
	saveBgPalette();
	forceMiniMapUpdate();
	int i = 0;
	if (isLevelOutdoors())
	{
		for (i = 0; i < 16; i++)
		{
			lightenBGPalette(i, -6,-6,-6);
		}
	}
	bool done = false;
	while(!done)
	{
		updateHud();
		updateSubscreen();
		updateSpriteDisplay();
		frameTick();
		setStatusMessage("Paused");
		if (isKeyPress(KEY_Y))
		{
			if (selectPrevItem())
			{
				playGameSfx(SFX_CHANGE_ITEM);	
			}
		}
		if (isKeyPress(KEY_X))
		{
			if (selectNextItem())
			{
				playGameSfx(SFX_CHANGE_ITEM);
			}
		}
		if (isKeyPress(KEY_LEFT))
		{
			playGameSfx(SFX_CHANGE_ITEM);
			cyclePauseScreen(-1);
		}
		if (isKeyPress(KEY_RIGHT))
		{
			playGameSfx(SFX_CHANGE_ITEM);
			cyclePauseScreen(1);
		}
		if (isKeyPress(KEY_L))
		{
			previousSubscreen();
		}
		if (isKeyPress(KEY_R))
		{
			nextSubscreen();
		}
		if (isKeyPress(KEY_START))
		{
			done = true;
		}
		checkForCheats();
	}
	playGameSfx(SFX_SHOW_SUBSCREEN);
	clearStatusMessage();
	if (isLevelOutdoors())
	{
		restoreBgPalette();
	}
	g_isPaused = false;
	forceMiniMapUpdate();
	updateSubscreen();

}

bool isPaused()
{
	return g_isPaused;
}

void writeSubTextDbase(int x, int y, char * text)
{
	writeSubText(DBASE_X + x + 1, DBASE_Y + y + 1, text);
}

void writeSubNumDbase(int x, int y, int num)
{
	writeSubNum(DBASE_X + x + 1, DBASE_Y + y + 1, num);
}

void writeSubTextSS(int x, int y, char * text)
{
	writeSubText(SUBSCREEN_X + x + 1, SUBSCREEN_Y + y + 1, text);
}

void writeSubNumSS(int x, int y, int num)
{
	writeSubNum(SUBSCREEN_X + x + 1, SUBSCREEN_Y + y + 1, num);
}

void writeTileSS(int x, int y, int tile)
{
	writeSubTile(SUBSCREEN_X + x + 1, SUBSCREEN_Y + y + 1, tile);
}

void clearTopMidScreen()
{
	Bg * layer1 = &topBg;
	zeroBGMap(layer1, 32 * 2);
}

void clearMidScreen()
{
	Bg * layer1 = &ssBg;
	zeroBGMap(layer1, 32*4);
}

void writeMidScreenTile(int x, int y, int tile)
{
	ssBg.mapData[y*32 + x] = TILEWITHPAL(tile, HUD_PALETTE_NUM);
}

void showTopMidScreen()
{
	enableBg(BG_MID);
}

void showMidScreen()
{
	enableSubBg(BG_MID);
}

void hideTopMidScreen()
{
	disableBg(BG_MID);
}

void hideMidScreen()
{
	disableSubBg(BG_MID);
}

void drawTopSubscreenFrame(int x1, int x2, int y1, int y2)
{
	Bg * layer1 = &topBg;
	initBackground(layer1, BG_MID_NUM);
	layer1->tileBlock = 2;
	layer1->mapBlock = 25;
	layer1->size = TEXTBG_SIZE_256x256;
	layer1->colorMode = BG_COLOR_16;
	layer1->isSub = false;
	//turn it on to get everything setup on it, then turn
	//it back off so it doesn't show up
	enableBackground((Bg *)&topBg);
	disableBG((Bg *)&topBg);


	clearTopMidScreen();


	int x;
	int y;
	for (x = x1 + 1; x < x2; x++)
	for (y = y1 + 1; y < y2; y++)
	{
		writeTopSSTile(x,y, TILE_BLACK);
	}
	x = x1;
	int xr = x2;
	for (y = y1 + 1; y < y2; y++)
	{
		writeTopSSTile(x, y, TILE_L_EDGE);	
		writeTopSSTile(xr, y, TILE_R_EDGE);	
	}
	y = y1;
	int yb = y2;
	for (x = x1 + 1; x < x2; x++)
	{
		writeTopSSTile(x, y, TILE_T_EDGE);	
		writeTopSSTile(x, yb, TILE_B_EDGE);	
	}
	writeTopSSTile(x1,      y1,      TILE_TL_CORNER);
	writeTopSSTile(x1 + 1,  y1,      TILE_TL_CORNER + 1);
	writeTopSSTile(x1,      y1 + 1,  TILE_TL_CORNER + 32);
	writeTopSSTile(x1 + 1,  y1 + 1,  TILE_TL_CORNER + 33);

	writeTopSSTile(x2,     y1,      TILE_TR_CORNER);
	writeTopSSTile(x2 - 1, y1,      TILE_TR_CORNER - 1);
	writeTopSSTile(x2,     y1 + 1,  TILE_TR_CORNER + 32);
	writeTopSSTile(x2 - 1, y1 + 1,  TILE_TR_CORNER + 31);

	writeTopSSTile(x1,      y2,     TILE_BL_CORNER);
	writeTopSSTile(x1 + 1,  y2,     TILE_BL_CORNER + 1);
	writeTopSSTile(x1,      y2 - 1, TILE_BL_CORNER - 32);
	writeTopSSTile(x1 + 1,  y2 - 1, TILE_BL_CORNER - 31);


	writeTopSSTile(x2,     y2,     TILE_BR_CORNER);
	writeTopSSTile(x2 - 1, y2,     TILE_BR_CORNER - 1);
	writeTopSSTile(x2,     y2 - 1, TILE_BR_CORNER - 32);
	writeTopSSTile(x2 - 1, y2 - 1, TILE_BR_CORNER - 33);
}

void drawSubscreenFrame(int x1, int x2, int y1, int y2)
{
	Bg * layer1 = &ssBg;
	initBackground(layer1, BG_MID_NUM);
	layer1->tileBlock = 2;
	layer1->mapBlock = 25;
	layer1->size = TEXTBG_SIZE_256x256;
	layer1->colorMode = BG_COLOR_16;
	layer1->isSub = true;
	//turn it on to get everything setup on it, then turn
	//it back off so it doesn't show up
	enableBackground((Bg *)&ssBg);
	disableBG((Bg *)&ssBg);

	clearMidScreen();


	int x;
	int y;
	for (x = x1 + 1; x < x2; x++)
	for (y = y1 + 1; y < y2; y++)
	{
		writeSSTile(x,y, TILE_BLACK);
	}
	x = x1;
	int xr = x2;
	for (y = y1 + 1; y < y2; y++)
	{
		writeSSTile(x, y, TILE_L_EDGE);	
		writeSSTile(xr, y, TILE_R_EDGE);	
	}
	y = y1;
	int yb = y2;
	for (x = x1 + 1; x < x2; x++)
	{
		writeSSTile(x, y, TILE_T_EDGE);	
		writeSSTile(x, yb, TILE_B_EDGE);	
	}
	writeSSTile(x1,      y1,      TILE_TL_CORNER);
	writeSSTile(x1 + 1,  y1,      TILE_TL_CORNER + 1);
	writeSSTile(x1,      y1 + 1,  TILE_TL_CORNER + 32);
	writeSSTile(x1 + 1,  y1 + 1,  TILE_TL_CORNER + 33);

	writeSSTile(x2,     y1,      TILE_TR_CORNER);
	writeSSTile(x2 - 1, y1,      TILE_TR_CORNER - 1);
	writeSSTile(x2,     y1 + 1,  TILE_TR_CORNER + 32);
	writeSSTile(x2 - 1, y1 + 1,  TILE_TR_CORNER + 31);

	writeSSTile(x1,      y2,     TILE_BL_CORNER);
	writeSSTile(x1 + 1,  y2,     TILE_BL_CORNER + 1);
	writeSSTile(x1,      y2 - 1, TILE_BL_CORNER - 32);
	writeSSTile(x1 + 1,  y2 - 1, TILE_BL_CORNER - 31);


	writeSSTile(x2,     y2,     TILE_BR_CORNER);
	writeSSTile(x2 - 1, y2,     TILE_BR_CORNER - 1);
	writeSSTile(x2,     y2 - 1, TILE_BR_CORNER - 32);
	writeSSTile(x2 - 1, y2 - 1, TILE_BR_CORNER - 33);
	layer1->x_scroll = 0;
	layer1->y_scroll = 0;
	updateBackground(layer1);
}

void initTopSubscreen()
{
	//assume at this point that text layer has already been loaded....
	Bg * layer1 = &topBg;
	initBackground(layer1, BG_MID_NUM);
	layer1->tileBlock = 2;
	layer1->mapBlock = 25;
	layer1->size = TEXTBG_SIZE_256x256;
	layer1->colorMode = BG_COLOR_16;
	layer1->isSub = false;
	//turn it on to get everything setup on it, then turn
	//it back off so it doesn't show up
	enableBackground((Bg *)&topBg);
	disableBG((Bg *)&topBg);


	clearTopMidScreen();
	
	g_firstItemShowing = max(0,itemToSubscreenIndex(getCurrentItem()) - (SUBSCREEN_NUM_ITEMS >> 1));
	
	drawTopSubscreenFrame(SUBSCREEN_X, SUBSCREEN_X2, SUBSCREEN_Y, SUBSCREEN_Y2);
	
}

void initSubscreen()
{
	//assume at this point that text layer has already been loaded....
	Bg * layer1 = &ssBg;
	initBackground(layer1, BG_MID_NUM);
	layer1->tileBlock = 2;
	layer1->mapBlock = 25;
	layer1->size = TEXTBG_SIZE_256x256;
	layer1->colorMode = BG_COLOR_16;
	layer1->isSub = true;
	//turn it on to get everything setup on it, then turn
	//it back off so it doesn't show up
	enableBackground((Bg *)&ssBg);
	disableBG((Bg *)&ssBg);

	clearMidScreen();
	
	g_firstItemShowing = max(0,itemToSubscreenIndex(getCurrentItem()) - (SUBSCREEN_NUM_ITEMS >> 1));
	
	drawSubscreenFrame(SUBSCREEN_X, SUBSCREEN_X2, SUBSCREEN_Y, SUBSCREEN_Y2);
	
}

void showCaret(int option)
{
	writeSubTextSS(1, 1, " ");
	writeSubTextSS(1, 3, " ");
	writeSubTextSS(1, 5, " ");
	writeTileSS(1, option * 2 + 1, CARET);
	
}

void showSaveSuccess()
{
	clearMidScreen();
	drawSubscreenFrame(SUBSCREEN_X, SUBSCREEN_X2, SUBSCREEN_Y, SUBSCREEN_Y + 6);
	playGameSfx(SFX_SHOW_SUBSCREEN);
	enableSubBg(BG_MID);
	writeSubTextSS(7,2,((char **)SaveMenuText)[3]);
	waitKeyPress();
	playGameSfx(SFX_HIDE_SUBSCREEN);
	hideSubscreen();
	clearSubText();
	
}

void showCannotSave()
{
	writeSubTextSS(4,2,((char **)SaveMenuText)[4]);
	writeSubTextSS(4,3,((char **)SaveMenuText)[5]);
	bool done = false;
	while (!done)
	{
		frameTick();
		if (isKeyPress(KEY_START) || isKeyPress(KEY_A) || isKeyPress(KEY_B) || isKeyPress(KEY_SELECT))
		{
			done = true;
		}
		updateSpriteDisplayUsingCustomSprites();
	}	

	playGameSfx(SFX_HIDE_SUBSCREEN);
	hideSubscreen();
	clearSubText();
	forceHudRedraw();


}


void showSaveScreen()
{
	saveBgPalette();
	int i = 0;
	for (i = 0; i < 16; i++)
	{
		lightenBGPalette(i, -6,-6,-6);
	}

	
	hideItemWindow();
	clearMidScreen();
	clearCustomSprites();
	clearSubText();

	drawSubscreenFrame(SUBSCREEN_X, SUBSCREEN_X2, SUBSCREEN_Y, SUBSCREEN_Y + 8);
	playGameSfx(SFX_SHOW_SUBSCREEN);
	enableSubBg(BG_MID);

	if ((!isLevelOutdoors()) && numEnemiesAlive() > 0)
	{

		showCannotSave();
		restoreBgPalette();
		forceMiniMapUpdate();
		showSubscreen();
		return;
	}
	
	int choice = SAVE_CONTINUE;
	
	writeSubTextSS(3,1,((char **)SaveMenuText)[0]);
	writeSubTextSS(3,3,((char **)SaveMenuText)[1]);
	writeSubTextSS(3,5,((char **)SaveMenuText)[2]);
	
	showCaret(choice);
	
	
	bool done = false;
	while(!done)
	{
		frameTick();
		if (isKeyPress( KEY_UP))
		{
			choice--;
			if (choice < SAVE_CONTINUE)
			{
				choice = SAVE_CANCEL;
			}
			showCaret(choice);
		}
		if (isKeyPress( KEY_DOWN))
		{
			choice++;
			if (choice > SAVE_CANCEL)
			{
				choice = SAVE_CONTINUE;
			}
			showCaret(choice);
		}
		if ((isKeyPress( KEY_SELECT)) || (isKeyPress( KEY_B)))
		{
			choice = SAVE_CANCEL;
			done = true;
		}
		if ((isKeyPress( KEY_START)) || (isKeyPress( KEY_A)))
		{
			done = true;
		}
		updateSpriteDisplayUsingCustomSprites();
	}
	
	playGameSfx(SFX_HIDE_SUBSCREEN);
	hideSubscreen();
	clearSubText();
	
	if (choice != SAVE_CANCEL)
	{
		if (saveGame(0))
		{
			showSaveSuccess();
		}

	}
	forceHudRedraw();
	if (choice == SAVE_QUIT)
	{

		initEnemies(0, null);
		initSpriteList();
		disableSprites();
		resetMainMenu();
		exitMainLoop();
	}
	showItemWindow();
	restoreBgPalette();
	forceMiniMapUpdate();
	reshowSelectedSubscreen();
	
}

void clearSSItems()
{
	clearCustomSprites();
}

bool isMapItem(int itemIndex)
{
	return ((itemIndex >= ITEM_MAP_DNGN_2) && (itemIndex <= ITEM_MAP_DNGN_4));
}



Sprite * showCustomSpriteWithScreen(int spriteNum, int tileNum, int x, int y, int pal, bool isSub)
{
	Sprite * sprite;
	if (isSub)
	{
		sprite = specialSprite(spriteNum);
	}
	else
	{
		sprite = specialTopSprite(spriteNum);
	}
	initSprite(sprite);
	setSpriteShape(sprite, SQUARE);
	setSpriteSize(sprite, SIZE_16);
	setSpriteBitDepth(sprite, BIT_DEPTH_16);
	setSpriteStartTile(sprite,tileNum);
	setSpritePosition(sprite, x, y);
	setSpritePriority(sprite, 0);
	setSpritePalette(sprite, pal);
	return sprite;
}

Sprite * showCustomSprite(int spriteNum, int tileNum, int x, int y, int pal)
{
	return showCustomSpriteWithScreen(spriteNum, tileNum, x, y, pal, true);
}


void showItemAtSSWithSpecial(int specialSpriteNum, int itemIndex, int x, int y, int pal, bool isSub)
{
	int startTile = 0;
	if (itemIndex < 15) {
		startTile = 128 + (itemIndex * 2) - 2;
	}
	else
	{
		if (isMapItem(itemIndex))
		{
			startTile = MAP_SPRITE_TILE;
		}
		else
		{
			startTile = 128 + ((itemIndex - 15) * 2) + 67;
		}
	}
	int subX = ((SUBSCREEN_X + x) << 3) + 4;
	int subY = ((SUBSCREEN_Y + y) << 3) + 4;
	showCustomSpriteWithScreen(specialSpriteNum, startTile, subX, subY, pal, isSub);
}

void showItemAtSS(int itemIndex, int x, int y, int pal)
{
	showItemAtSSWithSpecial(itemIndex, itemIndex, x, y, pal, true);
}

void showSelector(int itemSlot)
{
	int baseX;

	int i;
	//clear the other one...
	for(i = 0; i < 8; i++)
	{
		baseX = i * 4;
		writeTileSS(baseX + 1, ITEMS_ROW - 1 , 0);
		writeTileSS(baseX + 3, ITEMS_ROW - 1, 0);
		writeTileSS(baseX + 1, ITEMS_ROW + 1, 0);
		writeTileSS(baseX + 3, ITEMS_ROW + 1, 0);
	}

	baseX = itemSlot * 4;
	writeTileSS(baseX + 1, ITEMS_ROW - 1 , 210);
	writeTileSS(baseX + 3, ITEMS_ROW - 1, 211);
	writeTileSS(baseX + 1, ITEMS_ROW + 1, 242);
	writeTileSS(baseX + 3, ITEMS_ROW + 1, 243);
}

void showItemSelector()
{
	int i;
	int iCtr = 0;
	int current = getCurrentItem();
	for(i = 0; i < NUM_ITEMS; i++)
	{
		if (getItemQty(i) > 0)
		{
			Item * item = getItemDef(i);
			if (item->useFunction != null)
			{
				if (current == i)
				{
					showSelector(iCtr);
				}
				iCtr++;

			}
		}
	}

	
}

void subscreenSwitchItemPrev()
{
	if (selectPrevItem())
	{
		showItemSelector();
		playGameSfx(SFX_CHANGE_ITEM);
	
	}
}

void subscreenSwitchItemNext()
{
	if (selectNextItem())
	{
		showItemSelector();
		playGameSfx(SFX_CHANGE_ITEM);
	}
}


void drawPortrait(u16* portraitPal, u16* portraitGfx, u16* portraitMap)
{
	loadBGPaletteLine(portraitPal, ENEMY_DB_PALETTE_NUM);

	Bg tmpBg;
	tmpBg.tileData = topBg.tileData + PORTRAIT_TILES_OFFSET;
	loadBGTiles(&tmpBg, (u32*)portraitGfx, 144);

	int x;
	int y;
	u16 * map = portraitMap;
	for (y = (SCREEN_BLOCK_HEIGHT-PORTRAIT_HEIGHT); y <= SCREEN_BLOCK_HEIGHT; y++)
	for (x = 0; x < PORTRAIT_WIDTH; x++)
	{
		writeTile(x,y,TILEWITHPAL((*map++) + PORTRAIT_TILE_START, ENEMY_DB_PALETTE_NUM)); 
	}
}

void drawSubPortrait(u16* portraitPal, u16* portraitGfx, u16* portraitMap)
{
	loadSubBGPaletteLine(portraitPal, ENEMY_DB_PALETTE_NUM);

	Bg tmpBg;
	tmpBg.tileData = ssBg.tileData + PORTRAIT_TILES_OFFSET;
	loadBGTiles(&tmpBg, (u32*)portraitGfx, 144);

	int x;
	int y;
	u16 * map = portraitMap;
	for (y = (SCREEN_BLOCK_HEIGHT-PORTRAIT_HEIGHT); y <= SCREEN_BLOCK_HEIGHT; y++)
	for (x = 0; x < PORTRAIT_WIDTH; x++)
	{
		writeSubTile(x,y,TILEWITHPAL((*map++) + PORTRAIT_TILE_START, ENEMY_DB_PALETTE_NUM)); 
	}
}

void drawEnemyPortrait(const EnemyDatabaseDef * def)
{
	drawSubPortrait((u16*)def->portraitPal, (u16*)def->portraitGfx, (u16*)def->portraitMap);
}

void clearTopPortrait()
{
	Bg tmpBg;
	tmpBg.tileData = topBg.tileData + PORTRAIT_TILES_OFFSET;
	int x;
	int y;
	u16 * dest = &(textLayer->mapData[(SCREEN_BLOCK_HEIGHT-PORTRAIT_HEIGHT) * 32]);
	for (y = (SCREEN_BLOCK_HEIGHT-PORTRAIT_HEIGHT); y <= SCREEN_BLOCK_HEIGHT; y++)
	{
		for (x = 0; x < PORTRAIT_WIDTH; x++)
		{
			*dest++ = 0;
		}
		dest += (32 - PORTRAIT_WIDTH);
	}
}

void clearEnemyPortrait()
{
	return;
	Bg tmpBg;
	tmpBg.tileData = ssBg.tileData + PORTRAIT_TILES_OFFSET;
	int x;
	int y;
	u16 * dest = &(textLayer->mapData[(20-PORTRAIT_HEIGHT) * 32]);
	for (y = (20-PORTRAIT_HEIGHT); y < 21; y++)
	{
		for (x = 0; x < PORTRAIT_WIDTH; x++)
		{
			*dest++ = 0;
		}
		dest += (32 - PORTRAIT_WIDTH);
	}
}


//NLT -- fix for sub
void showNewEnemyAdded(const EnemyDatabaseDef * def)
{
	/*
	char nameBuffer[32];

	char * buffIter = nameBuffer;

	int i;
	for (i = 0; i < PORTRAIT_WIDTH - 1 ; i++)
	{
		*buffIter++ = ' ';
	}

	char * nameIter = def->name;
	while (*nameIter != '\0')
	{
		*buffIter++ = *nameIter++;
		i++;
	}

	for (; i < 26; i++)
	{
		*buffIter++ = ' ';
	
	}

	*buffIter = '\0';

	setStatusMessage(nameBuffer);
	drawEnemyPortrait(def);
	*/

}

void showNewEnemyAddedFreeze(const EnemyDatabaseDef * def)
{
	drawSubscreenFrame(ENEMY_ADD_X, ENEMY_ADD_X2, ENEMY_ADD_Y, ENEMY_ADD_Y2);
	enableBg(BG_MID);
	drawEnemyPortrait(def);
	writeSubText(ENEMY_ADD_X + 2, ENEMY_ADD_Y + 2, def->name);
	int offsetX = ENEMY_ADD_X + 3 + strlen(def->name);
	writeSubText(offsetX, ENEMY_ADD_Y + 2, "added");
	writeSubText(ENEMY_ADD_X + 2, ENEMY_ADD_Y + 3, "to enemy database");

	clearStatusMessageTimer();
	int timer = 30;
	waitKeyPressOrTimeout(timer);

	clearEnemyPortrait();
	writeSubText(ENEMY_ADD_X + 2, ENEMY_ADD_Y + 2, "                     ");
	writeSubText(ENEMY_ADD_X + 2, ENEMY_ADD_Y + 3, "                  ");
	clearMidScreen();
	hideMidScreen();
}



void showEnemyStatsFromDatabase()
{
	EnemyType * type = getCurrentEnemyDatabaseEntry();

	Enemy * enemy = getLastHitEnemy();

	if (type == null)
	{
		//writeLinesSubPos(DBASE_X + 2, DBASE_Y + 2, TEXT_LINES(EmptyEnemyDatabaseText));
		return;
	}

	clearSubTextBottomHalf();

	const EnemyDatabaseDef * def = type->databaseDef;

	writeSubTextDbase(1, 1, "                ");
	writeSubTextDbase(1, 1, def->name);

	int hp = 0;
	if (enemy != null)
	{
		hp = enemy->hp;
	}
	writeSubTextDbase(1, 3, "HP:           ");
	writeSubNumDbase(5, 3, hp);
	writeSubTextDbase(8, 3, "/");
	int hpMaxX = 9;
	int hpMax = type->startHP;
	if (hpMax < 100)
	{
		hpMaxX++;
	}
	writeSubNumDbase(hpMaxX, 3, type->startHP);

	writeSubTextDbase(14, 3, "Damage:        ");
	writeSubNumDbase(22, 3, type->collideDamage);

	writeSubTextDbase(1,5,"                           ");
	writeSubTextDbase(1,7,"                           ");
	writeSubTextDbase(1,9,"                           ");
	writeSubTextDbase(1,11,"                           ");

	drawEnemyPortrait(def);


	if (def->description1 != null)
	{
		writeSubTextDbase(1, 5, def->description1);
	}


	if (def->description2 != null)
	{
		writeSubTextDbase(def->desc2X + 1, 7, def->description2);
	}

	if (def->description3 != null)
	{
		writeSubTextDbase(def->desc3X + 1, 9, def->description3);
	}

	if (def->description4 != null)
	{
		writeSubTextDbase(def->desc4X + 1, 11, def->description4);
	}


	
}



#define DB_L_SPACE 14
void drawDbControls()
{
	writeSubText(DBASE_X + DB_L_SPACE, DBASE_Y2 - 2, "Map");
	writeSubText(DBASE_X2 - 8 , DBASE_Y2 - 2, "Items");
	

	writeSubTileWithPal(DBASE_X + DB_L_SPACE - 2, DBASE_Y2 - 2, L_SHOULDER_TILE, BUTTON_PAL);
	writeSubTileWithPal(DBASE_X + DB_L_SPACE - 1, DBASE_Y2 - 2, L_SHOULDER_TILE + 1, BUTTON_PAL);
	writeSubTileWithPal(DBASE_X + DB_L_SPACE - 2, DBASE_Y2 - 1, L_SHOULDER_TILE + 32, BUTTON_PAL);
	writeSubTileWithPal(DBASE_X + DB_L_SPACE - 1, DBASE_Y2 - 1, L_SHOULDER_TILE + 33, BUTTON_PAL);

	writeSubTileWithPal(DBASE_X2 - 3 , DBASE_Y2 - 2, R_SHOULDER_TILE, BUTTON_PAL);
	writeSubTileWithPal(DBASE_X2 - 2 , DBASE_Y2 - 2, R_SHOULDER_TILE + 1, BUTTON_PAL);
	writeSubTileWithPal(DBASE_X2 - 3 , DBASE_Y2 - 1, R_SHOULDER_TILE + 32, BUTTON_PAL);
	writeSubTileWithPal(DBASE_X2 - 2 , DBASE_Y2 - 1, R_SHOULDER_TILE + 33, BUTTON_PAL);
	
}


void updateSubscreenEnemies()
{
	bool needsUpdate = getAndClearEnemyDBNeedsUpdate();
	if (needsUpdate)
	{
		showEnemyStatsFromDatabase();
		drawDbControls();
	}
}


void showEnemyDatabase()
{
	g_whichSubscreen = SUBSCREEN_ENEMIES;

	playGameSfx(SFX_SHOW_SUBSCREEN);

	clearSubText();
	clearMidScreen();
	clearCustomSprites();
	drawSubscreenFrame(DBASE_X, DBASE_X2, DBASE_Y, DBASE_Y2);
	enableSubBg(BG_MID);
	
	if (getCurrentEnemyDatabaseEntry() == null)
	{
		nextEnemyDatabaseEntry();
	}

	showEnemyStatsFromDatabase();
	drawDbControls();


	
	
}

bool isSubscreenShowing()
{
	return SubscreenShowing;	
}


void showSubscreen()
{
	g_whichSubscreen = SUBSCREEN_INVENTORY;
	SubscreenShowing = true;
	initSubscreen();
	playGameSfx(SFX_SHOW_SUBSCREEN);
	enableSubBg(BG_MID);
	clearSubText();
	forceHudRedraw();
	showLife();


	//show stats
	writeSubTextSS(SS_TEXT_X,     SS_TEXT_Y, "HP:    /");
	writeSubTextSS(SS_TEXT_X,     SS_TEXT_Y + 1, "Attack:        ");
	writeSubTextSS(SS_TEXT_X,     SS_TEXT_Y + 2, "Defense:");


	writeSubText(SUBSCREEN_X + 4, SUBSCREEN_Y2 - 2, "Enemy");
	writeSubText(SUBSCREEN_X + 4, SUBSCREEN_Y2 - 1, "List");
	writeSubText(SUBSCREEN_X2 - 6 , SUBSCREEN_Y2 - 2, "Map");

	writeSubTileWithPal(SUBSCREEN_X + 2, SUBSCREEN_Y2 - 2, L_SHOULDER_TILE, BUTTON_PAL);
	writeSubTileWithPal(SUBSCREEN_X + 3, SUBSCREEN_Y2 - 2, L_SHOULDER_TILE + 1, BUTTON_PAL);
	writeSubTileWithPal(SUBSCREEN_X + 2, SUBSCREEN_Y2 - 1, L_SHOULDER_TILE + 32, BUTTON_PAL);
	writeSubTileWithPal(SUBSCREEN_X + 3, SUBSCREEN_Y2 - 1, L_SHOULDER_TILE + 33, BUTTON_PAL);

	writeSubTileWithPal(SUBSCREEN_X2 - 3 , SUBSCREEN_Y2 - 2, R_SHOULDER_TILE, BUTTON_PAL);
	writeSubTileWithPal(SUBSCREEN_X2 - 2 , SUBSCREEN_Y2 - 2, R_SHOULDER_TILE + 1, BUTTON_PAL);
	writeSubTileWithPal(SUBSCREEN_X2 - 3 , SUBSCREEN_Y2 - 1, R_SHOULDER_TILE + 32, BUTTON_PAL);
	writeSubTileWithPal(SUBSCREEN_X2 - 2 , SUBSCREEN_Y2 - 1, R_SHOULDER_TILE + 33, BUTTON_PAL);


	updateSubscreen();

}




void updateSubscreenInventory()
{

	//updatable things
	writeSubNumSS (SS_TEXT_X + 4, SS_TEXT_Y, getMainCharHP());
	writeSubNumSS (SS_TEXT_X + 9, SS_TEXT_Y, getMainCharMaxHP());
	writeSubNumSS (SS_TEXT_X + 9, SS_TEXT_Y + 1, getMainCharPower());
	writeSubNumSS (SS_TEXT_X + 9, SS_TEXT_Y + 2, getMainCharArmor() + 1); //starts at 0, show it starting at 1

	clearSSItems();

	//show set items
	showItemAtSS(ITEM_GOLD, SS_TEXT_X + 1, SS_TEXT_Y + 4, SUB_ITEMS_PAL);
	showItemAtSS(ITEM_KEY_RED, ITEM_COLUMN, 1, SUB_LOCKS_PAL);
	showItemAtSS(ITEM_KEY_YELLOW, ITEM_COLUMN, 3, SUB_LOCKS_PAL);
	showItemAtSS(ITEM_KEY_BLUE, ITEM_COLUMN, 5, SUB_LOCKS_PAL);

	writeSubTextSS(SS_TEXT_X + 3, SS_TEXT_Y + 4, "    ");
	writeSubNumSS(SS_TEXT_X + 3, SS_TEXT_Y + 4, getMainCharGold());
	writeSubNumSS(ITEM_NUM_COLUMN,1,mainCharInventory(ITEM_KEY_RED));
	writeSubNumSS(ITEM_NUM_COLUMN,3,mainCharInventory(ITEM_KEY_YELLOW));
	writeSubNumSS(ITEM_NUM_COLUMN,5,mainCharInventory(ITEM_KEY_BLUE));

	
	int i;
	int iCtr = 0;
	for(i = 0; i < NUM_ITEMS; i++)
	{
		if (getItemQty(i) > 0)
		{
			Item * item = getItemDef(i);
			if (item->useFunction != null)
			{
				int baseX = iCtr * 4;
				showItemAtSS(i, baseX + 2, ITEMS_ROW, SUB_ITEMS_PAL);
				int qty = getItemUsableQty(i);
				if (qty != ITEM_NO_COUNT)
				{
					if (qty > 99)
					{
						writeSubNumSS(baseX + 1, ITEMS_ROW + 2, qty);
					}
					else
					{
						writeSubNumSS(baseX + 2, ITEMS_ROW + 2, qty);
					}
				}
				else
				{
					writeSubTextSS(baseX + 1, ITEMS_ROW + 2, "    ");
				}
				iCtr++;

			}
		}
	}
	while (iCtr < 7)
	{
		int baseX = iCtr * 4;
		writeSubTextSS(baseX + 1, ITEMS_ROW + 2, "    ");
		iCtr++;
	}



	showItemSelector();
}



void hideSubscreen()
{
	disableSubBg(BG_MID);
	deactivateSubscreenWindow();
}

void hideTopSubscreen()
{
	disableBg(BG_MID);
	deactivateSubscreenWindow();
}


const int cheat_all_items[] = {KEY_UP, KEY_UP, 
							   KEY_DOWN, KEY_DOWN, 
							   KEY_LEFT, KEY_RIGHT,
							   KEY_LEFT, KEY_RIGHT,
							   KEY_B,    KEY_A};  //THE code
								
					
//const int cheat_level[] = {KEY_A,KEY_B,KEY_UP,KEY_DOWN};

const int cheat_enemies[] = {KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B, KEY_B };

const int cheat_gold[] = {KEY_A, KEY_B, KEY_B, KEY_A};

int cheatState;
int cheatTimer;
int cheatLevelState;
int cheatEnemiesState;
int cheatGoldState;

void checkForCheats()
{
#ifdef NOCHEATS
	return;
#else

	if(isKeyDown(cheat_all_items[cheatState]))
	{//if you pressed the next button, move ahead on the array
	   cheatState++;
	   cheatTimer = 30; //max number of game cycles to allow the user to press the next button in the sequence
	}

	if (isKeyDown(cheat_enemies[cheatEnemiesState]))
	{
	   cheatEnemiesState++;
	   cheatTimer = 30; //max number of game cycles to allow the user to press the next button in the sequence
	}

	/*
	if (KEY_DOWN(cheat_level[cheatLevelState]))
	{
	   cheatLevelState++;
	   cheatTimer = 30; //max number of game cycles to allow the user to press the next button in the sequence
	}
	*/

	if (isKeyDown(cheat_gold[cheatGoldState]))
	{
	   cheatGoldState++;
	   cheatTimer = 30; //max number of game cycles to allow the user to press the next button in the sequence
	}

	if (cheatTimer == 0) {
		cheatState=0; //reset cheat code
		cheatLevelState = 0;
	}

	if ((cheatState != 0) || (cheatLevelState != 0) || (cheatGoldState != 0) || (cheatEnemiesState != 0)) cheatTimer--;

	if (cheatLevelState >= 4)
	{
		cheatTimer = 0;
		cheatState = 0;
		setLevel(&CHEAT_LEVEL, 50,50);
	}

	if (cheatGoldState >= 4)
	{
		cheatTimer = 0;
		cheatGoldState = 0;
		addMainCharGold(155);
	}

	if (cheatEnemiesState >= 33)
	{
		cheatTimer = 0;
		cheatEnemiesState = 0;
		addAllEnemies();
	}

	if(cheatState >= 10) 
	{
		cheatTimer = 0;
		cheatState = 0;
		writeSubText(3,3,"Got all items");
		getAllItems();
		int i;
		for (i = 0; i < 16; i++)
		{
			mainCharLifeUp();
		}
		for (i = 0; i < 128; i++)
		{
			mainCharDefenceUp();
			mainCharAttackUp();
			mainCharMeatPickup();
			//miniMapBeenEverywhere();
		}
	}
#endif
}

void updateSubscreenMap()
{
	updateDrawMiniMap();
}

void showMiniMap()
{
	g_whichSubscreen = SUBSCREEN_MAP;

	clearSubText();
	clearCustomSprites();
	clearMidScreen();

	drawSubscreenFrame(MAP_X, MAP_X2, MAP_Y, MAP_Y2);
	enableSubBg(BG_MID);
	//activateSubscreenWindow();
	

	drawMiniMap();
	
	
	writeSubText(MAP_X + 3 , MAP_Y2 - 2, "Items");
	writeSubText(MAP_X2 - 7, MAP_Y2 - 2, "Enemy");
	writeSubText(MAP_X2 - 7, MAP_Y2 - 1, "List");
	
	writeSubTileWithPal(MAP_X + 1, MAP_Y2 - 2, L_SHOULDER_TILE, BUTTON_PAL);
	writeSubTileWithPal(MAP_X + 2, MAP_Y2 - 2, L_SHOULDER_TILE + 1, BUTTON_PAL);
	writeSubTileWithPal(MAP_X + 1, MAP_Y2 - 1, L_SHOULDER_TILE + 32, BUTTON_PAL);
	writeSubTileWithPal(MAP_X + 2, MAP_Y2 - 1, L_SHOULDER_TILE + 33, BUTTON_PAL);

	writeSubTileWithPal(MAP_X2 - 2 , MAP_Y2 - 2, R_SHOULDER_TILE, BUTTON_PAL);
	writeSubTileWithPal(MAP_X2 - 1 , MAP_Y2 - 2, R_SHOULDER_TILE + 1, BUTTON_PAL);
	writeSubTileWithPal(MAP_X2 - 2 , MAP_Y2 - 1, R_SHOULDER_TILE + 32, BUTTON_PAL);
	writeSubTileWithPal(MAP_X2 - 1 , MAP_Y2 - 1, R_SHOULDER_TILE + 33, BUTTON_PAL);

}

	
void loadTilesToPortraitSpace(u16 * tileData, int numTiles)
{
	Bg tmpBg;
	tmpBg.tileData = ssBg.tileData + PORTRAIT_TILES_OFFSET;
	loadBGTiles(&tmpBg, (u32*)tileData, numTiles);
}

void previousSubscreen()
{
	switch (g_whichSubscreen)
	{
		case SUBSCREEN_INVENTORY:
			showEnemyDatabase();
			break;
		case SUBSCREEN_MAP:
			showSubscreen();
			break;
		case SUBSCREEN_ENEMIES:
			showMiniMap();
			break;
	}
}

void nextSubscreen()
{
	switch (g_whichSubscreen)
	{
		case SUBSCREEN_INVENTORY:
			showMiniMap();
			break;
		case SUBSCREEN_MAP:
			showEnemyDatabase();
			break;
		case SUBSCREEN_ENEMIES:
			showSubscreen();
			break;
	}
}

void updateSubscreen()
{
	switch (g_whichSubscreen)
	{
		case SUBSCREEN_INVENTORY:
			updateSubscreenInventory();
			break;
		case SUBSCREEN_MAP:
			updateSubscreenMap();
			break;
		case SUBSCREEN_ENEMIES:
			updateSubscreenEnemies();
			break;
	}

}
