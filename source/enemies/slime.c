#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "slimeGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "portraits/slime_portrait.h"

#define RED_SLIME_DEFAULT_SPAWNS 3
#define SLIME_GET_BORED_TICKS 10

#define Frame(x) (void *)&(enemySlimeCharFrames[x])
#define Anim(x) (void *)&(enemySlimeCharAnim[x])

#define SM_SQ_POS -8,-8

const characterFrameDef enemySlimeCharFrames[] = 
{
	////FRAME: north 1
	{//0
		SM_SQ_POS,		//x,y offset
		68,			//tile
		SQUARE,		//shape (SQUARE/TALL/WIDE)
		SIZE_16,	//size
		false, 		//flip
		null //nextSprite
	},
	{//1
		SM_SQ_POS,		//x,y offset
		70,				//tile
		SQUARE,			//shape (SQUARE/TALL/WIDE)
		SIZE_16,		//size
		false, 
	null 			//nextSprite
	},
	////FRAME: south 1
	{//2
		SM_SQ_POS,		//x,y offset
		64,			//tile
		SQUARE,		//shape (SQUARE/TALL/WIDE)
		SIZE_16,	//size
		false, 		//flip
		null //nextSprite
	},
	{//3
		SM_SQ_POS,		//x,y offset
		66,				//tile
		SQUARE,			//shape (SQUARE/TALL/WIDE)
		SIZE_16,		//size
		false, 
		null 			//nextSprite
	},
	////FRAME: east 1
	{//4
		SM_SQ_POS,		//x,y offset
		0,			//tile
		SQUARE,		//shape (SQUARE/TALL/WIDE)
		SIZE_16,	//size
		true, 		//flip
		null //nextSprite
	},
	{//5
		SM_SQ_POS,		//x,y offset
		2,				//tile
		SQUARE,			//shape (SQUARE/TALL/WIDE)
		SIZE_16,		//size
		true, 
		null 			//nextSprite
	},
	////FRAME: west 1
	{//4
		SM_SQ_POS,		//x,y offset
		0,			//tile
		SQUARE,		//shape (SQUARE/TALL/WIDE)
		SIZE_16,	//size
		false, 		//flip
		null //nextSprite
	},
	{//5
		SM_SQ_POS,		//x,y offset
		2,				//tile
		SQUARE,			//shape (SQUARE/TALL/WIDE)
		SIZE_16,		//size
		false, 
		null 			//nextSprite
	},


};

#define ANIM_RATE 20

const characterAnimDef enemySlimeCharAnim[] = 
{
	{//0 - north frame 1
		ANIM_RATE,
		(u16*)slimeGfxData,
		Frame(0),
		Anim(1)
	},
	{//1 - north frame 2
		ANIM_RATE,
		(u16*)slimeGfxData,
		Frame(1),
		Anim(0)
	},
	{//2 - south frame 1
		ANIM_RATE,
		(u16*)slimeGfxData,
		Frame(2),
		Anim(3)
	},
	{//3 - south frame 2
		ANIM_RATE,
		(u16*)slimeGfxData,
		Frame(3),
		Anim(2)
	},
	{//4 - east frame 1
		ANIM_RATE,
		(u16*)slimeGfxData,
		Frame(4),
		Anim(5)
	},
	{//5 - east frame 2
		ANIM_RATE,
		(u16*)slimeGfxData,
		Frame(5),
		Anim(4)
	},
	{//6 - west frame 1
		ANIM_RATE,
		(u16*)slimeGfxData,
		Frame(6),
		Anim(7)
	},
	{//7 - east frame 2
		ANIM_RATE,
		(u16*)slimeGfxData,
		Frame(7),
		Anim(6)
	},
};


#define SLIME_N (characterAnimDef*)&enemySlimeCharAnim[0]
#define SLIME_S (characterAnimDef*)&enemySlimeCharAnim[2]
#define SLIME_E (characterAnimDef*)&enemySlimeCharAnim[4]
#define SLIME_W (characterAnimDef*)&enemySlimeCharAnim[6]

#define SLIME_CHASE 0
#define SLIME_WANDER 1



int enemySlimeInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)slimeGfxData, (characterAnimDef*) enemySlimeCharAnim);
	enemy->state4 = dice(SLIME_CHASE,SLIME_WANDER);
	return 1;
}

extern CharacterStatus * mcStatus;

