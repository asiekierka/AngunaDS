#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "slimeGfx.h"
#include "bigGhostGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "../mainChar.h"
#include "portraits/bigGhost_portrait.h"
#include "../level.h"

#define Frame(x) (void *)&(enemyGhostCharFrames[x])
#define Anim(x) (void *)&(enemyGhostCharAnim[x])

#define SM_SQ_POS -8,-8

#define GHOST_PAL bigGhostGfxPal
#define GHOST_GFX bigGhostGfxTiles

#define GHOST_TYPE_NORMAL 0
#define GHOST_TYPE_ATTACK 1

#define GHOST_ANIM_SPEED 25

#define BFrame(n) (void *)&(enemyBigGhostCharFrames[n])
#define BAnim(x) (void *)&(enemyBigGhostCharAnim[x])
#define BIGFRAME(x,f) {-16,-16,(x),SQUARE,SIZE_32,(f),null}
#define BIGANIM(x,y) {GHOST_ANIM_SPEED,(u16*)GHOST_GFX,BFrame(x),BAnim(y)}

const characterFrameDef enemyBigGhostCharFrames[] =
{
	BIGFRAME(0,false),
	BIGFRAME(4,false),
	BIGFRAME(0,true),
	BIGFRAME(4,true),
	BIGFRAME(8,false),
	BIGFRAME(12,false),
	BIGFRAME(16,false),
	BIGFRAME(20,false),
};

const characterAnimDef enemyBigGhostCharAnim[] = 
{
	BIGANIM(0,1),
	BIGANIM(1,0),

	BIGANIM(2,3),
	BIGANIM(3,2),

	BIGANIM(4,5),
	BIGANIM(5,4),

	BIGANIM(6,7),
	BIGANIM(7,6),

	{//8 - invisible
		999,
		(u16*)GHOST_GFX,
		null,
		BAnim(2)
	}

};



#define GHOST_W (characterAnimDef*)&enemyBigGhostCharAnim[0]
#define GHOST_E (characterAnimDef*)&enemyBigGhostCharAnim[2]
#define GHOST_S (characterAnimDef*)&enemyBigGhostCharAnim[4]
#define GHOST_N (characterAnimDef*)&enemyBigGhostCharAnim[6]
#define GHOST_INVIS (characterAnimDef*)&enemyBigGhostCharAnim[8]

#define GHOST_HIDING 0
#define GHOST_SHOWING 1
#define GHOST_FADE_IN 2
#define GHOST_FADE_OUT 3

#define GHOST_SHOW_TIME 120
#define GHOST_HIDE_TIME 120
#define GHOST_FADE_IN_TIME 60
#define GHOST_FADE_OUT_TIME 60
#define GHOST_ATTACK_FRAME 60


int enemyBigGhostInit(Enemy * enemy)
{
	enemy->state1 = GHOST_HIDING;
	enemy->state2 = 0;
	initEnemyGfx(enemy, (u16*)GHOST_GFX, GHOST_INVIS);
	setSpritePriority(enemy->status->spriteList[0], SPRITE_PRIORITY_HIGH);
	enemy->status->mobility = MOBILITY_GHOST;
	return 1;
}

/*int enemyBigGhostAttack(Enemy * enemy)
{
	BulletDef * bullet = fireEnemyBullet(enemy, GHOST_BULLET_TILE, 0, 0, 0, 0);
	int bulletDir = enemy->state3 * 3;
	//swap S and E to put it in clock order
	if ((bulletDir == 3) || (bulletDir == 6))
	{
		bulletDir = 9 - bulletDir;
	}
	setBulletClockDirection(bullet, bulletDir, 0);
	
	return 1;
}
*/

int updateHidingBigGhost(Enemy * enemy)
{
	setCharacterAnimation(enemy->status, GHOST_INVIS);
	
	if (enemy->state2 > GHOST_HIDE_TIME)
	{
		LevelData * level = getCurrentLevel();
		
		int posX = dice(20,(level->width * 8) - 20);
		int posY = dice(20,(level->height * 8) - 20);
		
		enemy->state3 = dice(0,3);
		
		enemy->status->x = posX;
		enemy->status->y = posY;
		
			
		enemy->state2 = 0;
		enemy->state1 = GHOST_FADE_IN;
	}
	
	return 1;
}


