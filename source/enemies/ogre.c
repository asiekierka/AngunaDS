#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "ogreGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "enemies/portraits/ogre_portrait.h"

#define Frame(x) (void *)&(enemyOgreCharFrames[x])
#define Anim(x) (void *)&(enemyOgreCharAnim[x])

#define TILE(x) {0,-8,(x),TALL,SIZE_32, false, null}
#define TILEF(x) {0,-8,(x),TALL,SIZE_32, true, null}

enum
{
	OGRE_N_N,
	OGRE_N_A,
	OGRE_N_B,

	OGRE_S_N,
	OGRE_S_A,
	OGRE_S_B,

	OGRE_L_N,
	OGRE_L_A,
	OGRE_L_B,

	OGRE_R_N,
	OGRE_R_A,
	OGRE_R_B,

	OGRE_N_N2,
	OGRE_S_N2,
	OGRE_L_N2,
	OGRE_R_N2,

	OGRE_RUN_N_N,
	OGRE_RUN_N_A,
	OGRE_RUN_N_B,

	OGRE_RUN_S_N,
	OGRE_RUN_S_A,
	OGRE_RUN_S_B,

	OGRE_RUN_L_N,
	OGRE_RUN_L_A,
	OGRE_RUN_L_B,

	OGRE_RUN_R_N,
	OGRE_RUN_R_A,
	OGRE_RUN_R_B,

	OGRE_RUN_N_N2,
	OGRE_RUN_S_N2,
	OGRE_RUN_L_N2,
	OGRE_RUN_R_N2,

	
};

const characterFrameDef enemyOgreCharFrames[] = 
{
	[OGRE_N_N] = TILE(18),
	[OGRE_N_A] = TILE(20),
	[OGRE_N_B] = TILE(22),

	[OGRE_S_N] = TILE(12),
	[OGRE_S_A] = TILE(14),
	[OGRE_S_B] = TILE(16),

	[OGRE_L_N] = TILE(6),
	[OGRE_L_A] = TILE(8),
	[OGRE_L_B] = TILE(10),

	[OGRE_R_N] = TILEF(6),
	[OGRE_R_A] = TILEF(8),
	[OGRE_R_B] = TILEF(10),

};

#define walkN Anim(OGRE_N_N)
#define walkL Anim(OGRE_L_N)
#define walkR Anim(OGRE_R_N)
#define walkS Anim(OGRE_S_N)
#define runN  Anim(OGRE_RUN_N_N)
#define runL Anim(OGRE_RUN_L_N)
#define runR Anim(OGRE_RUN_R_N)
#define runS Anim(OGRE_RUN_S_N)

#define WALK_SPEED 30
#define RUN_SPEED 10

#define WALK(x,y) {WALK_SPEED,(u16*)ogreGfxTiles,Frame(x),Anim(y)}
#define RUN(x,y) {RUN_SPEED,(u16*)ogreGfxTiles,Frame(x),Anim(y)}

