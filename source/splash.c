#include "splash.h"
#include "misc.h"
#include "bg.h"
#include "text.h"
#include "textData.h"
#include "enemies.h"
#include "sound.h"
#include "hud.h"
#include "gfx/cutscene_dungeonWindow.h"
#include "gfx/splashMenu.h"
#include "gfx/bg_gameover.h"
#include "gfx/angel.h"
#include "gfx/spriteAttack.h"
#include "gfx/menuBottomDay.h"
#include "gfx/menuTopNoTitle.h"
#include "gfx/mainMenuTitleTop.h"
#include "gfx/dayMenuNewGame.h"
#include "characters.h"
#include "mainChar.h"
#include "darkness.h"
#include "gameState.h"
#include "input.h"
#include "platform.h"
#include "memcpy.h"

#define CHILI_BITE_DELAY 120
#define CHILI_SCREEN_DELAY 180

#define ANGEL_SCREEN_POS_X 132
#define ANGEL_SCREEN_POS_Y 100

#define ANGEL_RADIUS_X 2 
#define ANGEL_RADIUS_Y 1

#define SPLASH_SCREEN_SIZE 608
#define FULL_SPLASH_SCREEN_SIZE 1024

#define TILE(x,y) bg->mapData[((y)*32) + (x)]
#define SWAPTILES(x,y,i,j) tmp=TILE(x,y);TILE(x,y)=TILE(i,j);TILE(i,j)=tmp
#define BLEND_BG_FRONT(x) (1<<(x))

int g_gameOverChoice= NO_CHOICE;

void turnOffBottomScreen()
{
	disableSubBg(BG_TEXT);
	disableSubBg(BG_MID);
	disableSubBg(BG_FOREGROUND);
	disableSubBg(BG_MAIN);
}

void doSplashLoop() {

	clearCustomSprites();
	while (!isKeyPress(KEY_A) && isKeyPress(KEY_START)) {
		frameTick();
		updateSpriteDisplayUsingCustomSprites();
	}
}

void showSplashScreen(void * gfx, void * pal) {

	setFadeBlack();
	setGfxMode(GFX_MODE_SPLASH);

	void * gfxDest = (void *)SCREEN_BASE_BLOCK(0);

	copyMem16(gfxDest, gfx, 19200);
	copyMem16(getBgPaletteMem(), pal, 128);
	fadeInSplash(1);
}

void showTileSplashScreenSub(void * gfx, void * pal, void * map, int yAdjust)
{
	turnOffBottomScreen();

	copyMem16(getSubBgPaletteMem(), pal, 256);
	Bg subSplash;
	Bg * bg = (Bg*)&subSplash;
	bg->isSub = true;

	initBackground(bg, BG_MID_NUM);

	bg->tileBlock = 2;
	bg->mapBlock = 0;
	bg->size = BgSize_T_256x256;
	bg->colorMode = BG_COLOR_256;
	bg->isSub = true;

	enableBackground(bg);
	loadBGTiles(bg, (u32*)gfx, 2024);

	//zeroMem((u32*)bg->mapData, 256);
	copyMem16(bg->mapData, map, SPLASH_SCREEN_SIZE + 1024);
	//zeroMem((u32*)(bg->mapData + SPLASH_SCREEN_SIZE), 600);
	updateBackground(bg);
}

u16 * showTileSplashScreenNoModeChg(void * gfx, void * pal, void * map,
		bool fadeIn) {

	disableBg(BG_TEXT);
	disableBg(BG_MAIN);
	disableBg(BG_MID);

	setFadeBlack();
	copyMem16(getBgPaletteMem(), pal, 256);
	Bg splash;
	Bg * bg = (Bg*)&splash;
	initBackground(bg, 2);
	bg->tileBlock = 0;
	bg->x_scroll = -8;
	enableBackground(bg);
	loadBGTiles(bg, (u32*)gfx, 1024);
	zeroMem((u32*)bg->mapData, 2048);
	copyMem16(bg->mapData, map, SPLASH_SCREEN_SIZE); //this gets everything from the original GBA size of splash screens
	updateBackground(bg);

	if (fadeIn) {
		fadeInSplash(1);
	} else {
		fadeInSplash(15);
	}

	return bg->mapData;

}

