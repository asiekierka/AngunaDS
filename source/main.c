#include <nds.h>
#include <nds/arm9/math.h>
#include <nds/arm9/console.h>
#include <nds/arm9/trig_lut.h>

#include <stdio.h>


#include "misc.h"
#include "level.h"
#include "text.h"
#include "spriteMgr.h"
#include "enemies.h"
#include "mainChar.h"
#include "bullet.h"
#include "sound.h"
#include "splash.h"
#include "textData.h"
#include "subscreen.h"
#include "gameState.h"
#include "hud.h"
#include "enemyDatabase.h"
#include "gfx/chiliBite.h"
#include "gfx/cutscene_gameStart.h"
#include "enemies/itemGfx.h"
#include "minimap.h"
#include "platform.h"
#include "input.h"


#include "config.h"

#define GAME_START_X T(8)
#define GAME_START_Y T(2)

#ifndef GAME_START
#define GAME_START level_d1_2_0
#endif

#ifndef RELEASE
#define SKIP_SPLASH 
#endif

extern  LevelData GAME_START;
extern const unsigned short mc_downTiles[];
extern const unsigned int spritePalette[];




int main()
{

	//initSoundSystem(); -- this is init'd during the chili bite intro
	initMisc();
	//initVBlank();
	initEnemyDatabase();
	initBulletSystem();
	seedRandom();
	initSaveSystem();

	setGfxMode(GFX_MODE_STANDARD);
#ifndef SKIP_SPLASH
	showChiliBite((void*)chiliBiteTiles, (void*) chiliBitePal, (void*)chiliBiteMap);
#endif 

	while(true)
	{
		deactivateDarkMode();
		disableSprites();
		initSpriteList();
		playMusic(MUSIC_MAINTHEME);	// play module
		

		int startMode = getGameOverChoice();
		if (startMode == NO_CHOICE)
		{
			startMode = mainMenu(verifySaveGame(0));
		}

		if (!(verifySaveGame(0)))
		{
			startMode = NEW_GAME;
		}

		initText();
#ifndef SKIP_SPLASH
		if (startMode == NEW_GAME)
		{
			showTileSplashScreen((void*)cutscene_gameStartTiles, (void*)cutscene_gameStartPal, (void*)cutscene_gameStartMap, true);
			setLastGoodRoom(GAME_START_X, GAME_START_Y, &GAME_START);
			
			clearText();
			addText((char **)cutSceneText1_1);
			waitKeyPress();
			clearText();
			clearSubText();
			addText((char **)cutSceneText1_2);
			waitKeyPress();
			clearText();
			clearSubText();
			addText((char **)cutSceneText1_3);
			waitKeyPress();
			clearSubText();
			fadeOutSplash(1);
		}
#endif
		clearText();
		initEnemySystem();
		initSubscreen();
		initSpriteGfxSwapper();
		initSpriteList();
		loadSpritePalette((u16*)(void*)spritePalette);
		loadSpriteGfx((u16*)mc_downTiles, 0, 128);
		initBullets();
		initMainChar();
		initGameState();
		initLevelLoader();

		
		setGfxMode(GFX_MODE_STANDARD);
		showSubscreen();

		if (startMode == CONTINUE)
		{
			disableSprites();
			if (!loadGame(0))
			{
				startMode = NEW_GAME;
			}
		}		
		
		if (startMode == NEW_GAME)
		{
			disableSprites();
			setLevel(&GAME_START, 0, 0);	
			mcUnstick();
			initEnemyDatabase();
		}
		

		enableSprites();
		

		startMainLoop();

		doDarkIfDark();

		while(shouldContinueMainLoop()) {
			//checkLidSleep();

			processControls();
			updateMainChar();

			updateBullets();
			updateAllEnemies();
			//updateMCDisplay();

			sortSpriteList();

			frameTick();
			reloadMCGfx();
			updateMaps();
			updateSpriteDisplay();
			updateSubscreen();
			updateHud();
			if (isKeyDown(KEY_A))
			{
				if (isKeyDown(KEY_SELECT))
				{
					loadGame(0);

				}

			}
		}
		releaseMainCharSprites();
	}

	return 0;
}

