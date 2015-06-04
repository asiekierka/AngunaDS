#include "../enemies.h"
#include "../characters.h"
#include "../spriteMgr.h"
#include "../level.h"
#include "../sound.h"
#include "../door.h"
#include "../mainChar.h"
#include "mapObjectGfx.h"

#define Frame(x) (void *)&(enemySwitchCharFrames[x])
#define Anim(x) (characterAnimDef *)&(enemySwitchCharAnim[x])

#define OFFSET 5

#define objFrameDef(x) {0,-OFFSET,(x),SQUARE,SIZE_16,false,null}
#define objFrameDefTall(x) {0,0,(x),TALL,SIZE_32,false,null}
#define objAnimDef(x) {99999,(u16*)map_objectGfxTiles,Frame(x),null}

enum
{
	NOTHING,
	SWITCH_UP,
	SWITCH_DOWN
};

#define NO_COLLISION 0
#define COLLISION 1

#define HIDDEN 0
#define VISIBLE 1

#define SWITCH_STATE state1
#define HIDE_STATE state2
#define REWARD_CTR state4
#define COLLIDE_STATE state3

const characterFrameDef enemySwitchCharFrames[] = 
{
	[NOTHING]     = objFrameDef(80),
	[SWITCH_UP]   = objFrameDef(76),
	[SWITCH_DOWN] = objFrameDef(78),
};

const characterAnimDef enemySwitchCharAnim[] = 
{
	[NOTHING] =        objAnimDef(NOTHING),
	[SWITCH_UP] =      objAnimDef(SWITCH_UP),
	[SWITCH_DOWN] =    objAnimDef(SWITCH_DOWN),
	
};


int enemySwitchInit(Enemy * enemy)
{

	enemy->REWARD_CTR = 0;
	enemy->SWITCH_STATE = SWITCH_UP;
	enemy->status->priority = CHAR_PRIORITY_MAP;

	if (enemy->special & ITEM_REWARD)
	{
		enemy->HIDE_STATE = HIDDEN;
		initEnemyGfx(enemy, (u16*)map_objectGfxTiles, (characterAnimDef * )&enemySwitchCharAnim[NOTHING]);
	}
	else
	{
		enemy->HIDE_STATE = VISIBLE;
		initEnemyGfx(enemy, (u16*)map_objectGfxTiles, (characterAnimDef * )&enemySwitchCharAnim[SWITCH_UP]);
	}

	return 0;
}


int enemySwitchBulletCollide(Enemy * enemy)
{
	return 0;
}

extern void enemyDoorOpen(Enemy * enemy);

void enemySwitchOpenDoor(Enemy * enemy)
{
	int i;
	for (i = 0; i < SWITCH_NUM_DOORS(enemy); i++)
	{
		enemy++;
		enemyDoorOpen(enemy);
	}
	
}


int enemySwitchCollide(Enemy * enemy)
{

	bool collide = false;
	
	if (enemy->HIDE_STATE == VISIBLE) 
	{
		collide = (checkCharacterCollidesWithMC(enemy->status));
	}

	enemy->COLLIDE_STATE = collide;


	//if it's a map level object, no collision 
	return NO_COLLISION;
}

int enemySwitchEnemyCollide(Enemy * enemy)
{
	return NO_COLLISION;
}


int enemySwitchDead(Enemy * enemy)
{
	resetEnemy(enemy);
	return 1;
	
}

int enemySwitchUpdate(Enemy * enemy)
{
	bool collide = enemy->COLLIDE_STATE;
	int toggleNum = SWITCH_GET_TOGGLE(enemy);

	if (collide) {
		if (enemy->SWITCH_STATE == SWITCH_UP)
		{
			//push button....
			enemy->SWITCH_STATE = SWITCH_DOWN;
			playGameSfx(SFX_SWITCH);
			setCharacterAnimation(enemy->status, Anim(SWITCH_DOWN));
			if (SWITCH_NUM_DOORS(enemy))
			{
				enemySwitchOpenDoor(enemy);
			}

			//change toggle status if it's a toggle switch
			if (toggleNum > 0)
			{
				toggleToggleState(toggleNum);
			}
		}
		enemy->COLLIDE_STATE = false;
	}
	else
	{

		//switch only comes back up if it's a toggle switch
		if (toggleNum > 0) 
		{
			//and only do toggley things if it was down last frame
			if (enemy->SWITCH_STATE == SWITCH_DOWN) 
			{
				enemy->SWITCH_STATE = SWITCH_UP;
				setCharacterAnimation(enemy->status, Anim(SWITCH_UP));
				playGameSfx(SFX_SWITCH);
			}

		}
	}

	if (enemy->HIDE_STATE == HIDDEN) 
	{
		if (enemy->special & ITEM_REWARD)
		{
			if (enemy->REWARD_CTR++ > 5)
			{
				enemy->REWARD_CTR = 0;
				if (numEnemiesAlive() == 0)
				{
					enemy->HIDE_STATE = VISIBLE;
					setCharacterAnimation(enemy->status, Anim(SWITCH_UP));
					playGameSfx(SFX_SWITCH);
				}
			}
		}
	}
	

	return 0;
}



int enemySwitchFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemySwitchInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemySwitchUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return enemySwitchDead(enemy);
			resetEnemy(enemy);
			return 1;
			break;
		case ENEMY_BULLET_COLLIDE:
			return enemySwitchBulletCollide(enemy);
			break;
		case ENEMY_MC_COLLIDE:
			return enemySwitchCollide(enemy);
			break;
		case ENEMY_ENEMY_COLLIDE:
			return enemySwitchEnemyCollide(enemy);
			break;
	}
	return 1;
}


const EnemyType enemy_switch_def = 
{
	1,				//hp
	0,				//collideDamage
	1,				//numSprites
	14 - OFFSET,	//bbHeight
	14,				//bbWidth
	1,				//bbOffx
	1,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemySwitchFunc,	//updateFunction
	(short*)&map_objectGfxPal,
	null
};

