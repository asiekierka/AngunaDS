#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "../misc.h"
#include "../characters.h"
#include "multiSmall2Gfx.h"
#include "enemies/portraits/snake_portrait.h"

#define Frame(x) (void *)&(enemySnakeCharFrames[x])
#define Anim(x) (void *)&(enemySnakeCharAnim[x])

#define GFX (u16*)multiSmall2GfxTiles

#define STATE_BORED_CTR state2
#define STATE_DIR state3
#define STATE_EVERY_OTHER state4

#define FRAME_HORIZ(start,flip) -8,0,(start),WIDE,SIZE_32,(flip),null
#define FRAME_HEAD(start,flip) 0,0,(start),SQUARE,SIZE_16,(flip),Frame(ENEMY_EXTRA)


const characterFrameDef enemySnakeCharFrames[] = 
{
	[ENEMY_N_A] = {FRAME_HEAD(26,false)},
	[ENEMY_N_B] = {FRAME_HEAD(28,false)},
	[ENEMY_S_A] = {FRAME_SMALL_SQUARE(84,false)},
	[ENEMY_S_B] = {FRAME_SMALL_SQUARE(86,false)},
	[ENEMY_E_A] = {FRAME_HORIZ(88,false)},
	[ENEMY_E_B] = {FRAME_SMALL_SQUARE(92,false)},
	[ENEMY_W_A] = {FRAME_HORIZ(88,true)},
	[ENEMY_W_B] = {FRAME_SMALL_SQUARE(92,true)},
	[ENEMY_EXTRA] = {4,-8,103,SQUARE,SIZE_8,false,null}, 
};

#define ANIM_ENTRY(x,y) 9,(u16*)GFX,Frame(x),Anim(y)

#define SNAKE_N (characterAnimDef*)&enemySnakeCharAnim[ENEMY_N_A]
#define SNAKE_S (characterAnimDef*)&enemySnakeCharAnim[ENEMY_S_A]
#define SNAKE_E (characterAnimDef*)&enemySnakeCharAnim[ENEMY_E_A]
#define SNAKE_W (characterAnimDef*)&enemySnakeCharAnim[ENEMY_W_A]


const characterAnimDef enemySnakeCharAnim[] = 
{
	[ENEMY_N_A] = {ANIM_ENTRY(ENEMY_N_A,ENEMY_N_B)},
	[ENEMY_N_B] = {ANIM_ENTRY(ENEMY_N_B,ENEMY_N_A)},

	[ENEMY_S_A] = {ANIM_ENTRY(ENEMY_S_A,ENEMY_S_B)},
	[ENEMY_S_B] = {ANIM_ENTRY(ENEMY_S_B,ENEMY_S_A)},

	[ENEMY_E_A] = {ANIM_ENTRY(ENEMY_E_A,ENEMY_E_B)},
	[ENEMY_E_B] = {ANIM_ENTRY(ENEMY_E_B,ENEMY_E_A)},

	[ENEMY_W_A] = {ANIM_ENTRY(ENEMY_W_A,ENEMY_W_B)},
	[ENEMY_W_B] = {ANIM_ENTRY(ENEMY_W_B,ENEMY_W_A)},
};




int enemySnakeInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)GFX, (characterAnimDef*) enemySnakeCharAnim);
	enemy->STATE_DIR = dice(0,3);
	return 1;
}

extern CharacterStatus * mcStatus;

void enemySnakeReface(Enemy * enemy)
{
		switch (enemy->STATE_DIR)
		{
			case ENEMY_DIR_E:
				setCharacterAnimation(enemy->status, SNAKE_E);
				break;
			case ENEMY_DIR_W:
				setCharacterAnimation(enemy->status, SNAKE_W);
				break;
			case ENEMY_DIR_S:
				setCharacterAnimation(enemy->status, SNAKE_S);
				break;
			case ENEMY_DIR_N:
				setCharacterAnimation(enemy->status, SNAKE_N);
				break;

	
		}

}

int enemySnakeFacingPlayer(Enemy * enemy)
{
	int dx = mcStatus->x - enemy->status->x;
	int dy = mcStatus->y - enemy->status->y;
	if (abs(dx) < 10)
	{
		int newDir = enemy->STATE_DIR;
		if (dy > 0)
		{
			newDir = ENEMY_DIR_S;
			enemySnakeReface(enemy);
		}
		else
		{
			newDir = ENEMY_DIR_N;
			enemySnakeReface(enemy);
		}
		if (newDir != enemy->STATE_DIR)
		{
			enemy->STATE_DIR = newDir;
			enemySnakeReface(enemy);
		}
		return true;
	}
	if (abs(dy) < 10)
	{
		int newDir = enemy->STATE_DIR;
		if (dx > 0)
		{
			newDir = ENEMY_DIR_E;
			enemySnakeReface(enemy);
		}
		else
		{
			newDir = ENEMY_DIR_W;
			enemySnakeReface(enemy);
		}
		if (newDir != enemy->STATE_DIR)
		{
			enemy->STATE_DIR = newDir;
			enemySnakeReface(enemy);
		}
		return true;
	}
	return false;
}

int enemySnakeUpdate(Enemy * enemy)
{

	int xd = 0;
	int yd = 0;

	switch(enemy->STATE_DIR)
	{
		case ENEMY_DIR_E:
			xd = 1;
			break;
		case ENEMY_DIR_W:
			xd = -1;
			break;
		case ENEMY_DIR_S:
			yd = 1;
			break;
		case ENEMY_DIR_N:
			yd =-1;
			break;
	}


	if (!enemySnakeFacingPlayer(enemy))
	{
		if (enemy->STATE_EVERY_OTHER > 0)
		{
			xd = 0;
			yd = 0;
			enemy->STATE_EVERY_OTHER = 0;
		}
		else
		{
			enemy->STATE_EVERY_OTHER = 1;
		}

	}


	enemy->STATE_BORED_CTR++;
	if (!moveEnemy(enemy, xd, yd) || (enemy->STATE_BORED_CTR > dice(20, 90)))
	{
		enemy->STATE_BORED_CTR = 0;
		enemy->STATE_DIR = dice(0,3);
		enemySnakeReface(enemy);

	}
	return 1;

}




int enemySnakeFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemySnakeInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemySnakeUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_snake_db = 
{
	snake_portrait_Tiles,
	snake_portrait_Pal,
	snake_portrait_Map,
	"Snake",
	"Venomous Serpent",
	"Wanders slowly, ",
	     "strikes at once",
	null,
	9,8,0
	
};


const EnemyType enemy_snake_def = 
{
	4,				//hp
	4,				//collideDamage
	2,				//numSprites
	12,				//bbHeight
	12,				//bbWidth
	2,				//bbOffx
	2,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemySnakeFunc,	//updateFunction
	(short*)&snakePal,
	&enemy_snake_db
};

