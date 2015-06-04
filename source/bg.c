#include "bg.h"
#include "misc.h"
#include "memcpy.h"
//#include "bios.h"


extern unsigned short fgMapData[];
extern unsigned short bgMapData[];

u16 paletteSave[256];

TilePaletteApplierFunction bgPalApplier;
TilePaletteApplierFunction fgPalApplier;

int getTileIndex(int x, int y)
{
	if (y < 0)
	{
		y += 32;
	}
	if (x < 0)
	{
		x += 64;
	}
	if (x > 63)
	{
		x -= 63;
	}
	if (x > 31)
	{
		return 1024 + (y % 32) * 32 + (x % 32);
	}
    return (y % 32) * 32 + (x % 32);
}

void loadBGPalette(u16 * palette) {
	int count = 256;
	u16 *dest= BG_PALETTE;
	do
		*dest++ = *palette++;
	while (--count);
}

void loadSubBGPalette(u16 * palette) {
	int count = 256;
	u16 *dest= BG_PALETTE_SUB;
	do
		*dest++ = *palette++;
	while (--count);
}

u16 * getBgPaletteMem() {
	return BG_PALETTE;
}

u16 * getSubBgPaletteMem() {
	return BG_PALETTE_SUB;
}

void saveBgPalette()
{
	copyMem16(paletteSave, BG_PALETTE, 256);
}

void restoreBgPalette()
{
	copyMem16(BG_PALETTE, paletteSave, 256);
}

void lightenBGPaletteByMem(u16 * bgPal, int r, int g, int b) {
	int i;
	for (i = 0; i < 16; i++) {
		u16 pal = *bgPal;
		int blue = (pal & (0x1F << 10)) >> 10;
		int green = (pal & (0x1F << 5)) >> 5;
		int red = pal & 0x1F;

		blue += b;
		green += g;
		red += r;

		red = min(red, 31);
		green = min(green, 31);
		blue = min(blue, 31);

		red = max(red, 0);
		green = max(green, 0);
		blue = max(blue, 0);

		*bgPal = ((red)+(green << 5) + (blue << 10));

		bgPal++;
	}

}

void lightenBGPalette(int paletteNum, int r, int g, int b) {
	u16 * bgPal= BG_PALETTE;
	bgPal += (paletteNum * 16);
	lightenBGPaletteByMem(bgPal, r, g, b);
}

void lightenSubBGPalette(int paletteNum, int r, int g, int b) {
	u16 * bgPal= BG_PALETTE_SUB;
	bgPal += (paletteNum * 16);
	lightenBGPaletteByMem(bgPal, r, g, b);
}

void loadBGPaletteLine(u16 * palette, int paletteNum) {
	u16 * bgPal= BG_PALETTE;
	bgPal += (paletteNum * 16);
	copyMem16(bgPal, palette, 16);
}

void loadBlankBGPaletteLine(int paletteNum) {
	u16 * bgPal = BG_PALETTE;
	bgPal += (paletteNum * 16);
	int i = 0;
	for (i = 0; i < 16; i++)
	{
		*bgPal++ = 0;
	}
}

void loadSubBGPaletteLine(u16 * palette, int paletteNum) {
	u16 * bgPal= BG_PALETTE_SUB;
	bgPal += (paletteNum * 16);
	copyMem16(bgPal, palette, 16);
}

void setPaletteAppliers(TilePaletteApplierFunction bgApp,
		TilePaletteApplierFunction fgApp) {
	bgPalApplier = bgApp;
	fgPalApplier = fgApp;
}

u16 screenBuff[2048];
u16 screenBuffFG[2048];
	
