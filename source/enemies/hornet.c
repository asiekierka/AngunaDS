#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "../misc.h"
#include "../characters.h"
#include "multiSmall2Gfx.h"
#include "enemies/portraits/hornet_portrait.h"

#define Frame(x) (void *)&(enemyHornetCharFrames[x])
#define Anim(x) (void *)&(enemyHornetCharAnim[x])

#define GFX (u16*)multiSmall2GfxTiles

#define HORNET_SMALL_SQUARE(start,flip) FRAME_SM_SQ_POS,(start),SQUARE,SIZE_16,(flip),Frame(ENEMY_EXTRA)

const characterFrameDef enemyHornetCharFrames[] = 
{
	[ENEMY_N_A] = {HORNET_SMALL_SQUARE(76,false)},
	[ENEMY_N_B] = {HORNET_SMALL_SQUARE(78,false)},
	[ENEMY_S_A] = {HORNET_SMALL_SQUARE(80,false)},
	[ENEMY_S_B] = {HORNET_SMALL_SQUARE(82,false)},
	[ENEMY_E_A] = {HORNET_SMALL_SQUARE(16,false)},
	[ENEMY_E_B] = {HORNET_SMALL_SQUARE(18,false)},
	[ENEMY_W_A] = {HORNET_SMALL_SQUARE(16,true)},
	[ENEMY_W_B] = {HORNET_SMALL_SQUARE(18,true)},
	[ENEMY_EXTRA] = {4,16,102,SQUARE,SIZE_8,false,null},
};

#define ANIM_ENTRY(x,y) 9,(u16*)GFX,Frame(x),Anim(y)

#define HORNET_N (characterAnimDef*)&enemyHornetCharAnim[ENEMY_N_A]
#define HORNET_S (characterAnimDef*)&enemyHornetCharAnim[ENEMY_S_A]
#define HORNET_E (characterAnimDef*)&enemyHornetCharAnim[ENEMY_E_A]
#define HORNET_W (characterAnimDef*)&enemyHornetCharAnim[ENEMY_W_A]


const characterAnimDef enemyHornetCharAnim[] = 
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




int enemyHornetInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)GFX, (characterAnimDef*) enemyHornetCharAnim);
	enemy->state3 = dice(0,3);
	return 1;
}

extern CharacterStatus * mcStatus;

int enemyHornetUpdate(Enemy * enemy)
{

	int xd = 0;
	int yd = 0;

	switch(enemy->state3)
	{
		case 0:
			xd = 1;
			break;
		case 1:
			xd = -1;
			break;
		case 2:
			yd = 1;
			break;
		case 3:
			yd =-1;
			break;
	}

	enemy->state2++;
	if (!moveEnemy(enemy, xd, yd) || (enemy->state2 > dice(20, 90)))
	{
		enemy->state2 = 0;
		enemy->state3 = dice(0,3);
		switch (enemy->state3)
		{
			case 0:
				setCharacterAnimation(enemy->status, HORNET_E);
				break;
			case 1:
				setCharacterAnimation(enemy->status, HORNET_W);
				break;
			case 2:
				setCharacterAnimation(enemy->status, HORNET_S);
				break;
			case 3:
				setCharacterAnimation(enemy->status, HORNET_N);
				break;

	
		}
	}

	return 1;
}

int enemyHornetFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyHornetInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyHornetUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_hornet_db = 
{
	hornet_portraitTiles,
	hornet_portraitPal,
	hornet_portraitMap,
	"Wasp",
	"Overgrown stinging bug",
	"Flies around",
	     "looking for prey",
	null,
	5,5,0
	
};


const EnemyType enemy_hornet_def = 
{
	4,				//hp
	4,				//collideDamage
	2,				//numSprites
	12,				//bbHeight
	12,				//bbWidth
	3,				//bbOffx
	5,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyHornetFunc,	//updateFunction
	(short*)&hornetPal,
	&enemy_hornet_db
};

