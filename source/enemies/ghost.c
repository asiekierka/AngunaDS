#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "multiSmall2Gfx.h"
#include "../misc.h"
#include "../characters.h"
#include "../mainChar.h"
#include "portraits/ghost_portrait.h"
#include "../level.h"

#define Frame(x) (void *)&(enemyGhostCharFrames[x])
#define Anim(x) (void *)&(enemyGhostCharAnim[x])

#define SM_SQ_POS -8,-8

#define GHOST_PAL ghostPal
#define GHOST_GFX multiSmall2GfxTiles

#define GHOST_TYPE_NORMAL 0
#define GHOST_TYPE_ATTACK 1

const characterFrameDef enemyGhostCharFrames[] = 
{
	{//n1
		FRAME_SMALL_SQUARE(4,false)
	},
	{//n2
		FRAME_SMALL_SQUARE(6,false)
	},
	{//e1
		FRAME_SMALL_SQUARE(0,true)
	},
	{//e2
		FRAME_SMALL_SQUARE(2,true)
	},
	{//s1
		FRAME_SMALL_SQUARE(64,true)
	},
	{//s2
		FRAME_SMALL_SQUARE(66,true)
	},
	{//w1
		FRAME_SMALL_SQUARE(0,false)
	},
	{//w2
		FRAME_SMALL_SQUARE(2,false)
	},
	{//invis
		FRAME_SMALL_SQUARE(94,false)
	},
};

#define ANIM_RATE 15
#define ANIM_ENTRY(x,y) ANIM_RATE,(u16*)GHOST_GFX,Frame(x),Anim(y)


const characterAnimDef enemyGhostCharAnim[] = 
{
	{//n1
		ANIM_ENTRY(0,1)
	},
	{//n2
		ANIM_ENTRY(1,0)
	},
	{//e1
		ANIM_ENTRY(2,3)
	},
	{//e2
		ANIM_ENTRY(3,2)
	},
	{//s1
		ANIM_ENTRY(4,5)
	},
	{//s2
		ANIM_ENTRY(5,4)
	},
	{//w1
		ANIM_ENTRY(6,7)
	},
	{//w2
		ANIM_ENTRY(7,6)
	},
	{//w2
		ANIM_ENTRY(8,8)
	},
};


#define GHOST_N (characterAnimDef*)&enemyGhostCharAnim[0]
#define GHOST_S (characterAnimDef*)&enemyGhostCharAnim[4]
#define GHOST_E (characterAnimDef*)&enemyGhostCharAnim[2]
#define GHOST_W (characterAnimDef*)&enemyGhostCharAnim[6]
#define GHOST_INVIS (characterAnimDef*)&enemyGhostCharAnim[8]

#define GHOST_HIDING 0
#define GHOST_SHOWING 1
#define GHOST_FADE_IN 2
#define GHOST_FADE_OUT 3

#define GHOST_SHOW_TIME 120
#define GHOST_HIDE_TIME 120
#define GHOST_FADE_IN_TIME 60
#define GHOST_FADE_OUT_TIME 60
#define GHOST_ATTACK_FRAME 60

#define GHOST_FACE_N 0
#define GHOST_FACE_S 2
#define GHOST_FACE_E 1
#define GHOST_FACE_W 3

#define GHOST_BULLET_TILE 0
#define BULLET_L 68
#define BULLET_R 100
#define BULLET_U 69
#define BULLET_D 101


int enemyGhostInit(Enemy * enemy)
{
	enemy->state1 = GHOST_HIDING;
	enemy->state2 = 0;
	initEnemyGfx(enemy, (u16*)GHOST_GFX, GHOST_INVIS);
	setSpritePriority(enemy->status->spriteList[0], SPRITE_PRIORITY_HIGH);
	enemy->status->mobility = MOBILITY_GHOST;
	return 1;
}

int enemyGhostAttack(Enemy * enemy)
{
	int bulletTile = BULLET_U;
	int bulletDir = 0;
	switch (enemy->state3)
	{
		case GHOST_FACE_S:
			bulletTile = BULLET_D;
			bulletDir = 6;
			break;
		case GHOST_FACE_E:
			bulletTile = BULLET_R;
			bulletDir = 3;
			break;
		case GHOST_FACE_W:
			bulletTile = BULLET_L;
			bulletDir = 9;
			break;
	}
	BulletDef * bullet = fireEnemyBullet(enemy, bulletTile, 6, 6, 0, 0);
	setBulletClockDirection(bullet, bulletDir, 0);
	
	return 1;
}

