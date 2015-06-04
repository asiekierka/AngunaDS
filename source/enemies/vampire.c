#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "vampireGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "portraits/vampire_portrait.h"

#define Frame(x) (void *)&(enemyVampireCharFrames[x])
#define Anim(x) (void *)&(enemyVampireCharAnim[x])

#define TILE(x) {0,-8,(x),TALL,SIZE_32, false, null}
#define TILEF(x) {0,-8,(x),TALL,SIZE_32, true, null}

#define DARK_COLLIDE 7

#define ISDARK(_enemy) (((_enemy)->enemyType->collideDamage)==DARK_COLLIDE)

enum
{
	VAMPIRE_N_A,
	VAMPIRE_N_B,

	VAMPIRE_S_A,
	VAMPIRE_S_B,

	VAMPIRE_L_A,
	VAMPIRE_L_B,

	VAMPIRE_R_A,
	VAMPIRE_R_B,

	VAMPIRE_RUN_N_A,
	VAMPIRE_RUN_N_B,

	VAMPIRE_RUN_S_A,
	VAMPIRE_RUN_S_B,

	VAMPIRE_RUN_L_A,
	VAMPIRE_RUN_L_B,

	VAMPIRE_RUN_R_A,
	VAMPIRE_RUN_R_B,

	
};

const characterFrameDef enemyVampireCharFrames[] = 
{
	[VAMPIRE_N_A] = TILE(8),
	[VAMPIRE_N_B] = TILE(10),

	[VAMPIRE_S_A] = TILE(4),
	[VAMPIRE_S_B] = TILE(6),

	[VAMPIRE_L_A] = TILE(0),
	[VAMPIRE_L_B] = TILE(2),

	[VAMPIRE_R_A] = TILEF(0),
	[VAMPIRE_R_B] = TILEF(2),

};

#define walkN Anim(VAMPIRE_N_A)
#define walkL Anim(VAMPIRE_L_A)
#define walkR Anim(VAMPIRE_R_A)
#define walkS Anim(VAMPIRE_S_A)
#define runN  Anim(VAMPIRE_RUN_N_A)
#define runL Anim(VAMPIRE_RUN_L_A)
#define runR Anim(VAMPIRE_RUN_R_A)
#define runS Anim(VAMPIRE_RUN_S_A)

#define WALK_SPEED 30
#define RUN_SPEED 10

#define DIRECTION state1
#define BORED_CTR state2
#define STEP_CTR state3
#define BAT_TIMER state4

#define BAT_CUTOFF 10
#define BAT_DELAY 30

#define WALK(x,y) {WALK_SPEED,(u16*)vampireGfxTiles,Frame(x),Anim(y)}
#define RUN(x,y) {RUN_SPEED,(u16*)vampireGfxTiles,Frame(x),Anim(y)}

const characterAnimDef enemyVampireCharAnim[] = 
{
	[VAMPIRE_N_A] =  WALK(VAMPIRE_N_A,VAMPIRE_N_B),
	[VAMPIRE_N_B] =  WALK(VAMPIRE_N_B,VAMPIRE_N_A),

	[VAMPIRE_S_A] =  WALK(VAMPIRE_S_A,VAMPIRE_S_B),
	[VAMPIRE_S_B] =  WALK(VAMPIRE_S_B,VAMPIRE_S_A),

	[VAMPIRE_L_A] =  WALK(VAMPIRE_L_A,VAMPIRE_L_B),
	[VAMPIRE_L_B] =  WALK(VAMPIRE_L_B,VAMPIRE_L_A),

	[VAMPIRE_R_A] =  WALK(VAMPIRE_R_A,VAMPIRE_R_B),
	[VAMPIRE_R_B] =  WALK(VAMPIRE_R_B,VAMPIRE_R_A),

	[VAMPIRE_RUN_N_A] =  RUN(VAMPIRE_N_A,VAMPIRE_RUN_N_B),
	[VAMPIRE_RUN_N_B] =  RUN(VAMPIRE_N_B,VAMPIRE_RUN_N_A),

	[VAMPIRE_RUN_S_A] =  RUN(VAMPIRE_S_A,VAMPIRE_RUN_S_B),
	[VAMPIRE_RUN_S_B] =  RUN(VAMPIRE_S_B,VAMPIRE_RUN_S_A),

	[VAMPIRE_RUN_L_A] =  RUN(VAMPIRE_L_A,VAMPIRE_RUN_L_B),
	[VAMPIRE_RUN_L_B] =  RUN(VAMPIRE_L_B,VAMPIRE_RUN_L_A),

	[VAMPIRE_RUN_R_A] =  RUN(VAMPIRE_R_A,VAMPIRE_RUN_R_B),
	[VAMPIRE_RUN_R_B] =  RUN(VAMPIRE_R_B,VAMPIRE_RUN_R_A),

};




int enemyVampireInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)vampireGfxTiles, (characterAnimDef*) enemyVampireCharAnim);
	enemy->DIRECTION = 0;
	return 1;
}

extern CharacterStatus * mcStatus;
extern const EnemyType enemy_bat_def;
extern const EnemyType enemy_redbat_def;
extern void enemyBatSetDir(Enemy * enemy, int x, int y);

