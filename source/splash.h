#ifndef _SPLASH_H_
#define _SPLASH_H_

#include "misc.h"

void showSplashScreen(void * gfx, void * pal);
void fadeOutSplash(int fadeSpeed);
void fadeOutSplashes(int fadeSpeed);
void showTextScreen(char ** lines);
void addText(char ** lines);
int doGameOver();
u16 * showTileSplashScreen(void * gfx, void * pal, void * map, bool fadeIn);
void showTileSplashScreenSub(void * gfx, void * pal, void * map, int yAdjust);
void showChiliBite(void * gfx, void * pal, void * map);
void fadeInSplash(int fadeSpeed);
void fadeInSplashScreens(int fadeSpeed, bool both);
void setFadeBlack();
void setSubFadeBlack();
void doDemoOver();
void doCredits();
int mainMenu(bool continueSelected);
int getGameOverChoice();
void resetMainMenu();

#define NEW_GAME 0
#define CONTINUE 1
#define NO_CHOICE 2

#endif
