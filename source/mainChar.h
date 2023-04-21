#ifndef _MAINCHAR_H_
#define _MAINCHAR_H_

#include "bullet.h"
#include "characters.h"



void initMainChar();
void releaseMainCharSprites();
void updateMainChar();
void processControls();


void setMainCharPos(int x, int y);
void mainCharAddInventory(int item, int qty);
int mainCharInventory(int item);
int getMainCharX();
int getMainCharY();
int getMainCharPower();
int getMainCharBasePower();
int getMainCharArmor();
int getMainCharGold();
int getMainCharMaxHP();
int getMainCharHP();
int updateMCDisplay();
void mcUnstick();
void reloadMCGfx();
void setAttackBonus(int bonus);

int checkBulletCollidesWithMC(BulletDef * bullet);
bool checkCharacterCollidesWithMC(CharacterStatus * character);
void mcTakeHit(int dmg, int srcX, int srcY);
CharacterStatus * getMainCharStatus();

extern u16 * gfxToReload;

#define AXIS_X 2
#define AXIS_Y 1

void returnMainCharToLastGoodPosition(int axis);
void ensureMCOnOwnerList();


int mainCharDefenceUp();
int mainCharLifeUp();
int mainCharAttackUp();
int mainCharMeatPickup();
void mcAddHP(int delta);
void addMainCharGold(int amount);

int mcXFacing();
int mcYFacing();

bool isMCAttacking();

void loadMCFromSave(int hp, int hpMax, int power, int armor, int x, int y, int gold);
int getPercentComplete();

void setStealButtons();
bool checkAndClearStealButtons();

#endif
