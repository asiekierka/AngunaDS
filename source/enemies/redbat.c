#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "slimeGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "enemies/portraits/bat_portrait.h"

#define Frame(x) (void *)&(enemyRedBatCharFrames[x])
#define Anim(x) (void *)&(enemyRedBatCharAnim[x])

#define XDIR state1
#define YDIR state2
#define BORED_CTR state3

#define BORED_APPROX 30


const characterFrameDef enemyRedBatCharFrames[] = 
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

const characterAnimDef enemyRedBatCharAnim[] = 
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



void enemyRedBatChooseDirection(Enemy * enemy)
{
	enemy->XDIR = dice(0,2) - 1;
	enemy->YDIR = dice(0,2) - 1;
	
}


int enemyRedBatInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)slimeGfxData, (characterAnimDef*) enemyRedBatCharAnim);
	enemy->status->mobility = MOBILITY_FLYING;
	enemyRedBatChooseDirection(enemy);
	return 1;
}

extern CharacterStatus * mcStatus;



void enemyRedBatSetDir(Enemy * enemy, int x, int y)
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

int enemyRedBatUpdate(Enemy * enemy)
{
	enemy->BORED_CTR++;

	if (enemy->BORED_CTR > BORED_APPROX + dice(0,120))
	{
		enemyRedBatChooseDirection(enemy);
		enemy->BORED_CTR = 0;
	}
	
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

int enemyRedBatFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyRedBatInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyRedBatUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_redbat_db = 
{
	bat_portraitTiles,
	bat_portraitPal + 16,
	bat_portraitMap,
	"Red Bat",
	"An angrier vampire bat",
	"Can fly over obstacles",
	     "",
	null,
	0,1,0
};


const EnemyType enemy_redbat_def = 
{
	5,				//hp
	4,				//collideDamage
	1,				//numSprites
	12,				//bbHeight
	12,				//bbWidth
	3,				//bbOffx
	3,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyRedBatFunc,	//updateFunction
	(short*)&redSlimePal,
	&enemy_redbat_db
};
	
