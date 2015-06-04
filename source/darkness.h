#ifndef _DARKNESS_H_
#define _DARKNESS_H_

#include "spriteMgr.h"
#include "level.h"

void activateDarkMode();
void deactivateDarkMode();

void activateSubscreenWindow();
void deactivateSubscreenWindow();

bool isDark();
bool isRoomDark();

void hideItemWindow();
void showItemWindow();

bool levelWillBeDark(LevelData * level);


#endif