int updateShowingBigGhost(Enemy * enemy)
{
	if (enemy->state2 > GHOST_SHOW_TIME)
	{
		enemy->state2 = 0;
		enemy->state1 = GHOST_FADE_OUT;
	}
	
	/*if (enemy->state4 == GHOST_TYPE_ATTACK)
	{
		if (enemy->state2 == GHOST_ATTACK_FRAME)
		{
			enemyBigGhostAttack(enemy);
		}
	}*/

	int mcx = getMainCharX();
	int mcy = getMainCharY();


	if (enemy->state2 & 1)
	{

		//pick direction
		int dx = enemy->status->x - mcx;
		int dy = enemy->status->y - mcy;

		if (abs(dx) > abs(dy))
		{
			if (enemy->status->x > mcx + 5)
			{
				setCharacterAnimation(enemy->status, GHOST_W);
			}
			else if (enemy->status->x < mcx)
			{
				setCharacterAnimation(enemy->status, GHOST_E);

			}
		}
		else
		{
			if (enemy->status->y > mcy + 5)
			{
				setCharacterAnimation(enemy->status, GHOST_N);
			}
			else
			{
				setCharacterAnimation(enemy->status, GHOST_S);
			}
		}


		//move enemy
		if (enemy->status->x > mcx + 5)
		{
			moveEnemy(enemy, -1, 0);
		}
		else if (enemy->status->x < mcx)
		{
			moveEnemy(enemy, 1, 0); 

		}

		if (enemy->status->y > mcy + 5)
		{
			moveEnemy(enemy, 0, -1);
		}
		else
		{
			moveEnemy(enemy, 0, 1); 

		}
	}
	
	
	return 1;
}

int updateFadeInBigGhost(Enemy * enemy)
{
	setCharacterAnimation(enemy->status, 
		(characterAnimDef*)&enemyBigGhostCharAnim[1]);
	
	enemy->status->faded = 3;  //just enough to make sure it stays,
								//without worrying about boundary condition
	
	if (enemy->state2 > GHOST_FADE_IN_TIME)
	{
		enemy->state2 = 0;
		enemy->state1 = GHOST_SHOWING;
	}
	
	return 1;
}

int updateFadeOutBigGhost(Enemy * enemy)
{
	enemy->status->faded = 3;  //just enough to make sure it stays,
								//without worrying about boundary condition
	
	if (enemy->state2 > GHOST_FADE_OUT_TIME)
	{
		enemy->state2 = 0;
		enemy->state1 = GHOST_HIDING;
	}

	
	return 1;
}

int enemyBigGhostUpdate(Enemy * enemy)
{
	enemy->state2++;
	
	switch(enemy->state1)
	{
		case GHOST_HIDING:
			return updateHidingBigGhost(enemy);
		case GHOST_SHOWING:
			return updateShowingBigGhost(enemy);
		case GHOST_FADE_IN:
			return updateFadeInBigGhost(enemy);
		case GHOST_FADE_OUT:
			return updateFadeOutBigGhost(enemy);
	}

	return 1;
}

int enemyBigGhostBulletCollide(Enemy * enemy)
{
	if (enemy->state1 == GHOST_SHOWING)
	{
		return 1;
	}
	return 0;
}




int enemyBigGhostFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyBigGhostInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyBigGhostUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
		case ENEMY_BULLET_COLLIDE:
			return enemyBigGhostBulletCollide(enemy);
		case ENEMY_MC_COLLIDE:
			return enemyBigGhostBulletCollide(enemy);
			break;

	}
	return 1;
}


const EnemyDatabaseDef enemy_big_ghost_db = 
{
	bigGhost_portrait_Tiles,
	bigGhost_portrait_Pal,
	bigGhost_portrait_Map,
	"Big Ghost",
	"Giant Ghost",
	"Bigger and tougher",
	        "than the",
			"little ones",
	7,7,7

};
	

const EnemyType enemy_big_ghost_def = 
{
	25,				//hp
	4,				//collideDamage
	2,				//numSprites
	28,				//bbHeight
	28,				//bbWidth
	-14,				//bbOffx
	-14,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyBigGhostFunc,	//updateFunction
	(short*)&GHOST_PAL,
	&enemy_big_ghost_db
};

const EnemyType enemy_big_ghost_final_def = 
{
	100,				//hp
	50,				//collideDamage
	2,				//numSprites
	28,				//bbHeight
	28,				//bbWidth
	-14,				//bbOffx
	-14,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyBigGhostFunc,	//updateFunction
	(short*)&GHOST_PAL,
	&enemy_big_ghost_db
};

	
	