#define VAMPIRE_LEFT 0
#define VAMPIRE_RIGHT 1
#define VAMPIRE_UP 2
#define VAMPIRE_DOWN 3
#define VAMPIRE_DIR_MASK 0x4

#define VAMPIRE_STATE_WALKING 1
#define VAMPIRE_STATE_CHANGEUP 0
#define VAMPIRE_STATE_BAT 2
#define VAMPIRE_STATE_PAUSING 3

int enemyVampireSetDirection(Enemy * enemy, int axis, int xa, int ya)
{
		if (axis == 0)
		{
			if (xa < 0)
			{
				enemy->DIRECTION = VAMPIRE_LEFT;
			}
			else
			{
				enemy->DIRECTION = VAMPIRE_RIGHT;
			}
		}
		else
		{
			if (ya < 0)
			{
				enemy->DIRECTION = VAMPIRE_UP;
			}
			else
			{
				enemy->DIRECTION = VAMPIRE_DOWN;
			}
		}

		return enemy->DIRECTION;
}

void launchBat(Enemy * enemy)
{
	if (numEnemiesAlive() >= BAT_CUTOFF)
	{
		return;
	}

	enemy->BAT_TIMER++;
	if (enemy->BAT_TIMER < BAT_DELAY)
	{
		return;
	}

	enemy->BAT_TIMER = 0;

	EnemyType * batType = (EnemyType *)&enemy_bat_def;

	if (ISDARK(enemy))
	{
		batType = (EnemyType * )&enemy_redbat_def;
		
	}

	Enemy * bat = createNewEnemy(batType, enemy->status->x, enemy->status->y, 0, 0);
	switch (enemy->DIRECTION)
	{
		case VAMPIRE_LEFT:
			enemyBatSetDir(bat, -1, 0);
			break;

		case VAMPIRE_RIGHT:
			enemyBatSetDir(bat, 1, 0);
			break;

		case VAMPIRE_UP:
			enemyBatSetDir(bat, 0, -1);
			break;

		case VAMPIRE_DOWN:
			enemyBatSetDir(bat, 0, 1);
			break;
	}
	
}


int enemyVampireUpdate(Enemy * enemy)
{

	enemy->STEP_CTR--;
	//figure out which direction we're going if it's time to choose....
	if (enemy->BORED_CTR == VAMPIRE_STATE_CHANGEUP)
	{

		int xa = mcStatus->x - enemy->status->x;
		int ya = mcStatus->y - enemy->status->y;
		int axis = dice(0,1);
		enemyVampireSetDirection(enemy, axis, xa, ya);
		enemy->STEP_CTR = 15;
		enemy->BORED_CTR = 0;
	}

	int xd = 0;
	int yd = 0;



		//if tired of walking forward, change modes
		switch(enemy->DIRECTION)
		{
			case VAMPIRE_RIGHT: 
				xd = 1;
				setCharacterAnimation(enemy->status, walkR);
				break;
			case VAMPIRE_DOWN:
				yd = 1;
				setCharacterAnimation(enemy->status, walkS);
				break;
			case VAMPIRE_LEFT:
				xd = -1;
				setCharacterAnimation(enemy->status, walkL);
				break;
			case VAMPIRE_UP:
				yd = -1;
				setCharacterAnimation(enemy->status, walkN);
				break;
		}

		enemy->BORED_CTR++;

		if (enemy->STEP_CTR < 1)
		{
			if (!moveEnemy(enemy, xd, yd) || (enemy->BORED_CTR > dice(120, 300)))
			{
				enemy->BORED_CTR = VAMPIRE_STATE_CHANGEUP;
			}
			enemy->STEP_CTR = 2;
		}


		//if lined up with player, change to wait mode, then go mode....

		int xa = mcStatus->x - enemy->status->x;
		int ya = mcStatus->y - enemy->status->y;
		if (abs(xa) < 15)
		{
			launchBat(enemy);
		}
		else
		{
			if (abs(ya) < 15)
			{
			launchBat(enemy);
			}
		}

	return 1;
}


int enemyVampireFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyVampireInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyVampireUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_vampire_db = 
{
	vampire_portrait_Tiles,
	vampire_portrait_Pal,
	vampire_portrait_Map,
	"Vampire",
	"Master of bats",
	"Meanders slowly, ",
	     "spawning bats",
	null,
	7,8,0
};


const EnemyType enemy_vampire_def = 
{
	12,				//hp
	5,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	1,				//bbOffx
	1,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyVampireFunc,	//updateFunction
	(short * )&vampireGfxPal,
	&enemy_vampire_db
};

const EnemyDatabaseDef enemy_darkvampire_db = 
{
	vampire_portrait_Tiles,
	vampire_portrait_Pal + 16,
	vampire_portrait_Map,
	"Dark Vampire",
	"Master of bats",
	"Meanders quickly, ",
	     "spawning bats",
	null,
	7,8,0
};


const EnemyType enemy_darkvampire_def = 
{
	25,				//hp
	DARK_COLLIDE,   //collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	1,				//bbOffx
	1,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyVampireFunc,	//updateFunction
	(short * )&darkVampireGfxPal,
	&enemy_darkvampire_db
};
	
