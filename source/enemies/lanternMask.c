#include "../enemies.h"
#include "../spriteMgr.h"
#include "../characters.h"
#include "../mainChar.h"
#include "itemGfx.h"


#define Frame(x,y,f,n) {(x),(y),28,SQUARE,SIZE_32,(f),(void *)&(enemyLanternMaskCharFrames[n])}

const characterFrameDef enemyLanternMaskCharFrames[] = 
{
	Frame(-32,-32,FLIP_NONE,1),
	Frame(0,-32,FLIP_H_ONLY,2),
	Frame(-32,0,FLIP_V_ONLY,3),
	Frame(0,0,FLIP_BOTH,null),
};

const characterAnimDef enemyLanternMaskCharAnim[] = 
{
	{
		9999,
		(u16*)itemsTiles,
		(void *)&(enemyLanternMaskCharFrames[0]),
		null
	}
};

int enemyLanternMaskInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)itemsTiles, (characterAnimDef*) enemyLanternMaskCharAnim);
	enemy->status->priority = CHAR_PRIORITY_HIGH;
	int i;
	for(i = 0; i < enemy->status->listSize; i++)
	{
		Sprite * sprite = enemy->status->spriteList[i];
		setSpriteWindowed(sprite, true);
	}
	return 1;
}


int enemyLanternMaskDead(Enemy * enemy)
{
	resetEnemy(enemy);
	return 1;
}

int enemyLanternMaskUpdate(Enemy * enemy)
{
	
	enemy->status->x = getMainCharX();
	enemy->status->y = getMainCharY();
	return 1;
}

int enemyLanternMaskFunc(Enemy * enemy, int func)
{
	switch (func)
	{
		case ENEMY_INIT:
			return enemyLanternMaskInit(enemy);
			break;

		case ENEMY_DEAD:
			return enemyLanternMaskDead(enemy);
			break;

		case ENEMY_UPDATE:
			return enemyLanternMaskUpdate(enemy);

		case ENEMY_BULLET_COLLIDE:
		case ENEMY_MC_COLLIDE:
		case ENEMY_ENEMY_COLLIDE:
			return 0;
	}

	return 1;
}

const EnemyType enemy_lanternMask_def = 
{
	1,				//hp
	0,				//collideDamage
	4,				//numSprites
	0,				//bbHeight
	0,				//bbWidth
	0,				//bbOffx
	0,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyLanternMaskFunc,	//updateFunction
	(short*)&itemsPal,
	null
};
