#ifndef _HUD_H
#define _HUD_H

#include "spriteMgr.h"
#include "bgDefines.h"

//#define GOLD_HUD

void updateHud();
void showLife();
void forceHudRedraw();
void reloadHudPalette();
void hideHud();
Sprite * getHudItemSprite();
Sprite * getHudGoldSprite();


#endif
