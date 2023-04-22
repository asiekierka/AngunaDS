#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "bossBatGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "enemies/portraits/batBoss_portrait.h"

#define Frame(x) (void *)&(enemyGiantBatCharFrames[x])
#define Anim(x) (void *)&(enemyGiantBatCharAnim[x])

#define XDIR state1
#define YDIR state2
#define BORED_CTR state3
#define SINCE_SHOOT state4

#define BORED_APPROX 30
#define GIANT_BAT_BULLET_TILE 105
#define SHOOT_RATE 60


const characterFrameDef enemyGiantBatCharFrames[] = 
{
	{//0
		FRAME_32_SQUARE(10,false)
	},
	{//1
		FRAME_32_SQUARE(14,false)
	},
	{//2
		FRAME_32_SQUARE(18,false)
	},
};

#define ANIM_ENTRY(x,y) 10,(u16*)bossBatGfxTiles,Frame(x),Anim(y)

const characterAnimDef enemyGiantBatCharAnim[] = 
{
	{//0 - frame 1 
		ANIM_ENTRY(0,1)
	},
	{//1 - frame 2
		ANIM_ENTRY(1,2)
	},
	{//2 - frame 3
		ANIM_ENTRY(2,3)
	},
	{//3 - frame 4
		ANIM_ENTRY(1,0)
	},
};



void enemyGiantBatChooseDirection(Enemy * enemy)
{
	enemy->XDIR = dice(0,2) - 1;
	enemy->YDIR = dice(0,2) - 1;
	
}


int enemyGiantBatInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)bossBatGfxTiles, (characterAnimDef*) enemyGiantBatCharAnim);
	enemy->status->mobility = MOBILITY_FLYING;
	enemyGiantBatChooseDirection(enemy);
	enemy->SINCE_SHOOT = 0;
	return 1;
}

extern CharacterStatus * mcStatus;

void enemyGiantBatShoot(Enemy * enemy)
{
	if (enemy->SINCE_SHOOT > 0)
	{
		return;
	}

	int bulletTile = GIANT_BAT_BULLET_TILE; 
	int bulletDir;
	enemy->SINCE_SHOOT = SHOOT_RATE;
	for (bulletDir = 0; bulletDir < 12; bulletDir++)
	{
		BulletDef * bullet = fireEnemyBullet(enemy, bulletTile, 8, -8, 0, 0);
		setBulletClockDirection(bullet, bulletDir, 2);
		if (bullet->xSpeed < 0)
		{
			setSpriteFlip(bullet->sprite, true);
		}
		if (bullet->ySpeed > 0)
		{
			setSpriteVFlip(bullet->sprite, true);
		}
		bullet->ttl = SHOOT_RATE;
		setSpritePalette(bullet->sprite, findPalette((u16*)largeBatGfxPal));
	}
}


void enemyGiantBatSetDir(Enemy * enemy, int x, int y)
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

int enemyGiantBatUpdate(Enemy * enemy)
{
	enemy->BORED_CTR++;
	if (enemy->SINCE_SHOOT > 0)
	{
		enemy->SINCE_SHOOT--;
	}

	//if (enemy->BORED_CTR > BORED_APPROX + dice(0,120))
	if (enemy->BORED_CTR > BORED_APPROX + dice(0,120))
	{
		if ((enemy->XDIR == 0) && (enemy->YDIR == 0))
		{
			enemyGiantBatShoot(enemy);
		}
		enemyGiantBatChooseDirection(enemy);
		enemy->BORED_CTR = 0;
	}
	
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

int enemyGiantBatFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyGiantBatInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyGiantBatUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_giantbat_db = 
{
	batBoss_portraitTiles,
	batBoss_portraitPal,
	batBoss_portraitMap,
	"Giant Bat",
	"A HUGE vampire bat",
	"",
	     "",
	null,
	13,12,9
};


const EnemyType enemy_giantbat_def = 
{
	26,				//hp
	10,				//collideDamage
	1,				//numSprites
	12,				//bbHeight
	12,				//bbWidth
	3,				//bbOffx
	3,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyGiantBatFunc,	//updateFunction
	(short*)&largeBatGfxPal,
	&enemy_giantbat_db
};

const EnemyType enemy_giantbatfinal_def = 
{
	80,				//hp
	18,				//collideDamage
	1,				//numSprites
	20,				//bbHeight
	24,				//bbWidth
	4,				//bbOffx
	4,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyGiantBatFunc,	//updateFunction
	(short*)&largeBatGfxPal,
	&enemy_giantbat_db
};
	