const characterAnimDef enemyOgreCharAnim[] = 
{
	[OGRE_N_N] =  WALK(OGRE_N_N,OGRE_N_A),
	[OGRE_N_A] =  WALK(OGRE_N_A,OGRE_N_N2),
	[OGRE_N_N2] = WALK(OGRE_N_N,OGRE_N_B),
	[OGRE_N_B] =  WALK(OGRE_N_B,OGRE_N_N),

	[OGRE_S_N] =  WALK(OGRE_S_N,OGRE_S_A),
	[OGRE_S_A] =  WALK(OGRE_S_A,OGRE_S_N2),
	[OGRE_S_N2] = WALK(OGRE_S_N,OGRE_S_B),
	[OGRE_S_B] =  WALK(OGRE_S_B,OGRE_S_N),

	[OGRE_L_N] =  WALK(OGRE_L_N,OGRE_L_A),
	[OGRE_L_A] =  WALK(OGRE_L_A,OGRE_L_N2),
	[OGRE_L_N2] = WALK(OGRE_L_N,OGRE_L_B),
	[OGRE_L_B] =  WALK(OGRE_L_B,OGRE_L_N),

	[OGRE_R_N] =  WALK(OGRE_R_N,OGRE_R_A),
	[OGRE_R_A] =  WALK(OGRE_R_A,OGRE_R_N2),
	[OGRE_R_N2] = WALK(OGRE_R_N,OGRE_R_B),
	[OGRE_R_B] =  WALK(OGRE_R_B,OGRE_R_N),

	[OGRE_RUN_N_N] =  RUN(OGRE_N_N,OGRE_RUN_N_A),
	[OGRE_RUN_N_A] =  RUN(OGRE_N_A,OGRE_RUN_N_N2),
	[OGRE_RUN_N_N2] = RUN(OGRE_N_N,OGRE_RUN_N_B),
	[OGRE_RUN_N_B] =  RUN(OGRE_N_B,OGRE_RUN_N_N),

	[OGRE_RUN_S_N] =  RUN(OGRE_S_N,OGRE_RUN_S_A),
	[OGRE_RUN_S_A] =  RUN(OGRE_S_A,OGRE_RUN_S_N2),
	[OGRE_RUN_S_N2] = RUN(OGRE_S_N,OGRE_RUN_S_B),
	[OGRE_RUN_S_B] =  RUN(OGRE_S_B,OGRE_RUN_S_N),

	[OGRE_RUN_L_N] =  RUN(OGRE_L_N,OGRE_RUN_L_A),
	[OGRE_RUN_L_A] =  RUN(OGRE_L_A,OGRE_RUN_L_N2),
	[OGRE_RUN_L_N2] = RUN(OGRE_L_N,OGRE_RUN_L_B),
	[OGRE_RUN_L_B] =  RUN(OGRE_L_B,OGRE_RUN_L_N),

	[OGRE_RUN_R_N] =  RUN(OGRE_R_N,OGRE_RUN_R_A),
	[OGRE_RUN_R_A] =  RUN(OGRE_R_A,OGRE_RUN_R_N2),
	[OGRE_RUN_R_N2] = RUN(OGRE_R_N,OGRE_RUN_R_B),
	[OGRE_RUN_R_B] =  RUN(OGRE_R_B,OGRE_RUN_R_N),



};





extern CharacterStatus * mcStatus;

#define OGRE_LEFT 0
#define OGRE_RIGHT 1
#define OGRE_UP 2
#define OGRE_DOWN 3
#define OGRE_DIR_MASK 0x4

#define OGRE_STATE_WALKING 1
#define OGRE_STATE_CHANGEUP 0
#define OGRE_STATE_RUNNING 2
#define OGRE_STATE_PAUSING 3

#define DIRECTION state1
#define DIRTIMER state2
#define PAUSETIMER state3
#define STATE state4

bool enemyOgreIsRed(Enemy * enemy);

int enemyOgreInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)ogreGfxTiles, (characterAnimDef*) enemyOgreCharAnim);
	enemy->STATE = 0;
	return 1;
}


int enemyOgreSetDirection(Enemy * enemy, int axis, int xa, int ya)
{
		if (axis == 0)
		{
			if (xa < 0)
			{
				enemy->DIRECTION = OGRE_LEFT;
			}
			else
			{
				enemy->DIRECTION = OGRE_RIGHT;
			}
		}
		else
		{
			if (ya < 0)
			{
				enemy->DIRECTION = OGRE_UP;
			}
			else
			{
				enemy->DIRECTION = OGRE_DOWN;
			}
		}

		return enemy->DIRECTION;
}

