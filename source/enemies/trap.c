#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "multiSmall2Gfx.h"
#include "../misc.h"
#include "../characters.h"

#define TRAP_PAL trapPal
#define TRAP_GFX multiSmall2GfxTiles


#define Frame(x) (void *)&(enemyTrapCharFrames[x])
#define Anim(x) (void *)&(enemyTrapCharAnim[x])

#define XDIR state1
#define YDIR state2


const characterFrameDef enemyTrapCharFrames[] = 
{
	{//0
		FRAME_SMALL_SQUARE(30,false)
	},
};

#define ANIM_ENTRY(x,y) 9999,(u16*)TRAP_GFX,Frame(x),Anim(y)

const characterAnimDef enemyTrapCharAnim[] = 
{
	{//0 - frame 1 
		ANIM_ENTRY(0,0)
	},
};




int enemyTrapInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)TRAP_GFX, (characterAnimDef*) enemyTrapCharAnim);
	enemy->status->mobility = MOBILITY_FLYING;
	enemy->XDIR = 0;
	enemy->YDIR = 0;
	int speed = enemy->hp;
	switch (enemy->special)
	{
		case ENEMY_DIR_E:
			enemy->XDIR = speed;
			break;
		case ENEMY_DIR_W:
			enemy->XDIR = -speed;
			break;
		case ENEMY_DIR_N:
			enemy->YDIR = -speed;
			break;
		case ENEMY_DIR_S:
			enemy->YDIR = speed;
			break;
	}
	return 1;
}



int enemyTrapUpdate(Enemy * enemy)
{
	if (!moveEnemy(enemy, enemy->XDIR, enemy->YDIR))
	{
		enemy->XDIR =- enemy->XDIR;
		enemy->YDIR =- enemy->YDIR;
	}

	return 1;
}

int enemyTrapFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyTrapInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyTrapUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
		//traps don't take any damage...
		case ENEMY_BULLET_COLLIDE:
			return 0;
			break;
		case ENEMY_DYNAMITED:
			return enemy->hp = 0;
			break;
	}
	return 1;
}


const EnemyType enemy_trap_def = 
{
	5,				//hp
	6,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	1,				//bbOffx
	1,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyTrapFunc,	//updateFunction
	(short*)&TRAP_PAL,
	null
};

const EnemyType enemy_trap2_def = 
{
	3,				//hp
	6,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	1,				//bbOffx
	1,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyTrapFunc,	//updateFunction
	(short*)&TRAP_PAL,
	null
};

const EnemyType enemy_trap3_def = 
{
	3,				//hp
	6,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	1,				//bbOffx
	1,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyTrapFunc,	//updateFunction
	(short*)&TRAP_PAL,
	null
};
	