void initScreen(Bg * bg, LevelData * level, int x, int y, Bg * bgFG) {

	int width = level->width;
	int height = level->height;
	int yIterEnd = y + 30;
	int xIterEnd = x + 42;
	int xIter,yIter;


	for (yIter = y - 1; yIter < yIterEnd; yIter++)
	{
		int yComponent = yIter * width;
		for (xIter = x - 2; xIter < xIterEnd; xIter++)
		{
			int mapPos = yComponent + xIter;
			int bgPos = getTileIndex(xIter & 63, yIter & 31);
			u16 tile = 0;
			int fgTile = -1;
			if ((yIter < height) && (xIter < width) && (yIter >= 0) && (xIter >= 0))
			{
				tile = bgMapData[mapPos];
				fgTile = fgMapData[mapPos];
			}
			screenBuff[bgPos] = bgPalApplier(level, tile);
			screenBuffFG[bgPos] = fgPalApplier(level, fgTile);

		}
		
	}


	//TICK - frameTick();
	/////improved take 1 (working)
	/*
	int i, k;
	int width = level->width;
	int kEnd = y + 32;
	int iEnd = x + 32;
	int kWidth = (y - 2) * width;
	int perTick = 7;
	int tickTimer = 0;
	for (k = y - 2; k < kEnd - 1; k++) {
		int mapY = (k & 31) * 32;
		int mapPos = kWidth + x - 2;
		for (i = x - 2; i < iEnd - 1; i++) {
			int mapX = i & 31;
			int pos = mapX + mapY;
			u16 tile = bgMapData[mapPos];
			screenBuff[pos] = bgPalApplier(level, tile);
			screenBuffFG[pos] = fgPalApplier(level, fgMapData[mapPos]);
			mapPos++;
		}
		kWidth += width;
		if (tickTimer++ > perTick) {
			//TICK - frameTick();
			tickTimer = 0;
		}
	}
	*/

	initEnemies(0, null);
	sortSpriteList();
	//TICK - frameTick();
	updateSpriteDisplay();

	disableBg(BG_FOREGROUND);
	disableBg(BG_MAIN);

	copyMem16(bg->mapData, screenBuff, 2048);
	updateMaps();
	//TICK - frameTick();
	copyMem16(bgFG->mapData, screenBuffFG, 2048);
	updateMaps();

}

void loadBGTiles(Bg * bg, u32* gfx, int numTiles) {
	if (!bg) {
		return;
	}
	copyMem32(bg->tileData, gfx, (numTiles*8));
}

void zeroBGMap(Bg * bg, int mapSize) {
	void * dest = bg->mapData;
	mapSize = mapSize * mapSize >> 2;
	fillMem16(0, dest, mapSize);
}

bool initBackground(Bg* bg, int layer) {
	if (!bg) {
		return false;
	}
	bg->number = layer; //background number 0-3
	int tmp = layer - 1;
	if (tmp < 0)
	{
		tmp = 0;
	}
	bg->priority = tmp;
	//if (layer == 1) {
		//bg->priority = 0;
	//}
	bg->tileBlock = 0;
	bg->mapBlock = 28;
	bg->colorMode = BG_COLOR_256;
	bg->size = TEXTBG_SIZE_256x256; //size of map
	bg->mosaic = 0; //not enabled
	bg->x_scroll = 0; //scrolling variables
	bg->y_scroll = 0;
	bg->isSub = false;
	bg->foreground = null;

	return true;
}

//u32 * mapDataForBG(int bgNum)
//{
//	return (u16 *)ScreenBaseBlock(bgNum);
//}

void enableBackground(Bg* bg) {

	vuint16* bgControllers[] = { &BG0_CR, &BG1_CR, &BG2_CR, &BG3_CR,
			&SUB_BG0_CR, &SUB_BG1_CR, &SUB_BG2_CR, &SUB_BG3_CR };

	if (bg->isSub) {
		bg->tileData = (u32*)BG_TILE_RAM_SUB(bg->tileBlock);
		bg->mapData = (u16*)BG_MAP_RAM_SUB(bg->mapBlock);
	} else {
		bg->tileData = (u32*)BG_TILE_RAM(bg->tileBlock);
		bg->mapData = (u16*)BG_MAP_RAM(bg->mapBlock);
	}

	u16 temp= BG_TILE_BASE(bg->tileBlock) | BG_MAP_BASE(bg->mapBlock)
			| bg->size | bg->colorMode | bg->priority;// | bg->mosaic | bg->priority;

	int bgRegisterNum = bg->number + (bg->isSub ? 4 : 0);

	*(bgControllers[bgRegisterNum]) = temp;
	
	u32 bgActives[] = { DISPLAY_BG0_ACTIVE, DISPLAY_BG1_ACTIVE,
			DISPLAY_BG2_ACTIVE, DISPLAY_BG3_ACTIVE, 
    };
	

    vuint32* reg = &DISPLAY_CR;
    if (bg->isSub)
    reg = &SUB_DISPLAY_CR;
    
    u32 value = bgActives[bg->number];
    
    *reg |= value;

}

void disableBG(Bg * bg) {
	u32 bgActives[] = { DISPLAY_BG0_ACTIVE, DISPLAY_BG1_ACTIVE,
			DISPLAY_BG2_ACTIVE, DISPLAY_BG3_ACTIVE, 
    };

    vuint32* reg = &DISPLAY_CR;
    if (bg->isSub)
    reg = &SUB_DISPLAY_CR;
    
    u32 value = bgActives[bg->number];
    
    *reg &= ~value;
    
}