int enemyOgreUpdate(Enemy * enemy)
{

	enemy->PAUSETIMER--;
	//figure out which direction we're going if it's time to choose....
	if (enemy->STATE == OGRE_STATE_CHANGEUP)
	{

		int xa = mcStatus->x - enemy->status->x;
		int ya = mcStatus->y - enemy->status->y;
		int axis = dice(0,1);
		enemyOgreSetDirection(enemy, axis, xa, ya);
		enemy->STATE = OGRE_STATE_WALKING;
		enemy->PAUSETIMER = 15;
		enemy->DIRTIMER = 0;
	}

	int xd = 0;
	int yd = 0;


	int xa = mcStatus->x - enemy->status->x;
	int ya = mcStatus->y - enemy->status->y;


	if (enemy->STATE == OGRE_STATE_WALKING)
	{
		//if tired of walking forward, change modes
		switch(enemy->DIRECTION)
		{
			case OGRE_RIGHT: 
				xd = 1;
				setCharacterAnimation(enemy->status, walkR);
				break;
			case OGRE_DOWN:
				yd = 1;
				setCharacterAnimation(enemy->status, walkS);
				break;
			case OGRE_LEFT:
				xd = -1;
				setCharacterAnimation(enemy->status, walkL);
				break;
			case OGRE_UP:
				yd = -1;
				setCharacterAnimation(enemy->status, walkN);
				break;
		}

		enemy->DIRTIMER++;

		if (enemy->PAUSETIMER < 1)
		{
			if (!moveEnemy(enemy, xd, yd) || (enemy->DIRTIMER > dice(120, 300)))
			{
				enemy->DIRTIMER = 0;
				enemy->STATE = OGRE_STATE_CHANGEUP;
			}
			enemy->PAUSETIMER = 2;
		}


		//if lined up with player, change to wait mode, then go mode....

		if (abs(xa) < 15)
		{
			enemyOgreSetDirection(enemy, 1, 0, ya);
			enemy->STATE = OGRE_STATE_PAUSING;
			enemy->PAUSETIMER = 60;
		}
		else
		{
			if (abs(ya) < 15)
			{
				enemyOgreSetDirection(enemy, 0, xa, 0);
				enemy->STATE = OGRE_STATE_PAUSING;
				enemy->PAUSETIMER = 60;
			}
		}
	}

	if (enemy->STATE == OGRE_STATE_PAUSING)
	{
		switch(enemy->DIRECTION)
		{
			case OGRE_RIGHT: 
				setCharacterAnimation(enemy->status, runR);
				break;
			case OGRE_DOWN:
				setCharacterAnimation(enemy->status, runS);
				break;
			case OGRE_LEFT:
				setCharacterAnimation(enemy->status, runL);
				break;
			case OGRE_UP:
				setCharacterAnimation(enemy->status, runN);
				break;
		}

		if (enemy->PAUSETIMER < 1)
		{
			enemy->STATE = OGRE_STATE_RUNNING;

		}
	}

	int tmp;

	if (enemy->STATE == OGRE_STATE_RUNNING)
	{
		tmp = enemy->PAUSETIMER;
		if ((enemy->PAUSETIMER < 1) || (enemy->PAUSETIMER > 2))
		{
			enemy->PAUSETIMER = 2;
			tmp = enemy->PAUSETIMER;
		}
		switch(enemy->DIRECTION)
		{
			case OGRE_RIGHT: 
				xd = tmp;
				break;
			case OGRE_DOWN:
				yd = tmp;
				break;
			case OGRE_LEFT:
				xd = -tmp;
				break;
			case OGRE_UP:
				yd = -tmp;
				break;
		}
		
		if (enemyOgreIsRed(enemy))
		{
			switch(enemy->DIRECTION)
			{
				case OGRE_RIGHT: 
				case OGRE_LEFT:
					if (ya < 10)
					{
						yd--;
					}
					if (ya > 10)
					{
						yd++;
					}

					break;
				case OGRE_DOWN:
				case OGRE_UP:
					if (xa < 10)
					{
						xd--;
					}
					if (xa > 10)
					{
						xd++;
					}
					break;
			}
		}

		if (!moveEnemy(enemy, xd, yd))
		{
			enemy->STATE = OGRE_STATE_CHANGEUP;	
		}
	}


	return 1;
}


int enemyOgreFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyOgreInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyOgreUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_ogre_db = 
{
	ogre_portrait_Tiles,
	ogre_portrait_Pal,
	ogre_portrait_Map,
	"Green Toady",
	"Dim-witted Man-Toad",
	"Meanders slowly,",
	     "but charges",
		 "on sight.",
	7,7,8
};


const EnemyType enemy_ogre_def = 
{
	8,				//hp
	3,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	1,				//bbOffx
	1,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyOgreFunc,	//updateFunction
	(short * )&ogreGfxPal,
	&enemy_ogre_db
};

const EnemyDatabaseDef enemy_redogre_db = 
{
	ogre_portrait_Tiles,
	ogre_portrait_Pal + 16,
	ogre_portrait_Map,
	"Red Toady",
	"Angry Man-Toad",
	"Not quite as stupid ",
	     "as the green one",
	null,
	6,6,0
};


const EnemyType enemy_redogre_def = 
{
	24,				//hp
	9,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	1,				//bbOffx
	1,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyOgreFunc,	//updateFunction
	(short * )&ogreRedGfxPal,
	&enemy_redogre_db

};

bool enemyOgreIsRed(Enemy * enemy)
{
	return (enemy->enemyType == &enemy_redogre_def);
}

	
