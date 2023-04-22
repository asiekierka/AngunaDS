#include "shop.h"
#include "sound.h"
#include "subscreen.h"
#include "text.h"
#include "hud.h"
#include "misc.h"
#include "darkness.h"
#include "sound.h"
#include "items.h"
#include "mainChar.h"
#include "gameState.h"
#include "input.h"
#include "enemies/portraits/shopKeeper_portrait.h"



#define SHOP_SS_X  SUBSCREEN_X + 3
#define SHOP_SS_Y SUBSCREEN_Y + 6 

#define DID_BUY_DYNAMITE_SAVE 57
#define DID_BUY_TRAPS_SAVE 70

enum
{
	SHOP_CALTROPS,
	SHOP_POTION,
	SHOP_LIFE_UP,
	SHOP_DYNAMITE,
	SHOP_KEY_RED,
	SHOP_ARROWS,
	SHOP_DEFENCE_UP,
	SHOP_CHEAP_POTION,
	SHOP_DEFENCE_UP_2,
	SHOP_LIFE_UP_2,
	SHOP_MAP_DNGN_2,
	SHOP_MAP_DNGN_3,
	SHOP_MAP_DNGN_4,
};

ShopItemDef shopItems[] = 
{
	[SHOP_CALTROPS] = {ITEM_CALTROPS, 30, 0, DID_BUY_TRAPS_SAVE},
	[SHOP_POTION] = {ITEM_POTION, 50, 0, 0},
	[SHOP_LIFE_UP] = {ITEM_LIFE_UP, 100, 33, 0},
	[SHOP_DYNAMITE] = {ITEM_DYNAMITE, 100, 0, DID_BUY_DYNAMITE_SAVE},
	[SHOP_KEY_RED] = {ITEM_KEY_RED, 200, 0, 0},
	[SHOP_ARROWS] = {ITEM_ARROWS, 15, 0, 0},
	[SHOP_DEFENCE_UP] = {ITEM_DEFENCE_UP, 80, 51, 0},
	[SHOP_CHEAP_POTION] = {ITEM_POTION, 30, 0, 0},
	[SHOP_DEFENCE_UP_2] = {ITEM_DEFENCE_UP, 150, 58, 0},
	[SHOP_LIFE_UP_2] = {ITEM_LIFE_UP, 150, 59, 0},
	[SHOP_MAP_DNGN_2] = {ITEM_MAP_DNGN_2, 30, 0, 0},
	[SHOP_MAP_DNGN_3] = {ITEM_MAP_DNGN_3, 40, 0, 0},
	[SHOP_MAP_DNGN_4] = {ITEM_MAP_DNGN_4, 50, 0, 0},
	
};

ShopItemDef * shop1Items[] = 
{
	&shopItems[SHOP_CALTROPS],
	//&shopItems[SHOP_POTION],
	&shopItems[SHOP_MAP_DNGN_2],
	&shopItems[SHOP_LIFE_UP],
};

ShopItemDef * shop2Items[] = 
{
	&shopItems[SHOP_DYNAMITE],
	&shopItems[SHOP_POTION],
	&shopItems[SHOP_KEY_RED],
};

ShopItemDef * shop3Items[] = 
{
	&shopItems[SHOP_MAP_DNGN_3],
	&shopItems[SHOP_MAP_DNGN_4],
	&shopItems[SHOP_DEFENCE_UP],
};

ShopItemDef * shop4Items[] = 
{
	&shopItems[SHOP_CHEAP_POTION],
	&shopItems[SHOP_DEFENCE_UP_2],
	&shopItems[SHOP_LIFE_UP_2],
};


ShopDef shopDefs[] =
{
	{3,shop1Items},
	{3,shop2Items},
	{3,shop3Items},
	{3,shop4Items},
};



enum
{
	CHOICE_CANCEL,
	CHOICE_BUY
};



void showShopPanel()
{
	clearText();
	initTopSubscreen();
	//clearTopMidScreen();
	clearTopCustomSprites();
	hideHud();
	playGameSfx(SFX_SHOW_SUBSCREEN);
	enableBg(BG_MID);



	activateSubscreenWindow();
}


void hideShopPanel()
{
	playGameSfx(SFX_HIDE_SUBSCREEN);
	hideTopSubscreen();
	clearText();
	setStealButtons();
	
}

void showShopIntro()
{
	showShopPanel();
	drawPortrait((u16*)shopKeeper_portraitPal, (u16*) shopKeeper_portraitTiles, (u16*) shopKeeper_portraitMap);
	bool done = false;

	writeText(SHOP_SS_X, SHOP_SS_Y - 4, "Items for sale!");
	writeText(SHOP_SS_X, SHOP_SS_Y - 2, "Do you want to buy");
	writeText(SHOP_SS_X + 10, SHOP_SS_Y,  "something?");


	while (!done)
	{
		if ((isKeyPress( KEY_B)) || 
			(isKeyPress( KEY_A)) ||
			(isKeyPress( KEY_START))) 
		{
			playGameSfx(SFX_HIDE_SUBSCREEN);
			done = true;
		}
		sortSpriteList();
		frameTick();
		updateTopSpriteDisplayUsingCustomSprites();
	}
	clearTopPortrait();
	hideShopPanel();
	clearText();
	forceHudRedraw();
}


