#ifndef BG_H
#define BG_H

#include <nds.h>
#include <nds/arm9/math.h>
#include <nds/arm9/console.h>
#include <nds/arm9/trig_lut.h>


#include "level.h"
#include "bgDefines.h"


//#define BG_MOSAIC_ENABLE		0x40

//#define CharBaseBlock(n)		(((n)*0x4000)+0x6000000)
//#define ScreenBaseBlock(n)		(((n)*0x800)+0x6000000)

//#define WRAPAROUND              	0x1
#define TILEWITHPAL(x,y) ((x) | ((y)<<12))

//#define CHAR_SHIFT			2
//#define SCREEN_SHIFT			8

typedef struct Bg
{
	u32* tileData;
	u16* mapData;
	struct Bg * foreground;
	u8 mosaic;
	int colorMode;
	u8 number;
	u16 size;
	u8 tileBlock;
	u8 mapBlock;
	u8 wraparound;
	s16 x_scroll,y_scroll;
	s32 DX,DY;
	s16 PA,PB,PC,PD;
	bool isSub;
	int priority;
}Bg;

struct LevelDataStruct;


typedef u16(*TilePaletteApplierFunction)(LevelData *, int);


#ifndef BG_C
extern void enableBackground(Bg* bg);
extern void updateBackground(Bg* bg);
extern bool initBackground(Bg* bg, int layer);
extern void loadBGPalette(u16 * palette);
extern void loadSubBGPalette(u16 * palette);
extern void loadBGPaletteLine(u16 * palette, int paletteNum);
extern void loadSubBGPaletteLine(u16 * palette, int paletteNum);
extern void lightenBGPalette(int paletteNum, int r, int g, int b);
extern void lightenSubBGPalette(int paletteNum, int r, int g, int b); 
extern void loadBlankBGPaletteLine(int paletteNum);
extern void loadBGTiles(Bg * bg, u32* gfx, int numTiles);
extern void initScreen(Bg * bg, LevelData * level, int x, int y, Bg * bgFG);
extern u16 ScrollBgMapUp (Bg* bg, LevelData * level, int x, int y);
extern u16 ScrollBgMapDown (Bg* bg, LevelData * level, int x, int y);
extern u16 ScrollBgMapLeft (Bg* bg, LevelData * level, int x, int y);
extern u16 ScrollBgMapRight (Bg* bg, LevelData * level, int x, int y);
extern void zeroBGMap(Bg * bg, int mapSize);
void hideBgs();
void restoreBgs();
void disableBG(Bg * bg);
void bgMosaicBlurIn(int framesPerLevel);
void bgMosaicBlurOut(int framesPerLevel);
u16 * mapDataForBG(int bgNum);
void setPaletteAppliers(TilePaletteApplierFunction bgApp, TilePaletteApplierFunction fgApp);
void writeOneTile(Bg * bg, LevelData * lvl, int x, int y, int tile);
void enableBg(int x); 
void disableBg(int x); 
void enableSubBg(int x); 
void disableSubBg(int x); 
u16 * getBgPaletteMem();
u16 * getSubBgPaletteMem();
void saveBgPalette();
void restoreBgPalette();
void setBGMosaic(int mosaicLevel); 
void disableBGMosaic();
#endif
#endif
