#ifndef _SUBSCREEN_H_
#define _SUBSCREEN_H_

#include "enemies.h"


void showSubscreen();
void hideSubscreen();
void initSubscreen();
void updateSubscreen();

void showSaveScreen();
void showMiniMap();

void nextSubscreen();
void previousSubscreen();


void pauseGame();
void clearMidScreen();
void writeMidScreenTile(int x, int y, int tile);
void showMidScreen();
void hideMidScreen();
void hideTopSubscreen();
void showNewEnemyAdded(const EnemyDatabaseDef * def);
void drawPortrait(u16* portraitPal, u16* portraitGfx, u16* portraitMap);
void clearEnemyPortrait();
void clearTopPortrait();
bool isSubscreenShowing();
void initSubscreen();
void initTopSubscreen();
void loadTilesToPortraitSpace(u16 * tileData, int numTiles);
Sprite * showCustomSprite(int spriteNum, int tileNum, int x, int y, PaletteId pal);

bool isPaused();

//this is really more of an internal thing, but
//the shop module uses it to avoid duplicating code...
//probably should be pulled out into some sort of 
//"UI panel" module instead.....
void showItemAtSS(int itemIndex, int x, int y, PaletteId pal);
void showItemAtSSWithSpecial(int specialSpriteNum, int itemIndex, int x, int y, PaletteId pal, bool isSub);
void clearSSItems();

#define TILE_BLACK 226
#define TILE_L_EDGE 224
#define TILE_R_EDGE 229 
#define TILE_T_EDGE 194
#define TILE_B_EDGE 290
#define TILE_TL_CORNER 192
#define TILE_TR_CORNER 197
#define TILE_BL_CORNER 288
#define TILE_BR_CORNER 293

#define L_SHOULDER_TILE 188
#define R_SHOULDER_TILE 190

#define SUBSCREEN_X 1
#define SUBSCREEN_Y 6
#define SUBSCREEN_WIDTH 29
#define SUBSCREEN_HEIGHT 16
#define SUBSCREEN_X2  (SUBSCREEN_X + SUBSCREEN_WIDTH)
#define SUBSCREEN_Y2  (SUBSCREEN_Y + SUBSCREEN_HEIGHT)

#define DBASE_X 1
#define DBASE_Y 7
#define DBASE_WIDTH 27
#define DBASE_HEIGHT 16
#define DBASE_X2  (DBASE_X + DBASE_WIDTH)
#define DBASE_Y2  (DBASE_Y + DBASE_HEIGHT)

#define MAP_X 4
#define MAP_Y 8
#define MAP_WIDTH 24
#define MAP_HEIGHT 12
#define MAP_X2 (MAP_X + MAP_WIDTH)
#define MAP_Y2 (MAP_Y + MAP_HEIGHT)

#define ENEMY_ADD_X 5
#define ENEMY_ADD_X2 26
#define ENEMY_ADD_Y 5
#define ENEMY_ADD_Y2 10

#define CARET 82

#define U32_PER_TILE 8 
#define PORTRAIT_TILE_START 320
#define PORTRAIT_TILES_OFFSET (PORTRAIT_TILE_START * U32_PER_TILE)

#endif