int enemySlimeUpdate(Enemy * enemy)
{

	if (enemy->state3 <= 2)
	{
		enemy->state3 ++;
		return 0;
	}
	else
	{
		enemy->state3 = 0;
	}


	//either chase, or wander, depending on slime style...
	//
	if (enemy->state4 == SLIME_CHASE)
	{

		int xa = (mcStatus->x - enemy->status->x) + 4;
		int ya = (mcStatus->y - enemy->status->y);
		if(abs(xa) > abs(ya))
		{
			if (xa > 0)
			{
				setCharacterAnimation(enemy->status, SLIME_E);
			}
			else
			{
				setCharacterAnimation(enemy->status, SLIME_W);
			}
		}
		else
		{
			if (ya > 0)
			{
				setCharacterAnimation(enemy->status, SLIME_S);
			}
			else
			{
				setCharacterAnimation(enemy->status, SLIME_N);
			}
	
		}
		int oldX = enemy->status->x;
		int oldY = enemy->status->y;

		moveEnemy(enemy, unitVal(xa), unitVal(ya));

		if ((enemy->status->x == oldX) && (enemy->status->y == oldY))
		{
			enemy->state1++;
			if (enemy->state1 > SLIME_GET_BORED_TICKS)
			{
				enemy->state4 = SLIME_WANDER;
			}
		}
		else
		{
			enemy->state1 = 1;	
		}



		return 0;
	}

	int xd = 0;
	int yd = 0;

	switch(enemy->state1)
	{
		case 1: 
			xd = 1;
			setCharacterAnimation(enemy->status, SLIME_E);
			break;
		case 2:
			yd = 1;
			setCharacterAnimation(enemy->status, SLIME_S);
			break;
		case 3:
			xd = -1;
			setCharacterAnimation(enemy->status, SLIME_W);
			break;
		case 4:
			yd = -1;
			setCharacterAnimation(enemy->status, SLIME_N);
			break;
	}

	enemy->state2++;
	if (!moveEnemy(enemy, xd, yd) || (enemy->state2 > dice(20, 90)))
	{
		enemy->state2 = 0;
		enemy->state1 = dice(1,4);
		if (dice(1,6) == 1)
		{
			enemy->state4 = SLIME_CHASE;
	
		}
	}


	return 1;
}

int enemyRedSlimeDead(Enemy * enemy)
{
	if (enemy->special < 0)
	{
		return killEnemy(enemy);
	}
	
	
	int spawnsRemaining = enemy->special - 1;
	if (spawnsRemaining == -1)
	{
		spawnsRemaining = RED_SLIME_DEFAULT_SPAWNS;
	}
	if (spawnsRemaining == 0)
	{
		spawnsRemaining = -1;
	}	
	
	Enemy * newSlime = createNewEnemy(enemy->enemyType, 
								      enemy->status->x, 
								      enemy->status->y, 
								      enemy->itemDrop, 
								      spawnsRemaining);
								   
	newSlime->status->faded = ENEMY_FADE_TIME;
	newSlime->status->lastBulletHitBy = enemy->status->lastBulletHitBy;
	newSlime->state4 = SLIME_WANDER;
	
	
	enemy->hp = enemy->enemyType->startHP;
	enemy->special = spawnsRemaining;
	enemy->state4 = SLIME_WANDER;
	
	return 1; 
}

int enemySlimeFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemySlimeInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemySlimeUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_slime_db = 
{
	slime_portrait_Tiles,
	slime_portrait_Pal,
	slime_portrait_Map,
	"Blue Slime",
	"Slimy gooey blob",
	"Slowly wanders around",
	        "the room.",
	null,
	0,9,0

};
	

const EnemyType enemy_slime_def = 
{
	2,				//hp
	2,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	-6,				//bbOffx
	-6,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemySlimeFunc,	//updateFunction
	(short*)&blueSlimePal,
	&enemy_slime_db
};



int enemyRedSlimeFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemySlimeInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemySlimeUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return enemyRedSlimeDead(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_redSlime_db = 
{
	slime_portrait_Tiles,
	slime_portrait_Pal + 16,
	slime_portrait_Map,
	"Red Slime",
	"Slimy gooey blob",
	"Multiplies when killed",
	        "",
	null,
	0,9,0

};
	

const EnemyType enemy_redSlime_def = 
{
	6,				//hp
	5,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	-6,				//bbOffx
	-6,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyRedSlimeFunc,	//updateFunction
	(short*)&redSlimePal,
	&enemy_redSlime_db
};

	
	
