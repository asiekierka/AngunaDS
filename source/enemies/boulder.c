#include "../enemies.h"
#include "../characters.h"
#include "../spriteMgr.h"
#include "../level.h"
#include "../text.h"
#include "../mainChar.h"
#include "statueGfx.h"

#define Frame(x) (void *)&(enemyBoulderCharFrames[x])
#define Anim(x) (void *)&(enemyBoulderCharAnim[x])

#define OFFSET 16 

#define objFrameDefSquare(x) {0,-OFFSET,(x),SQUARE,SIZE_32,false,null}
#define objAnimDef(x) {99999,(u16*)statueGfxTiles,Frame(x),null}




const characterFrameDef enemyBoulderCharFrames[] = 
{
	[ENEMY_DIR_N] = objFrameDefSquare(28),
};

const characterAnimDef enemyBoulderCharAnim[] = 
{
	[ENEMY_DIR_N] = objAnimDef(ENEMY_DIR_N),
	
};


int enemyBoulderInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)statueGfxTiles, (characterAnimDef * )&enemyBoulderCharAnim[enemy->special]);
	return 0;
}

int enemyBoulderUpdate(Enemy * enemy)
{
	return 0;
}

int enemyBoulderBulletCollide(Enemy * enemy)
{
	return 0;
}


int enemyBoulderCollide(Enemy * enemy)
{
	return ENEMY_COLLIDE_NO_DMG;
}

int enemyBoulderEnemyCollide(Enemy * enemy)
{
	return ENEMY_COLLIDE_NO_DMG;
}


int enemyBoulderFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyBoulderInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyBoulderUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
		case ENEMY_BULLET_COLLIDE:
			return enemyBoulderBulletCollide(enemy);
			break;
		case ENEMY_MC_COLLIDE:
			return enemyBoulderCollide(enemy);
			break;
		case ENEMY_ENEMY_COLLIDE:
			return enemyBoulderEnemyCollide(enemy);
			break;
		case ENEMY_DYNAMITED:
			return enemy->hp = 0;
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_boulder_db = 
{
	null,
	null,
	null,
	"boulder"
};

const EnemyType enemy_boulder_def = 
{
	1,				//hp
	0,				//collideDamage
	1,				//numSprites
	25,	            //bbHeight
	32,				//bbWidth
	0,				//bbOffx
	-9,			//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyBoulderFunc,	//updateFunction
	(short*)&boulderPal,
	null
};