u16 * showDualIntroScreen(void * topGfx, void * topPal, void * topMap,
						  void * subGfx, void * subPal, void * subMap){

	
	disableBg(BG_TEXT);
	disableBg(BG_MAIN);
	disableBg(BG_MID);

	disableSubBg(BG_TEXT);
	disableSubBg(BG_MAIN);
	disableSubBg(BG_MID);


	setFadeBlack();
	setSubFadeBlack();


	copyMem16(getBgPaletteMem(), topPal, 256);
	Bg splash;
	Bg * bg = (Bg*)&splash;
	initBackground(bg, 2);
	bg->tileBlock = 0;
	bg->x_scroll = 0;
	enableBackground(bg);
	loadBGTiles(bg, (u32*)topGfx, 2024);
	copyMem16(bg->mapData, topMap, FULL_SPLASH_SCREEN_SIZE); //this gets everything from the original GBA size of splash screens
	bg->foreground = null;
	updateBackground(bg);

	copyMem16(getSubBgPaletteMem(), subPal, 256);
	u16 * subPalM = getSubBgPaletteMem();
	subPalM[0] = 0;


	Bg sub;
	bg = (Bg*)&sub;
	initBackground(bg, 2);
	bg->tileBlock = 0;
	bg->x_scroll = 0;
	bg->isSub = true;
	enableBackground(bg);
	loadBGTiles(bg, (u32*)subGfx, 2024);
	copyMem16(bg->mapData, subMap, FULL_SPLASH_SCREEN_SIZE); //this gets everything from the original GBA size of splash screens
	updateBackground(bg);



	fadeInSplashScreens(1, true);

	return bg->mapData;
}


u16 * showTileSplashScreen(void * gfx, void * pal, void * map, bool fadeIn) {
	turnOffBottomScreen();
	return showTileSplashScreenNoModeChg(gfx, pal, map, fadeIn);
}

void showChiliBite(void * gfx, void * pal, void * map) {
	u16 * mapData = showTileSplashScreenNoModeChg(gfx, pal, map, false);
	showTileSplashScreenSub((void*)spriteAttackTiles, (void*) spriteAttackPal, (void*)spriteAttackMap, 0);

	//doh...these edge pieces are showing onscreen now...let's nuke 'em
	int i, j;
	for (i = -2; i < 0; i++)
		for (j = 14; j < 20; j+= 1)
			mapData[i+j*32] = 0;

	//this is a hack...something in here is wiping out my pre-loaded sound system stuff..
	initSoundSystem();

	int frameCtr = 0;

	while (!isKeyPress(KEY_START) && !isKeyPress(KEY_A) && (frameCtr
			< CHILI_SCREEN_DELAY)) {
		frameTick();
		if (frameCtr++ == CHILI_BITE_DELAY) {
			playGameSfx(SFX_BITE);
			//some magic numbers....(yeah yeah)
			//these replace the tiles for biting the chili
			//coordinates figured out by trial and error
			for (i = 6; i < 11; i++)
				for (j = 192; j < 384; j += 32) {

					mapData[i+j] = 0;

				}
			mapData[266] = 136;
			mapData[299] = 155;
			mapData[331] = 172;
			mapData[362] = 173;
		}

	}
	fadeOutSplashes(1);

}

void addText(char ** lines) {

	enableBg(BG_TEXT);
	writeLines(lines);

}

void showTextScreen(char ** lines) {
	setGfxMode(GFX_MODE_TEXTONLY);
	writeLines(lines);
}

void setFadeBlack() {
	REG_BLDCNT = BLEND_DST_BACKDROP | BLEND_ALPHA | BLEND_SRC_BG2;
	REG_BLDALPHA = 15<<8;
}
void setSubFadeBlack() {
	REG_BLDCNT_SUB = BLEND_DST_BACKDROP | BLEND_ALPHA | BLEND_SRC_BG2;
	REG_BLDALPHA_SUB = 15<<8;
}


