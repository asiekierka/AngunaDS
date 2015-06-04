#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "multiSmall2Gfx.h"
#include "../misc.h"
#include "../characters.h"
#include "portraits/spider_portrait.h"

#define Frame(x) (void *)&(enemySpiderCharFrames[x])
#define Anim(x) (void *)&(enemySpiderCharAnim[x])

#define GFX (u16*)multiSmall2GfxTiles

#define XDIR state1
#define YDIR state2
#define BORED_CTR state3
#define RUNNING state4

#define BORED_APPROX 30


const characterFrameDef enemySpiderCharFrames[] = 
{
	[ENEMY_N_A] = {FRAME_SMALL_SQUARE(72,false)},
	[ENEMY_N_B] = {FRAME_SMALL_SQUARE(74,false)},
	[ENEMY_S_A] = {FRAME_SMALL_SQUARE(8,false)},
	[ENEMY_S_B] = {FRAME_SMALL_SQUARE(10,false)},
	[ENEMY_E_A] = {FRAME_SMALL_SQUARE(12,true)},
	[ENEMY_E_B] = {FRAME_SMALL_SQUARE(14,true)},
	[ENEMY_W_A] = {FRAME_SMALL_SQUARE(12,false)},
	[ENEMY_W_B] = {FRAME_SMALL_SQUARE(14,false)},
};

#define ANIM_ENTRY(x,y) {10,GFX,Frame(x),Anim(y)}

const characterAnimDef enemySpiderCharAnim[] = 
{
	[ENEMY_N_A] = ANIM_ENTRY(ENEMY_N_A, ENEMY_N_B),
	[ENEMY_N_B] = ANIM_ENTRY(ENEMY_N_B, ENEMY_N_A),
	[ENEMY_S_A] = ANIM_ENTRY(ENEMY_S_A, ENEMY_S_B),
	[ENEMY_S_B] = ANIM_ENTRY(ENEMY_S_B, ENEMY_S_A),
	[ENEMY_E_A] = ANIM_ENTRY(ENEMY_E_A, ENEMY_E_B),
	[ENEMY_E_B] = ANIM_ENTRY(ENEMY_E_B, ENEMY_E_A),
	[ENEMY_W_A] = ANIM_ENTRY(ENEMY_W_A, ENEMY_W_B),
	[ENEMY_W_B] = ANIM_ENTRY(ENEMY_W_B, ENEMY_W_A),
};



void enemySpiderChooseDirection(Enemy * enemy)
{
	int axis = dice(0,1);
	int dir = dice(0,1);
	if (dir == 0)
	{
		dir = -1;
	}
	if (axis == 0)
	{
		enemy->XDIR = dir;
		enemy->YDIR = 0;
		if (dir == 1)
		{
			setCharacterAnimation(enemy->status, Anim(ENEMY_E_A));
		}
		else
		{
			setCharacterAnimation(enemy->status, Anim(ENEMY_W_A));
		}

	}
	else
	{
		enemy->YDIR = dir;
		enemy->XDIR = 0;
		if (dir == 1)
		{
			setCharacterAnimation(enemy->status, Anim(ENEMY_S_A));
		}
		else
		{
			setCharacterAnimation(enemy->status, Anim(ENEMY_N_A));
		}
	}

	
}


int enemySpiderInit(Enemy * enemy)
{
	initEnemyGfx(enemy, GFX, (characterAnimDef*) enemySpiderCharAnim);
	enemySpiderChooseDirection(enemy);
	enemy->RUNNING = 0;
	return 1;
}



int enemySpiderUpdate(Enemy * enemy)
{
	enemy->BORED_CTR++;
	
	if (!enemy->RUNNING)
	{
		if (enemy->BORED_CTR > BORED_APPROX)
		{
			enemy->RUNNING = true;
			enemy->BORED_CTR = 0;
		}
	}
	else
	{
		if (enemy->BORED_CTR > BORED_APPROX + dice(0,120))
		{
			enemySpiderChooseDirection(enemy);
			enemy->RUNNING = false;
			enemy->BORED_CTR = 0;
		}
		else
		{

			if (!moveEnemy(enemy, enemy->XDIR, enemy->YDIR))
			{
				enemySpiderChooseDirection(enemy);
				enemy->RUNNING = false;
				enemy->BORED_CTR = 0;
			}

		}
	}


	return 1;
}

int enemySpiderFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemySpiderInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemySpiderUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_spider_db = 
{
	spider_portrait_Tiles,
	spider_portrait_Pal,
	spider_portrait_Map,
	"Spider",
	"An overgrown spider",
	"Venomous and hungry",
	     "",
	null,
	0,1,0
};


const EnemyType enemy_spider_def = 
{
	2,				//hp
	3,				//collideDamage
	1,				//numSprites
	12,				//bbHeight
	12,				//bbWidth
	3,				//bbOffx
	3,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemySpiderFunc,	//updateFunction
	(short*)&spiderPal,
	&enemy_spider_db
};
	
