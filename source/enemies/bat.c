#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "slimeGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "enemies/portraits/bat_portrait.h"

#define Frame(x) (void *)&(enemyBatCharFrames[x])
#define Anim(x) (void *)&(enemyBatCharAnim[x])

#define XDIR state1
#define YDIR state2


const characterFrameDef enemyBatCharFrames[] = 
{
	{//0
		FRAME_SMALL_SQUARE(12,false)
	},
	{//1
		FRAME_SMALL_SQUARE(14,false)
	},
	{//2
		FRAME_SMALL_SQUARE(16,false)
	},
};

#define ANIM_ENTRY(x,y) 10,(u16*)slimeGfxData,Frame(x),Anim(y)

const characterAnimDef enemyBatCharAnim[] = 
{
	{//0 - frame 1 
		ANIM_ENTRY(0,1)
	},
	{//1 - frame 2
		ANIM_ENTRY(1,2)
	},
	{//2 - frame 3
		ANIM_ENTRY(2,3)
	},
	{//3 - frame 4
		ANIM_ENTRY(1,0)
	},
};




int enemyBatInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)slimeGfxData, (characterAnimDef*) enemyBatCharAnim);
	enemy->status->mobility = MOBILITY_FLYING;
	enemy->XDIR = dice(0,1);
	if (enemy->XDIR == 0)
	{
		enemy->XDIR = -1;
	}
	enemy->YDIR = dice(0,1);
	if (enemy->YDIR == 0)
	{
		enemy->YDIR = -1;
	}
	return 1;
}

extern CharacterStatus * mcStatus;

void enemyBatSetDir(Enemy * enemy, int x, int y)
{
	if (x != 0)
	{
		enemy->XDIR = x;
	}
	if (y!= 0)
	{
		enemy->YDIR = y;
	}
}

int enemyBatUpdate(Enemy * enemy)
{
	if((enemy->state3++) == 0)
	{
		return 1;
	}

	enemy->state3 = 0;
	
	if (!moveEnemy(enemy, enemy->XDIR, enemy->YDIR))
	{
		if (!moveEnemy(enemy, enemy->XDIR, 0))
		{
			enemy->XDIR =- enemy->XDIR;
		}
		if (!moveEnemy(enemy, 0, enemy->YDIR))
		{
			enemy->YDIR =- enemy->YDIR;
		}
	}


	return 1;
}

int enemyBatFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyBatInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyBatUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_bat_db = 
{
	bat_portrait_Tiles,
	bat_portrait_Pal,
	bat_portrait_Map,
	"Dark Bat",
	"An angry vampire bat",
	"Can fly over obstacles",
	     "",
	null,
	0,1,0
};


const EnemyType enemy_bat_def = 
{
	1,				//hp
	2,				//collideDamage
	1,				//numSprites
	12,				//bbHeight
	12,				//bbWidth
	3,				//bbOffx
	3,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyBatFunc,	//updateFunction
	(short*)&bat1Pal,
	&enemy_bat_db
};
	
