#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "../misc.h"
#include "../characters.h"
#include "crabGfx.h"
#include "portraits/crabBoss_portrait.h"

#define Frame(x) (void *)&(enemyCrabCharFrames[x])
#define Anim(x) (void *)&(enemyCrabCharAnim[x])

#define XDIR state1
#define YDIR state2

#define CRAB_DIR state1
#define BORED_CTR state2

#define CRAB_CHANGEUP_TIME 120


const characterFrameDef enemyCrabCharFrames[] = 
{
	{ FRAME_SMALL_SQUARE(30,false) },
	{-24,-16,0,WIDE,SIZE_64,false,null},
	{-40,-16,6,WIDE,SIZE_64,false,null},
	{-24,-16,14,WIDE,SIZE_64,false,null},
	{-40,-16,20,WIDE,SIZE_64,false,null},
};

#define ANIM_ENTRY(x,y) 12,(u16*)crabGfxTiles,Frame(x),Anim(y)

const characterAnimDef enemyCrabCharAnim[] = 
{
	{//0 - blob
		ANIM_ENTRY(0,0)
	},
	{ANIM_ENTRY(1,2)},
	{ANIM_ENTRY(2,3)},
	{ANIM_ENTRY(3,4)},
	{ANIM_ENTRY(4,5)},
	{ANIM_ENTRY(3,6)},
	{ANIM_ENTRY(2,1)},
};



/////////////////FIRST THE BLOBS////////////////////////////////

int enemyCrabBlobInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)crabGfxTiles, (characterAnimDef*) enemyCrabCharAnim);
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



void enemyCrabBlobSetDir(Enemy * enemy, int x, int y)
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

int enemyCrabBlobUpdate(Enemy * enemy)
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

int enemyCrabBlobFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyCrabBlobInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyCrabBlobUpdate(enemy);
			break;
		case ENEMY_BULLET_COLLIDE:
			return 0;
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyType enemy_crabblob_def = 
{
	1,				//hp
	12,				//collideDamage
	1,				//numSprites
	12,				//bbHeight
	12,				//bbWidth
	3,				//bbOffx
	3,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyCrabBlobFunc,	//updateFunction
	(short*)&crabBubbleGfxPal,
	null
};





////////////////////////CRAB ITSELF///////////////////////////////

int enemyCrabInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)crabGfxTiles, (characterAnimDef*) &(enemyCrabCharAnim[1]));
	enemy->CRAB_DIR = dice(0,3);
	enemy->BORED_CTR = 0;
	return 1;
}



void enemyCrabChangeDir(Enemy * enemy)
{
	int lastDir = enemy->CRAB_DIR;
	int newDir = dice(0,2);
	if (newDir >= lastDir)
	{
		newDir++;
	}
	enemy->CRAB_DIR = newDir;
}


int enemyCrabUpdate(Enemy * enemy)
{
	enemy->status->mobility = MOBILITY_FLYING;
	enemy->BORED_CTR++;
	if (enemy->BORED_CTR + dice(0,45) > CRAB_CHANGEUP_TIME)
	{
		enemy->BORED_CTR = 0;
		enemyCrabChangeDir(enemy);
	}

	if (moveEnemyDirection(enemy, enemy->CRAB_DIR) != NO_COLLISION)
	{
		//enemyCrabChangeDir(enemy);
	}

	return 1;
}



//////////////////TO KILL OFF THE BLOBS WHEN HE DIES/////////////////


void enemyCrabKillBlobs(Enemy * enemy)
{
	Enemy * blob = enemy;
	blob++;
	while ((blob->enemyType) == &(enemy_crabblob_def))
	{
		blob->enemyType->updateFunction(blob, ENEMY_DEAD);
		blob++;
	}
}




//////////////////NOW THE MASTER FUNCTIONS/////////////////////



int enemyCrabFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyCrabInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyCrabUpdate(enemy);
			break;
		case ENEMY_DEAD:
			enemyCrabKillBlobs(enemy);
			return killEnemy(enemy);
			break;
	}
	return 1;
}

const EnemyDatabaseDef enemy_crab_db = 
{
	crabBoss_portrait_Tiles,
	crabBoss_portrait_Pal,
	crabBoss_portrait_Map,
	"Giant Enemy Crab",
	"Causes Massive Damage",
	"",
	     "",
	null,
	14,14,14
};





const EnemyType enemy_crab_def = 
{
	100,				//hp
	17,				//collideDamage
	1,				//numSprites
	24,				//bbHeight
	48,				//bbWidth,
	-24,				//bbOffx
	-8,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyCrabFunc,	//updateFunction
	(short*)&crabGfxPal,
	&enemy_crab_db
};
	
	