void fadeOutBgScreen(int fadeSpeed, int bgToFade, bool both) {
	int tickCtr = 2;
	if (fadeSpeed < 0) {
		tickCtr = abs(fadeSpeed);
		fadeSpeed = 1;
	}
	if (both)
	{
		REG_BLDCNT_SUB = BLEND_DST_BACKDROP | BLEND_ALPHA | BLEND_BG_FRONT(1);
	}
	REG_BLDCNT = BLEND_DST_BACKDROP | BLEND_ALPHA | BLEND_BG_FRONT(bgToFade);
	int i;
	int j;
	for (i = 15; i >= 0; i-=fadeSpeed) {
		int bg = (15 - i) << 8;
		if (both)
		{
			REG_BLDALPHA_SUB = bg | i;
		}
		REG_BLDALPHA = bg | i;

		for (j = 0; j < tickCtr; j++) {
			frameTick();
		}
	}

}

void fadeOutBg(int fadeSpeed, int bgToFade) {
	fadeOutBgScreen(fadeSpeed, bgToFade, false);
}

void fadeOutSplashes(int fadeSpeed) {
	fadeOutBgScreen(fadeSpeed, BG_FOREGROUND_NUM, true);
}

void fadeOutSplash(int fadeSpeed) {
	fadeOutBg(fadeSpeed, BG_FOREGROUND_NUM);
}

void fadeInSplashScreens(int fadeSpeed, bool both) {
	clearCustomSprites();
	int tickCtr = 2;
	if (fadeSpeed < 0) {
		tickCtr = abs(fadeSpeed);
		fadeSpeed = 1;
	}
	if (both)
	{
		REG_BLDCNT_SUB = BLEND_DST_BACKDROP | BLEND_ALPHA | BLEND_SRC_BG2;
	}
	REG_BLDCNT = BLEND_DST_BACKDROP | BLEND_ALPHA | BLEND_SRC_BG2;
	int i;
	int j;
	for (i = 0; i <= 15; i+=fadeSpeed) {
		int bg = (15 - i) << 8;
		REG_BLDALPHA = bg | i;
		if (both)
		{
			REG_BLDALPHA_SUB = bg | i;
		}

		for (j = 0; j < tickCtr; j++) {
			frameTick();
			updateSpriteDisplayUsingCustomSprites();
		}
	}

}

void fadeInSplash(int fadeSpeed) {
	fadeInSplashScreens(fadeSpeed, false);
}