uint32 dispCntHolder;

void hideBgs() {
	dispCntHolder = DISPLAY_CR;

	DISPLAY_CR &= ~DISPLAY_BG0_ACTIVE;
	DISPLAY_CR &= ~DISPLAY_BG1_ACTIVE;
	DISPLAY_CR &= ~DISPLAY_BG2_ACTIVE;
	DISPLAY_CR &= ~DISPLAY_BG3_ACTIVE;

}

void restoreBgs() {
	DISPLAY_CR = dispCntHolder;
}

void updateBackground(Bg* bg) {
	if (bg->x_scroll > 512) {
		bg->x_scroll -= 256;
	}
	if (bg->y_scroll > 256) {
		bg->y_scroll -= 256;
	}
	if (bg->x_scroll < 0) {
		bg->x_scroll += 512;
	}
	if (bg->y_scroll < 0) {
		bg->y_scroll += 256;
	}

	vuint16* bgXControllers[] = { &BG0_X0, &BG1_X0, &BG2_X0, &BG3_X0,
			&SUB_BG0_X0, &SUB_BG1_X0, &SUB_BG2_X0, &SUB_BG3_X0 };
	vuint16* bgYControllers[] = { &BG0_Y0, &BG1_Y0, &BG2_Y0, &BG3_Y0,
			&SUB_BG0_Y0, &SUB_BG1_Y0, &SUB_BG2_Y0, &SUB_BG3_Y0 };

	int bgRegisterNum = bg->number + (bg->isSub ? 4 : 0);

	vuint16* xReg = bgXControllers[bgRegisterNum];
	vuint16* yReg = bgYControllers[bgRegisterNum];

	*xReg = bg->x_scroll;
	*yReg = bg->y_scroll;

	if (bg->foreground != null)
	{
		bg->foreground->x_scroll = bg->x_scroll;
		bg->foreground->y_scroll = bg->y_scroll;
		updateBackground(bg->foreground);
	}

}

// Scrolls a background up by loading a new top row
u16 ScrollBgMapUp(Bg* bg, LevelData * level, int x, int y) {
	static int oldy = -9999;
	int loopx;

	// Set the offset of the background
	bg->y_scroll = y & 255;

	// Get the location in tiles by bitshifting to the right by 3 (/8)
	x = x >> 3;
	y = y >> 3;

	// Check to see if we need to copy a new row
	if (y == oldy) {
		return 0;
	}

	oldy = y;
	u16 * mapData = bg->mapData;
	u16 * fgData = bg->foreground->mapData;
	for (loopx = -5; loopx < 38; loopx++) {
		int dest = getTileIndex(loopx + x, y);
		int src = (loopx + x) + (y * level->width);

		if ((loopx + x) < 0)
			src = -1;
		if ((loopx + x) >= level->width)
			src = -1;

		
		int tile = 0;
		int fgTile = -1;

		if (src >= 0)
		{
			tile = bgMapData[src];
			fgTile = fgMapData[src];
		}

		mapData[dest] = bgPalApplier(level, tile);
		fgData[dest] = fgPalApplier(level, fgTile);

	}

	return 1;
}

// Scrolls a background down by loading a new bottom row
u16 ScrollBgMapDown(Bg* bg, LevelData * level, int x, int y) {
	static int oldy = -9999;
	int loopx;

	// Set the offset of the background
	bg->y_scroll = y & 255;

	// Get the location in tiles by bitshifting to the right (/8)
	x = x >> 3;
	y = y >> 3;

	// Check to see if we need to copy a new row
	if (y == oldy) {
		return 0;
	}

	oldy = y;

	// Copy in the bottom row
	y += SCREEN_BLOCK_HEIGHT;

	u16 * mapData = bg->mapData;
	u16 * fgData = bg->foreground->mapData;
	for (loopx = -5; loopx < 35; loopx++) {
		int dest = getTileIndex(loopx + x, y);
		int src = (loopx + x) + (y * level->width);

		if ((loopx + x) < 0)
			src = -1;
		if ((loopx + x) >= level->width)
			src = -1;

		int tile = 0;
		int fgTile = -1;

		if (src >= 0)
		{
			tile = bgMapData[src];
			fgTile = fgMapData[src];
		}

		mapData[dest] = bgPalApplier(level, tile);
		fgData[dest] = fgPalApplier(level, fgTile);
	}
	return 1;
}

