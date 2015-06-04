#include "hud.h"
#include "mainChar.h"
#include "text.h"
#include "misc.h"
#include "item.h"
#include "items.h"
#include "spriteMgr.h"
#include "spriteMgr.h"
#include "gfx/hud.h"
#include "enemies/itemGfx.h"
#include "config.h"


//tiles for the life bar
#define LIFE_F_1 202
#define LIFE_F_2 203
#define LIFE_F_RL 206
#define LIFE_F_RR 207
#define LIFE_F_END1 208
#define LIFE_F_END2 209
#define LIFE_BOTTOM_INC 32

#define LIFE_E_1 266 
#define LIFE_E_2 267 
#define LIFE_E_RL 270 
#define LIFE_E_RR 269 
#define LIFE_E_END1 270
#define LIFE_E_END2 273

#define LIFE_H_RL 272
#define LIFE_H_RR 271

#define ITEM_BOX_TILE 198
#define ITEM_BOX_X 26
#define ITEM_BOX_Y 0

#define LIFE_START 1

#define GOLD_X 1 
#define GOLD_Y 2
#define GOLD_SPRITE_TILE 154

#define SHOWLIFECHIP(x)  writeTile(i,0,(x));writeTile(i,1,(x)+LIFE_BOTTOM_INC);

#define RATIO 2

#define LEFT 1
#define RIGHT 0

#define SWAPSWAPPER() swapper=1-swapper
#define GETSWAPPED(x,y) ((swapper==LEFT)?(x):(y))

#ifdef HIDE_HUD
#define HUD_CHECK return;
#else
#define HUD_CHECK
#endif

int HUD_lastDisplayedHP;
int HUD_lastDisplayedHPMax;

int HUD_lastItemDisplayed;
int HUD_lastItemQtyDisplayed;

int HUD_lastGoldDisplayed = -1;

Sprite itemHud;
Sprite goldHud;

Sprite * getHudItemSprite()
{
	return &itemHud;
}

extern u16 lockGfxPal[];

#ifdef GOLD_HUD
Sprite * getHudGoldSprite()
{
	return &goldHud;
}

void initGoldHud()
{
	Sprite * sprite = &goldHud;
	initSprite(sprite);
	setSpriteShape(sprite, SQUARE);
	setSpriteShape(sprite, SQUARE);
	setSpriteSize(sprite, SIZE_16);
	setSpriteBitDepth(sprite, BIT_DEPTH_16);
	setSpritePriority(sprite, 0);
	PaletteId itemPal = loadPalette((u16*)itemsPal);
	setSpritePalette(sprite, itemPal);
	setSpriteStartTile(sprite, GOLD_SPRITE_TILE);
	setSpritePosition(sprite, (GOLD_X << 3), (GOLD_Y << 3));
	setSpriteFaded(sprite, false);
	
}

void showGold()
{
	int gold = getMainCharGold();
	if (gold != HUD_lastGoldDisplayed)
	{
		initGoldHud();
		writeNum(GOLD_X + 2, GOLD_Y, gold);
		HUD_lastGoldDisplayed = gold;
	}

}

#else
void showGold()
{
}
#endif

void reloadHudPalette()
{
	loadBGPaletteLine((u16*)hud_Pal, HUD_PALETTE_NUM);
	loadBGPaletteLine((u16*)hud_ButtonPal, HUD_BUTTON_PALETTE_NUM);
	loadBGPaletteLine((u16*)minimap_Pal, MINIMAP_PALETTE_NUM);

	loadSubBGPaletteLine((u16*)hud_Pal, HUD_PALETTE_NUM);
	loadSubBGPaletteLine((u16*)hud_ButtonPal, HUD_BUTTON_PALETTE_NUM);
	loadSubBGPaletteLine((u16*)minimap_Pal, MINIMAP_PALETTE_NUM);

	loadSubSpriteGfx((u16*)itemsTiles, 64, 128);
	loadSubSpritePaletteLine((u16*)itemsPal, SUB_ITEMS_PAL);
	loadSubSpritePaletteLine((u16*)lockGfxPal, SUB_LOCKS_PAL);

}


void forceHudRedraw()
{
	HUD_CHECK
	HUD_lastDisplayedHP = -10;
	HUD_lastItemDisplayed = -1;
	HUD_lastGoldDisplayed = -1;
}

void initItemHud()
{
	HUD_CHECK
	Sprite * sprite = &itemHud;
	initSprite(sprite);
	setSpriteShape(sprite, SQUARE);
	setSpriteShape(sprite, SQUARE);
	setSpriteSize(sprite, SIZE_16);
	setSpriteBitDepth(sprite, BIT_DEPTH_16);
	setSpritePriority(sprite, 0);
	PaletteId itemPal = loadPalette((u16*)itemsPal);
	setSpritePalette(sprite, itemPal);
	setSpriteStartTile(sprite, 132);
	setSpritePosition(sprite, (ITEM_BOX_X << 3) + 8, (ITEM_BOX_Y << 3) + 8);
	setSpriteFaded(sprite, false);
	
}