int updateHidingGhost(Enemy * enemy)
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


int updateShowingGhost(Enemy * enemy)
{
	if (enemy->state2 > GHOST_SHOW_TIME)
	{
		enemy->state2 = 0;
		enemy->state1 = GHOST_FADE_OUT;
	}
	
	if (enemy->state4 == GHOST_TYPE_ATTACK)
	{
		if (enemy->state2 == GHOST_ATTACK_FRAME)
		{
			enemyGhostAttack(enemy);
		}
	}
	else
	{
		int mcx = getMainCharX();
		int mcy = getMainCharY();

		if ((enemy->state2 & 3) == 1)
		{
			if (enemy->status->x > mcx + 5)
			{
				moveEnemy(enemy, -1, 0);
				setCharacterAnimation(enemy->status, GHOST_W);
			}
			else if (enemy->status->x < mcx)
			{
				moveEnemy(enemy, 1, 0); 
				setCharacterAnimation(enemy->status, GHOST_E);

			}
			if (enemy->status->y > mcy + 5)
			{
				setCharacterAnimation(enemy->status, GHOST_N);
				moveEnemy(enemy, 0, -1);
			}
			else if (enemy->status->y < mcy)
			{
				setCharacterAnimation(enemy->status, GHOST_S);
				moveEnemy(enemy, 0, 1); 

			}
		}



	}
	
	return 1;
}

int updateFadeInGhost(Enemy * enemy)
{
	setCharacterAnimation(enemy->status, 
		(characterAnimDef*)&enemyGhostCharAnim[enemy->state3 * 2]);
	
	enemy->status->faded = 3;  //just enough to make sure it stays,
								//without worrying about boundary condition
	
	if (enemy->state2 > GHOST_FADE_IN_TIME)
	{
		enemy->state2 = 0;
		enemy->state1 = GHOST_SHOWING;
	}
	
	return 1;
}

int updateFadeOutGhost(Enemy * enemy)
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

int enemyGhostUpdate(Enemy * enemy)
{
	enemy->state2++;
	
	switch(enemy->state1)
	{
		case GHOST_HIDING:
			return updateHidingGhost(enemy);
		case GHOST_SHOWING:
			return updateShowingGhost(enemy);
		case GHOST_FADE_IN:
			return updateFadeInGhost(enemy);
		case GHOST_FADE_OUT:
			return updateFadeOutGhost(enemy);
	}

	return 1;
}

int enemyGhostBulletCollide(Enemy * enemy)
{
	if (enemy->state1 == GHOST_SHOWING)
	{
		return 1;
	}
	return 0;
}


int enemyGhostFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyGhostInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyGhostUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
		case ENEMY_BULLET_COLLIDE:
			return enemyGhostBulletCollide(enemy);
		case ENEMY_MC_COLLIDE:
			return enemyGhostBulletCollide(enemy);
			break;

	}
	return 1;
}


const EnemyDatabaseDef enemy_ghost_db = 
{
	ghost_portrait_Tiles,
	ghost_portrait_Pal,
	ghost_portrait_Map,
	"Ghost",
	"Neither dead or alive",
	"Appears and disappears",
	        "",
	null,
	0,9,0

};
	

const EnemyType enemy_ghost_def = 
{
	4,				//hp
	3,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	2,				//bbOffx
	2,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyGhostFunc,	//updateFunction
	(short*)&GHOST_PAL,
	&enemy_ghost_db
};

int enemyAttackGhostFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			enemy->state4 = GHOST_TYPE_ATTACK;
			return enemyGhostInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyGhostUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
		case ENEMY_BULLET_COLLIDE:
			return enemyGhostBulletCollide(enemy);
		case ENEMY_MC_COLLIDE:
			return enemyGhostBulletCollide(enemy);
			break;

	}
	return 1;
}


const EnemyDatabaseDef enemy_attack_ghost_db = 
{
	ghost_portrait_Tiles,
	ghost_portrait_Pal,
	ghost_portrait_Map,
	"Dark Ghost",
	"Neither dead or alive",
	"Appears and shoots",
	        "",
	null,
	0,9,0

};
	

const EnemyType enemy_attack_ghost_def = 
{
	6,				//hp
	4,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	2,				//bbOffx
	2,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyAttackGhostFunc,	//updateFunction
	(short*)&GHOST_PAL,
	&enemy_attack_ghost_db
};

	
	