int doGameOver() {
	exitMainLoop();

	deactivateDarkMode();
	int mosaicCtr = 0;
	int mosaicLvl = 0;
	bool loop = true;
	while (loop) {

		frameTick();
		if (mosaicCtr++ == 6)
		{
			setBGMosaic(mosaicLvl);
			lightenBGPalette(0, -1, -1, -1);
			lightenBGPalette(1, -1, -1, -1);
			lightenBGPalette(2, -1, -1, -1);
			lightenBGPalette(3, -1, -1, -1);
			lightenBGPalette(4, -1, -1, -1);
			mosaicCtr = 0;
			if (mosaicLvl++ == 16)
			{
				loop = false;
			}
		}
	}
	disableBGMosaic();

	disableBg(BG_FOREGROUND);
	disableBg(BG_MAIN);
	disableBg(BG_MID);
	clearText();
	hideHud();
	initEnemies(0, null);
	initSpriteList();
	sortSpriteList();
	updateSpriteDisplay();
	setGfxMode(GFX_MODE_STANDARD);
	disableSprites();
	showTileSplashScreen((void*)bg_gameoverTiles, (void*)bg_gameoverPal,
			(void*)bg_gameoverMap, true);
	resetWorldPosition();

	//initialize the angel sprite
	CharacterStatus angelChar;
	CharacterStatus * angel = &angelChar;
	Sprite * angelSprites[1];
	angelSprites[0] = null;
	getMultipleSpritesWithOwner(angelSprites, 1, angel);
	angel->spriteList = angelSprites;
	angel->listSize = 1;
	setSpritePriority(angel->spriteList[0], 0);

	angel->gfxBlock = loadSpriteGfxBlock((u16*)angelTiles);
	loadAndSetPalette(angel, (u16*)angelPal);
	setCharacterAnimationForced(angel, (characterAnimDef *)AngelAnim);

	int posX = 0;
	int posY = 0;
	int xDir = 1;
	int yDir = 1;
	int xFrameCtr = 0;
	int yFrameCtr = 0;

	//just like doSplashLoop, but can have extra code to control the "angel"

	//we want sprites turned on so we can see the angel
	sortSpriteList();
	updateSpriteDisplay();
	frameTick();
	enableSprites();

	bool continueSelected = true;
	int choice= NEW_GAME;

	while (!isKeyPress(KEY_START) && !isKeyPress(KEY_A)) {

		//newgame-continue choices
		if (isKeyPress(KEY_LEFT) || isKeyPress(KEY_RIGHT) || continueSelected) {
			continueSelected = false;

			Bg splash;
			Bg * bg = (Bg*)&splash;
			initBackground(bg, 2);
			bg->tileBlock = 0;
			enableBackground(bg);

			choice = 1 - choice;

			const u16 * bgSource = bg_gameoverMap;

			if (choice != NEW_GAME)
			{
				bgSource += (32 * 4);
			}

			int x, y;

			for (x = 14; x < 17; x++) {
				for (y = 16; y < 18; y++) {
					bg->mapData[y * 32 + x] = bgSource[y * 32 + x];
				}
			}

			u16 * pal = getBgPaletteMem();
			u16 paltmp = pal[0x10];
			pal[0x10] = pal[0x21];
			pal[0x21] = paltmp;
			playGameSfx(SFX_CHANGE_ITEM);

		}

		//reposition the angel
		xFrameCtr++;
		yFrameCtr++;

		if (xFrameCtr > 20) {
			posX += xDir;
			xFrameCtr = 0;
			if (abs(posX) > ANGEL_RADIUS_X) {
				xDir =-xDir;
			}
		}
		if (yFrameCtr > 10) {
			posY += yDir;
			yFrameCtr = 0;
			if (abs(posY) >= ANGEL_RADIUS_Y) {
				yDir =-yDir;
				//setSpriteFlip(angel->spriteList[0], (yDir<0));
			}
		}

		angel->x = ANGEL_SCREEN_POS_X + posX;
		angel->y = ANGEL_SCREEN_POS_Y + posY;

		updateCharacter(angel);
		repositionCharacter(angel);

		frameTick();
		sortSpriteList();
		updateSpriteDisplay();
	}

	initSpriteList();
	disableSprites();
	resetGfxStats();
	clearText();
	setGfxMode(GFX_MODE_TEXTONLY);

	g_gameOverChoice = choice;

	return choice;
}

int getGameOverChoice() {
	return g_gameOverChoice;
}

void resetMainMenu() {
	g_gameOverChoice = NO_CHOICE;
}

void doDemoOver() {
	initEnemies(0, null);
	hideHud();
	initSpriteList();
	updateSpriteDisplay();
	disableBg(BG_FOREGROUND);
	fadeOutBg(-10, BG_MAIN_NUM);
	exitMainLoop();
	initSpriteList();
	setGfxMode(GFX_MODE_STANDARD);
	clearText();
	disableSprites();
	showTileSplashScreenNoModeChg((void*)cutscene_dungeonWindowTiles,
			(void*)cutscene_dungeonWindowPal, (void*)cutscene_dungeonWindowMap,
			true);
	addText((char **)TextDemoOver1);
	doSplashLoop();
	clearText();
	addText((char **)TextDemoOver2);
	doSplashLoop();
	clearText();
	disableSprites();
	setGfxMode(GFX_MODE_TEXTONLY);
	g_gameOverChoice = NO_CHOICE;
}

