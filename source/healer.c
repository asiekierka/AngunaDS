#include "healer.h"
#include "misc.h"
#include "sound.h"
#include "subscreen.h"
#include "text.h"
#include "textData.h"
#include "hud.h"
#include "darkness.h"
#include "sound.h"
#include "items.h"
#include "mainChar.h"
#include "gameState.h"
#include "input.h"
//NLT - healer needs his portrait
#include "enemies/portraits/oldMan_portrait.h"

#define HEALER_SS_X  SUBSCREEN_X + 7
#define HEALER_SS_Y SUBSCREEN_Y + 4 

#define HEALER_SS_X2  SUBSCREEN_X +10 
#define HEALER_SS_Y2 SUBSCREEN_Y + 6 

void showHealerPanel()
{
	clearText();
	hideHud();
	initTopSubscreen();
	playGameSfx(SFX_SHOW_SUBSCREEN);
	drawPortrait((u16*)oldMan_portrait_Pal, (u16*) oldMan_portrait_Tiles, (u16*) oldMan_portrait_Map);
	enableBg(BG_MID);
	
}


void hideHealerPanel()
{
	playGameSfx(SFX_HIDE_SUBSCREEN);
	clearTopPortrait();
	hideTopSubscreen();
	clearText();
	setStealButtons();
}

void doHealer()
{
	showHealerPanel();
	writeText(HEALER_SS_X, HEALER_SS_Y, HealerText[0]);
	writeText(HEALER_SS_X2, HEALER_SS_Y2, HealerText[1]);
	playGameSfx(SFX_POTION);
	mcAddHP(99999);
	bool done = false;
	while (!done) 
	{
		if (isButtonKeyPressed())
		{
			done = true;
		}
		sortSpriteList();
		frameTick();
		updateSpriteDisplay();

	}
	hideHealerPanel();
	clearText();
	forceHudRedraw();

}