ShopDef * getShopById(int id)
{
	return &shopDefs[id];
}


void doShop(int shopId)
{
	doShopByDef(getShopById(shopId));
}

int getShopActualNumItems(ShopDef * shop)
{

	int i = 0;
	int lastShopNum = 0;
	for (i = 0; i < shop->numItems; i++)
	{
		ShopItemDef * item = shop->shopItems[i];
		if (item->saveIfLimited > 0)
		{
			if (!getGameState(item->saveIfLimited))
			{
				lastShopNum++;
			}
		}
		else
		{
			lastShopNum++;
		}
	
	}

	return lastShopNum;

}

bool itemSoldOut(ShopItemDef * shopItem)
{
	return (getGameState(shopItem->saveIfLimited));
}

extern u16 itemsPal[];
extern u16 lockGfxPal[];

void drawShopItemLine(ShopDef * shop, int itemLine)
{
	int drawLine = itemLine;
	if (itemLine >= getShopActualNumItems(shop))
	{
		return;
	}

	ShopItemDef * shopItemDef = shop->shopItems[itemLine];
	while (getGameState(shopItemDef->saveIfLimited))
	{
		itemLine++;
		if (itemLine >= shop->numItems)
		{
			return;
		}
		shopItemDef = shop->shopItems[itemLine];

	}
	int itemId = shopItemDef->item;

	int x = SHOP_SS_X;
	int y = SHOP_SS_Y + drawLine * 3 + 1;

	writeNum(x, y, shopItemDef->price);
	u16 * pal = (u16*)&itemsPal;
	if (ITEM_IS_KEY(itemId))
	{
		pal = (u16*)lockGfxPal;
	}
	PaletteId itemPal = loadPalette((u16*)pal);
	//show pic of item
	showItemAtSSWithSpecial(itemLine, itemId, 6, y - 7 , itemPal, false);
	writeText(x + 6, y, getItemName(itemId));
	
}

void showItemLines(ShopDef * shop)
{
	int itemLine = 0;
	for (itemLine = 0; itemLine < getShopActualNumItems(shop); itemLine++)
	{
		drawShopItemLine(shop, itemLine);
	}
}

void showShopUI(int option, ShopDef * shop)
{
	int x = SHOP_SS_X - 1;
	int i = 0;
	for (i = 0; i < getShopActualNumItems(shop); i++)
	{
		writeText(x, SHOP_SS_Y + 1 + i * 3, " ");
	}
	writeTile(x, SHOP_SS_Y + 1 + option * 3, CARET);
	showItemLines(shop);
}


void doShopByDef(ShopDef * shop)
{
	showShopIntro();
	showShopPanel();

	bool done = false;
	int choice = CHOICE_CANCEL;

	writeText(SHOP_SS_X, SHOP_SS_Y - 3, "You have:     gold");
	writeNum(SHOP_SS_X + 10, SHOP_SS_Y - 3, getMainCharGold());

	int caretLine = 0;
	showShopUI(caretLine, shop);

	int playerGold = getMainCharGold();

	while (!done)
	{
		if ((isKeyPress( KEY_B)) || 
			(isKeyPress( KEY_START)))
		{
			choice = CHOICE_CANCEL;
			playGameSfx(SFX_HIDE_SUBSCREEN);
			done = true;
		}
		if (isKeyPress( KEY_A)) 
		{
			if (itemSoldOut(shop->shopItems[caretLine]))
			{
				caretLine++;
			}
			ShopItemDef * shopItemDef = shop->shopItems[caretLine];
			int price = shopItemDef->price;
			if ((playerGold < price) || (!canHoldMore(shopItemDef->item)))
			{
				playGameSfx(SFX_BADCHOICE);
			}
			else
			{
				choice = CHOICE_BUY;
				done = true;
			}
		}
		if (isKeyPress( KEY_UP))
		{
			caretLine--;
			if (caretLine < 0)
			{
				caretLine = getShopActualNumItems(shop) - 1;
			}
			showShopUI(caretLine, shop);
		}
		if (isKeyPress( KEY_DOWN))
		{
			caretLine++;
			if (caretLine >= getShopActualNumItems(shop))
			{
				caretLine = 0;
			}
			showShopUI(caretLine, shop);
		}
		sortSpriteList();
		frameTick();
		updateTopSpriteDisplayUsingCustomSprites();
	
	}

	if (choice == CHOICE_BUY)
	{
		ShopItemDef * shopItemDef = shop->shopItems[caretLine];
		int price = shopItemDef->price;
		addMainCharGold(-price);
		pickupItem(shopItemDef->item);
		playGameSfx(SFX_TAKE_ITEM);
		if (shopItemDef->saveIfLimited > 0)
		{
			setGameState(shopItemDef->saveIfLimited, 1);
		}
		if (shopItemDef->saveOnBuy > 0)
		{
			setGameState(shopItemDef->saveOnBuy, 1);
		}
	}

	hideShopPanel();
	clearText();
	forceHudRedraw();
	
}

bool didBuyDynamite()
{
	return getGameState(DID_BUY_DYNAMITE_SAVE);
}

bool didBuyTraps()
{
	return getGameState(DID_BUY_TRAPS_SAVE);
}

