#include "darkness.h"
#include "subscreen.h"
#include "item.h"
#include "items.h"
#include "level.h"
#include "misc.h"
#include "platform.h"

bool mIsDark;

#define WIN_BG0    (1<<0)
#define WIN_BG1    (1<<1)
#define WIN_BG2    (1<<2)
#define WIN_BG3    (1<<3)
#define WIN_OBJ    (1<<4)
#define WIN_BLEND  (1<<5)

#define WIN_ALL (WIN_BG0 | WIN_BG1 | WIN_BG2 | WIN_BG3 | WIN_OBJ)

#define WINDOW_WIN0 0xD
#define WINDOW_WIN1 0xE
#define WINDOW_OBJ  0xF


void setWinObjItems(u8 items)
{
	WIN_OUT = (WIN_OUT & 0x003F) | (items << 8);
}

void setWinOutItems(u8 items)
{
	WIN_OUT = (WIN_OUT & 0x3F00) | (items);
}

void setWin0Items(u8 items)
{
	WIN_IN = (WIN_IN & 0x3F00) | (items);
}

void setWin1Items(u8 items)
{
	WIN_IN = (WIN_IN & 0x003F) | (items << 8);
}

void activateWindow(u8 window)
{
	REG_DISPCNT |= (1 << (window));
}

void deactivateWindow(u8 window)
{
	REG_DISPCNT &= (~(1 << (window)));
}

void setWin0Coords(u8 x1, u8 x2, u8 y1, u8 y2)
{
	WIN0_X0 = x1;
	WIN0_X1 = x2;
	WIN0_Y0 = y1;
	WIN0_Y1 = y2;
}

void setWin1Coords(u8 x1, u8 x2, u8 y1, u8 y2)
{
	WIN1_X0 = x1;
	WIN1_X1 = x2;
	WIN1_Y0 = y1;
	WIN1_Y1 = y2;
}

void activateDarkMode() {
	if (getCurrentItem() == ITEM_LANTERN) {
		return;
	}
	mIsDark = true;

	setWinObjItems(WIN_ALL);
	setWinOutItems(WIN_BG0 | WIN_BG1);
	setWin0Items(WIN_ALL);
	setWin0Coords(213,234,8,24);

	activateWindow(WINDOW_OBJ);
	activateWindow(WINDOW_WIN0);


	if (isSubscreenShowing()) {
		activateSubscreenWindow();
	}
}

void hideItemWindow() {
	if (isDark()) {
		setWin0Coords(0,0,0,0);
	}
}

void showItemWindow() {
	deactivateSubscreenWindow();
}

void activateSubscreenWindow() {
	if (isDark()) {
		//do we need to do anything here?  don't think so...
		/*
		 SET_WIN0_COORDS((SUBSCREEN_X << 3) + 4, 
		 SUBSCREEN_X2 << 3, 
		 (SUBSCREEN_Y << 3) + 4,
		 SUBSCREEN_Y2 << 3);
		 */
	}

}

void deactivateSubscreenWindow() {
	if (isDark()) {
		//    		SET_WIN0_COORDS(213, 230, 5, 24);
	}

}

bool isDark() {
	return mIsDark;
}

void deactivateDarkMode() {
	setWinOutItems(WIN_ALL);
	deactivateWindow(WINDOW_OBJ);
	deactivateWindow(WINDOW_WIN0);
	deactivateWindow(WINDOW_WIN1);
	mIsDark = false;
	if (isSubscreenShowing()) {
		deactivateSubscreenWindow();
	}
}

bool isRoomDark() {
	LevelData * level = getCurrentLevel();
	return level->dark;

}


bool levelWillBeDark(LevelData * level)
{
	return ((level->dark) && (getCurrentItem() != ITEM_LANTERN));
}