// Scrolls a background up by loading a new left row
u16 ScrollBgMapLeft(Bg* bg, LevelData * level, int x, int y) {
	static int oldx = -9999;
	int loopy;

	// Set the offset of the background
	bg->x_scroll = x & 511;

	
	// Get the location in tiles 
	x = x >> 3;
	y = y >> 3;

	// Check to see if we need to copy a new row
	if (x == oldx) {
		return 0;
	}

	oldx = x;

	u16 * mapData = bg->mapData;
	u16 * fgData = bg->foreground->mapData;
	for (loopy = 0; loopy < 31; loopy++) {
		int dest = getTileIndex(x, y + loopy);
		int src = x + (loopy + y) * level->width;

		int tile = 0;
		int fgTile = -1;

		if (src >= 0)
		{
			tile = bgMapData[src];
			fgTile = fgMapData[src];
		}

		mapData[dest] = bgPalApplier(level, tile);
		fgData[dest] = fgPalApplier(level, fgTile);
	}

	return 1;
}

// Scrolls a background up by loading a new top row
u16 ScrollBgMapRight(Bg * bg, LevelData * lvl, int x, int y) {
	static int oldx = -9999;
	int loopy;

	// Set the offset of the background
	bg->x_scroll = x & 511;

	// Get the location in tiles 
	x = x >> 3;
	y = y >> 3;


	// Check to see if we need to copy a new column
	if (x == oldx) {
		return 0;
	}

	oldx = x;

	// Copy in the far right column
	x+=32;

	u16 * mapData = bg->mapData;
	u16 * fgData = bg->foreground->mapData;
	for (loopy = 0; loopy < 31; loopy++) {
		int dest = getTileIndex(x, y + loopy);
		int src = x + (loopy + y) * lvl->width;

		int tile = 0;
		int fgTile = -1;

		if (src >= 0)
		{
			tile = bgMapData[src];
			fgTile = fgMapData[src];
		}

		mapData[dest] = bgPalApplier(lvl, tile);
		fgData[dest] = fgPalApplier(lvl, fgTile);
	}
	return 1;
}

void writeOneTile(Bg * bg, LevelData * lvl, int x, int y, int tile) {
	//int dest = (x & 31) + (y & 31) * 32;
	int dest = getTileIndex(x,y);
	bg->mapData[dest] = bgPalApplier(lvl, tile);
}

void disableBGMosaic() {
	BG1_CR &= ~BG_MOSAIC_ON;
	BG2_CR &= ~BG_MOSAIC_ON;
	BG3_CR &= ~BG_MOSAIC_ON;
}

void setBGMosaic(int mosaicLevel) {
	mosaicLevel = limitRange(mosaicLevel, 0, 15);
	BG1_CR |= BG_MOSAIC_ON;
	BG2_CR |= BG_MOSAIC_ON;
	BG3_CR |= BG_MOSAIC_ON;

	MOSAIC_CR = (mosaicLevel << 4) | mosaicLevel;

}

/**
 * enable a bg -- must pass in the actual value to write to the background register
 */
void enableBg(int x) {
	DISPLAY_CR = (DISPLAY_CR | (x));
}

void enableSubBg(int x) {
	SUB_DISPLAY_CR = (SUB_DISPLAY_CR | (x));
}

/**
 * enable a bg -- must pass in the actual value to clear from the background register
 */
void disableBg(int x) {
	DISPLAY_CR = (DISPLAY_CR & ~(x));
}

void disableSubBg(int x) {
	SUB_DISPLAY_CR = (SUB_DISPLAY_CR & ~(x));
}

void bgMosaicBlurIn(int framesPerLevel) {
	int i;
	int j;
	for (i = 15; i >= 0; i--) {
		setBGMosaic(i);
		for (j = 0; j < framesPerLevel; j++) {
			sortSpriteList();
			frameTick();
			updateSpriteDisplay();
		}
	}
	disableBGMosaic();
}

void bgMosaicBlurOut(int framesPerLevel) {
	int i;
	int j;
	for (i = 0; i <= 15; i++) {
		setBGMosaic(i);
		for (j = 0; j < framesPerLevel; j++) {
			sortSpriteList();
			frameTick();
			updateSpriteDisplay();
		}
	}
	disableBGMosaic();
}

/**
 * Gets a tile index for a 512x256 bg
u16 getTileIndex(u32 x, u32 y)
{
    u32 n= x + y*5;
    if(x >= 32)
        n += 0x03E0;
    return n;
}
 */




/*
u16 getTileIndexGeneric(u32 x, u32 y, u16 bgcnt)
{
    u32 n= x + y*5;
    if(x >= 32)
        n += 0x03E0;
    if(y >= 32 && (bgcnt&BG_REG_64x64))
        n += 0x0400;
    return n;
}
*/