void changeToContinue(Bg * bg)
{
	u16 * subPal = getSubBgPaletteMem();
	subPal[0x3b] = 0x00;
	subPal[0xe1] = 0xff;

	int i = 0;
	int x,y;
	for (  y = 1; y < 3; y++)
	for (  x = 9; x < 13; x++)
	{
		i = y * 22 + (21 - x);
		bg->mapData[y * 32 + x] = i | (1 << (0xa));
	}


}

void changeToNewGame(Bg * bg)
{
	//reset the menu palette
	u16 * subPal = getSubBgPaletteMem();
	subPal[0x3b] = 0xff;
	subPal[0xe1] = 0x00;

	int i = 0;
	int x,y;
	for (  y = 0; y < 4; y++)
	for (  x = 0; x < 22; x++)
	{
		bg->mapData[y * 32 + x] = i;
		i++;
	}

}


void initMenuLayer(Bg * bg)
{
	initBackground(bg, 1);
	bg->tileBlock = 3;
	bg->x_scroll = -40;
	bg->y_scroll = 38;
	bg->isSub = true;
	bg->mapBlock = 29;
	enableBackground(bg);
	loadBGTiles(bg, (u32*)dayMenuNewGameTiles, 200);
	int i;
	for (i = 100 * 16; i < 101 * 16; i++)
	{
		bg->tileData[i] = 0;
	}
	fillMem16(100, bg->mapData, 2048); 


	updateBackground(bg);

}

void initTitleLayer(Bg * bg)
{
	initBackground(bg, 1);
	bg->tileBlock = 5;
	bg->x_scroll = 0;
	bg->y_scroll = 0;
	bg->isSub = false;
	bg->mapBlock = 29;
	enableBackground(bg);
	loadBGTiles(bg, (u32*)mainMenuTitleTopTiles, 1024);

	copyMem16(bg->mapData, (u16*)mainMenuTitleTopMap, FULL_SPLASH_SCREEN_SIZE); 

	updateBackground(bg);

}

void updateMenuAnim(Bg * subBg, int frame)
{
	//move menu
	subBg->y_scroll = max(32, 24 + frame);
	updateBackground(subBg);


	//adjust alpha blend
	REG_BLDCNT = BLEND_DST_BG2 | BLEND_ALPHA | BLEND_BG_FRONT(1);


	if (frame > 70)
	{
		frame += 10;
	}
	int blendLevel = frame / 6;
	if (frame == 90)
	{
		blendLevel = 16;


	}
	int bg = (16 - blendLevel) << 8;
	REG_BLDALPHA = bg | blendLevel;



	frameTick();
}

int mainMenu(bool continueSelected) {
	disableSprites();
	showDualIntroScreen((void*)menuTopNoTitleTiles, (void*)menuTopCombinedPal, (void*)menuTopNoTitleMap,
                        (void*)menuBottomDayTiles,  (void*)menuBottomDayPal,   (void*)menuBottomDayMap);

	Bg sub;
	Bg * subBg = &sub;

	Bg title;
	Bg * titleBg = &title;

	initMenuLayer(subBg);
	initTitleLayer(titleBg);

	changeToNewGame(subBg);

	int choice = NEW_GAME;

	if (continueSelected)
	{
		changeToContinue(subBg);
		choice = 1 - choice;
	}

	int frame = 0;
	while (frame < 90)
	{
		if (isKeyPress(KEY_START) || isKeyPress(KEY_A))
		{
			frame = 89;
		}
		updateMenuAnim(subBg, frame++);
	}

	
	while (!isKeyPress(KEY_START) && !isKeyPress(KEY_A)) 
	{

		if (isKeyPress(KEY_LEFT) || isKeyPress(KEY_RIGHT)) 
		{

			choice = 1 - choice;
			if (choice == NEW_GAME)
			{
				changeToNewGame(subBg);
			}
			else
			{
				changeToContinue(subBg);
			}
			
			playGameSfx(SFX_CHANGE_ITEM);

		}

		frameTick();
		//updateSpriteDisplayUsingCustomSprites();
	}

	fadeOutSplash(1);
	zeroBGMap(titleBg, 32 * 4);

	return choice;

}

