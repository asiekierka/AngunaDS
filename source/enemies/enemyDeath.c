#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "../misc.h"
#include "../characters.h"
#include "../item.h"
#include "itemGfx.h"
#include "enemyDeathGfx.h"

#define DEATH_SPEED 2

#define Frame(x) (void *)&(enemyDeathCharFrames[x])
#define Anim(x) (void *)&(enemyDeathCharAnim[x])

#define TILE(x) {-16,0,(x)*4 + 64,WIDE,SIZE_32,false,null}

#define ATILE(x) {DEATH_SPEED, (u16*)enemyDeathGfxTiles, Frame(x), Anim(x+1)}

const characterFrameDef enemyDeathCharFrames[] =
{
	TILE(0),
	TILE(1),
	TILE(2),
	TILE(3),
	TILE(4),
	TILE(5),
	TILE(6),
	TILE(7),
};


const characterAnimDef enemyDeathCharAnim[] = 
{
	ATILE(0),
	ATILE(1),
	ATILE(2),
	ATILE(3),
	ATILE(4),
	ATILE(5),
	ATILE(6),
	ATILE(7),
	{DEATH_SPEED,(u16*)enemyDeathGfxTiles, Frame(7), Anim(8)},
};


int enemyDeathInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)enemyDeathGfxTiles, (characterAnimDef*) enemyDeathCharAnim);
	
	return 1;
}

int enemyDeathUpdate(Enemy * enemy)
{
	enemy->state1++;
	if (enemy->state1 == DEATH_SPEED * 8)
	{
		//don't kill the enemy (which produces the death animation)
		//instead reset it (clears it to nothing)
		if (enemy->itemDrop != ITEM_NONE)
		{
			releaseMultipleSprites(enemy->status->spriteList, enemy->status->listSize);
			createNewEnemyWithSlot((EnemyType *)&(enemy_item_def), enemy->status->x, enemy->status->y,
						enemy, enemy->status, enemy->itemDrop, 0);
		}
		else
		{
			resetEnemy(enemy);		
		}
		return 1;
	}
	return 0;
}


int enemyDeathFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyDeathInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyDeathUpdate(enemy);
			return 1;
			break;
		case ENEMY_DEAD:
			resetEnemy(enemy);
			return 1;
			break;
		case ENEMY_MC_COLLIDE:
			return 0;		//collision with this does nothing
			break;
	}
	return 1;
}

const EnemyDatabaseDef enemy_death_db = 
{
	null,
	null,
	null,
	"enemy death"
};


const EnemyType enemy_death_def = 
{
	1,				//hp
	0,				//collideDamage
	1,				//numSprites
	-999,			//bbHeight
	-999,			//bbWidth
	0,				//bbOffx
	0,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyDeathFunc,	//updateFunction
	(short *)&itemsPal,
	&enemy_death_db
};