void hideHud()
{
	Sprite * sprite = &itemHud;
	setSpriteNotInUse(sprite);
	setSpriteFaded(sprite, false);
	clearText();
}



void showItem()
{
	HUD_CHECK
	int currItem = getCurrentItem();
	int currItemQty = getItemUsableQty(currItem);

	if ((currItem == HUD_lastItemDisplayed) &&
		(currItemQty == HUD_lastItemQtyDisplayed))
	{
		return;
	}


	if (currItem != ITEM_NONE)
	{
		initItemHud();
		if (currItem < 15) 
		{
			setSpriteStartTile(&itemHud, 128 + (currItem * 2)-2);
		}
		else
		{
			setSpriteStartTile(&itemHud, 128 + ((currItem - 15) * 2) + 67);
		}
	}
	else
	{
		setSpriteNotInUse(&itemHud);
	}
	int x;
	int y;
	for(x = 0; x < 4; x++)
	for(y = 0; y < 4; y++)
	{
		writeTile(x + ITEM_BOX_X, y + ITEM_BOX_Y, ITEM_BOX_TILE + x + (y*32));
	}

	writeText(ITEM_BOX_X + 1, ITEM_BOX_Y + 4, "   ");
	if (currItemQty != ITEM_NO_COUNT)
	{
		if (currItemQty > 99)
		{
			writeNum(ITEM_BOX_X + 1, ITEM_BOX_Y + 4, currItemQty);
		}
		else
		{
			writeNum(ITEM_BOX_X + 2, ITEM_BOX_Y + 4, currItemQty);
		}
	}
	else
	{
		writeText(ITEM_BOX_X + 1, ITEM_BOX_Y + 4, "   ");
	}


}

void showLife()
{
	HUD_CHECK
	int mcHP = getMainCharHP();
	int maxHP = getMainCharMaxHP();

	//don't bother redrawing if nothing has changed
	if ((mcHP ==  HUD_lastDisplayedHP) &&
		(maxHP == HUD_lastDisplayedHPMax))
	{
		return;
	}

	HUD_lastDisplayedHP = mcHP;
	HUD_lastDisplayedHPMax = maxHP;


	int fullBlocks = mcHP >> RATIO;
	int maxHPBlocks = maxHP >> RATIO;


	int remainder = mcHP - (fullBlocks << RATIO);

	 //fullBlocks = 5;
	 //maxHPBlocks = 6;
	 //remainder = 2;
	 //

	int i = LIFE_START;


	//first tile
	if ((fullBlocks >= 1) || (remainder > 1))
	{
		SHOWLIFECHIP(LIFE_F_1);
	}
	else
	{
		//put empty 1st tile
		SHOWLIFECHIP(LIFE_E_1);
	}

	//2nd tile
	i++;
	if (fullBlocks >= 2)
	{
		SHOWLIFECHIP(LIFE_F_2);
	}
	else
	{
		//put empty 2nd tile
		if ((fullBlocks == 1) && (remainder > 0))
		{
			SHOWLIFECHIP(LIFE_H_RL);	
		}
		else
		{
			SHOWLIFECHIP(LIFE_E_2);
		}
	}


	//full tiles
	int swapper = LEFT;
	int lastNormal = min(fullBlocks, maxHPBlocks - 1);
	for (i = LIFE_START + 2; i <= lastNormal + LIFE_START; i++)
	{
		int chip = GETSWAPPED(LIFE_F_RL, LIFE_F_RR);
		SWAPSWAPPER();
		SHOWLIFECHIP(chip);
	}


	if ((remainder > 0) && (fullBlocks > 1) && (fullBlocks < maxHPBlocks - 1))
	{
		int chip = GETSWAPPED(LIFE_H_RL, LIFE_H_RR);
		SWAPSWAPPER();
		SHOWLIFECHIP(chip);
		i++;
	}


	//empty tiles
	for (i = i; i <= maxHPBlocks - 1 + LIFE_START; i++)
	{
		SHOWLIFECHIP(GETSWAPPED(LIFE_E_RL,LIFE_E_RR));
		SWAPSWAPPER();
	}

	//2nd to last tile
	//if (i <= maxHPBlocks - 1)
	//{
		//if (fullBlocks > maxHPBlocks - 2)
		//{
			//SHOWLIFECHIP( LIFE_F_END1);
		//}
		//else
		//{
			//SHOWLIFECHIP(LIFE_E_END1);
		//}
		//i++;
	//}

	//last tile
	if (fullBlocks > maxHPBlocks - 1)
	{
		SHOWLIFECHIP(LIFE_F_END2);
	}
	else
	{
		SHOWLIFECHIP(LIFE_E_END2);
	}
	i++;
	
}


void updateHud()
{
	HUD_CHECK
	showGold();
	showLife();
	showItem();
}