#define CREDIT_SCROLL_DELAY 1
#define LINES_PER_CREDIT 16
#define CREDIT_START_X 2
#define STOP_CREDITS -20 
#define CREDITS_COMPLETELY_FINISHED -30 

int scrollCreditsUp(int scrollPosition) {
	scrollPosition++;
	if (scrollPosition > 255) {
		scrollPosition = 0;
	}
	scrollTextBg(0, scrollPosition);
	return scrollPosition;
}

int addNextCreditLine(int creditLine, int scrollPosition) {

	int bgy = ((scrollPosition >> 3) + 25) & 31;
	writeText(CREDIT_START_X, bgy, "                             ");

	const char * credits= null;
	if (creditLine >= 0) {
		credits = creditsText[creditLine];
	}

	//if we've hit the last one, keep returning it as the last
	if ((credits == null) || (creditLine < 0)) {
		if (creditLine > 0) {
			return -1;
		}
		creditLine--;
		return creditLine;
	}

	writeText(CREDIT_START_X, bgy, credits);

	creditLine++;

	return creditLine;

}

int stateBy(int state, int shift) {
	return (getGameState(state) << shift);
}

int getValidation() {
	int valid = stateBy(67, 0);
	valid += stateBy(66, 1);
	valid += stateBy(63, 2);
	valid += stateBy(54, 3);
	valid += stateBy(7, 4);
	valid += stateBy(9, 5);
	valid += stateBy(12, 6);
	valid += stateBy(22, 7);
	valid += stateBy(68, 8);
	valid += stateBy(24, 9);
	valid += stateBy(29, 10);
	valid += stateBy(31, 11);
	valid += stateBy(39, 12);
	valid += stateBy(40, 13);
	valid += stateBy(43, 14);
	valid += stateBy(47, 15);
	valid += stateBy(50, 16);
	valid += stateBy(69, 17);
	return valid;

}

void doCreditLoop() {

	clearCustomSprites();
	int scrollTick = 0;
	int linesTick = 0;
	int creditLine = 0;
	int scrollPosition = 0;
	while (!isKeyPress( KEY_START) && !isKeyPress( KEY_A)) {
		if (creditLine != CREDITS_COMPLETELY_FINISHED) {
			if (scrollTick++ >= CREDIT_SCROLL_DELAY) {
				scrollPosition = scrollCreditsUp(scrollPosition);
				scrollTick = 0;
				linesTick++;
			}
			if (creditLine != STOP_CREDITS) {
				if (linesTick >= LINES_PER_CREDIT) {
					creditLine = addNextCreditLine(creditLine, scrollPosition);
					linesTick = 0;
				}
			} else {
				scrollTextBg(0, 0);
				writeText(CREDIT_START_X, 4, "Percentage of items found:");
				int completion = getPercentComplete();
				writeNum(CREDIT_START_X + 9 , 5, completion);
				writeText(CREDIT_START_X + 14 , 5, "%");
				creditLine = CREDITS_COMPLETELY_FINISHED;
			}
		}

		frameTick();
		updateSpriteDisplayUsingCustomSprites();
	}
}

void doCredits() {
	turnOffBottomScreen();
	initEnemies(0, null);
	hideHud();
	initSpriteList();
	updateSpriteDisplay();
	disableBg(BG_FOREGROUND);
	fadeOutBg(-10, BG_MAIN_NUM);
	initSpriteList();
	setGfxMode(GFX_MODE_STANDARD);
	turnOffBottomScreen();
	clearText();
	disableSprites();
	showTileSplashScreenNoModeChg((void*)cutscene_dungeonWindowTiles,
			(void*)cutscene_dungeonWindowPal, (void*)cutscene_dungeonWindowMap,
			true);
	enableBg(BG_TEXT);
	reloadHudPalette();
	doCreditLoop();
	clearText();
	disableSprites();
	setGfxMode(GFX_MODE_TEXTONLY);
	g_gameOverChoice = NO_CHOICE;
	exitMainLoop();
}

