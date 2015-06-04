#include "../enemies.h"
#include "../characters.h"
#include "../spriteMgr.h"
#include "../level.h"
#include "../text.h"
#include "../mainChar.h"
#include "statueGfx.h"

#define Frame(x) (void *)&(enemyFireStatueCharFrames[x])
#define Anim(x) (void *)&(enemyFireStatueCharAnim[x])

#define OFFSET 16 

#define objFrameDefSquare(x) {0,-OFFSET,(x),SQUARE,SIZE_32,false,null}
#define objAnimDef(x) {99999,(u16*)statueGfxTiles,Frame(x),null}

#define STATUE_FIRE_RATE 90

#define BULLET_TILE_L 8
#define BULLET_TILE_R 8
#define BULLET_TILE_U 8
#define BULLET_TILE_D 8

#define BULLET_ANIM_FRAME_CHANGE 10


const characterFrameDef enemyFireStatueCharFrames[] = 
{
	[ENEMY_DIR_E] = objFrameDefSquare(4),
	[ENEMY_DIR_W] = objFrameDefSquare(0),
};

const characterAnimDef enemyFireStatueCharAnim[] = 
{
	[ENEMY_DIR_E] = objAnimDef(ENEMY_DIR_E),
	[ENEMY_DIR_W] = objAnimDef(ENEMY_DIR_W),
	
};


int enemyFireStatueInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)statueGfxTiles, (characterAnimDef * )&enemyFireStatueCharAnim[enemy->special]);
	enemy->state1 = dice(0,STATUE_FIRE_RATE);
	return 0;
}

void enemyFireStatueShootBullet(Enemy * enemy)
{
	int bulletDir = dice(1,5);
	int bulletTile = 0; 
	int offX = 28;
	int offY = -20;
	switch(enemy->special)
	{
		case ENEMY_DIR_E: 
			bulletTile = BULLET_TILE_R;
			break;
		case ENEMY_DIR_S:
			bulletDir += 3;
			bulletTile = BULLET_TILE_D;
			break;
		case ENEMY_DIR_N:
			bulletDir -=3;
			if (bulletDir < 0)
			{
				bulletDir += 12;
			}
			bulletTile = BULLET_TILE_L;
			break;
		case ENEMY_DIR_W:
			bulletDir += 6;
			bulletTile = BULLET_TILE_U;
			offX = 0;
			break;
	}
	

	BulletDef * bullet = fireEnemyBullet(enemy, bulletTile, offX, offY, 0, 0);
	setSpriteSize(bullet->sprite, SIZE_16);
	setSpriteShape(bullet->sprite, SQUARE);
	setBulletClockDirection(bullet, bulletDir, 2);
	if (enemy->special == ENEMY_DIR_W)
	{
		setSpriteFlip(bullet->sprite, true);
	}
	else
	{
		setBulletSpriteOffset(bullet, -8, 0);
	}

	PaletteId bulletPal = loadPalette((u16*)statueFireGfxPal);

	setSpritePalette(bullet->sprite, bulletPal);
	updateOneBulletDef(bullet);

	//BulletDef * bullet = fireEnemyBullet(enemy, bulletTile, 6, 6, xd, yd);
	
}
	

int enemyFireStatueUpdate(Enemy * enemy)
{
	enemy->state1++;
	if (enemy->state1 > STATUE_FIRE_RATE)
	{
		enemy->state1 = 0;
		enemyFireStatueShootBullet(enemy);
		
	}

	return 0;
}

int enemyStatueUpdate(Enemy * enemy)
{

	return 0;
}

int enemyFireStatueBulletCollide(Enemy * enemy)
{
	return 0;
}


int enemyFireStatueCollide(Enemy * enemy)
{
	return ENEMY_COLLIDE_NO_DMG;
}

int enemyFireStatueEnemyCollide(Enemy * enemy)
{
	return ENEMY_COLLIDE_NO_DMG;
}


int enemyFireStatueDead(Enemy * enemy)
{
	resetEnemy(enemy);
	return 1;
}

int enemyFireStatueFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyFireStatueInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyFireStatueUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return enemyFireStatueDead(enemy);
			resetEnemy(enemy);
			return 1;
			break;
		case ENEMY_BULLET_COLLIDE:
			return enemyFireStatueBulletCollide(enemy);
			break;
		case ENEMY_MC_COLLIDE:
			return enemyFireStatueCollide(enemy);
			break;
		case ENEMY_ENEMY_COLLIDE:
			return enemyFireStatueEnemyCollide(enemy);
			break;
		case ENEMY_DYNAMITED:
			return enemy->hp = 0;
			break;
	}
	return 1;
}

int enemyStatueFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyFireStatueInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyStatueUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return enemyFireStatueDead(enemy);
			resetEnemy(enemy);
			return 1;
			break;
		case ENEMY_BULLET_COLLIDE:
			return enemyFireStatueBulletCollide(enemy);
			break;
		case ENEMY_MC_COLLIDE:
			return enemyFireStatueCollide(enemy);
			break;
		case ENEMY_ENEMY_COLLIDE:
			return enemyFireStatueEnemyCollide(enemy);
			break;
		case ENEMY_DYNAMITED:
			return enemy->hp = 0;
			break;
	}
	return 1;
}

void enemyFireStatueUpdateBullet(BulletDef * bullet)
{
	if (bullet->ttl == BULLET_INFINITE_TTL - BULLET_ANIM_FRAME_CHANGE)
	{

	}

}

const EnemyDatabaseDef enemy_fire_statue_db = 
{
	null,
	null,
	null,
	"fire statue"
};

const EnemyType enemy_fire_statue_def = 
{
	1,				//hp
	0,				//collideDamage
	1,				//numSprites
	16,	            //bbHeight
	32,				//bbWidth
	0,				//bbOffx
	0,			//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyFireStatueFunc,	//updateFunction
	(short*)&statueGfxPal,
	&enemy_fire_statue_db
};

const EnemyType enemy_statue_def = 
{
	1,				//hp
	0,				//collideDamage
	1,				//numSprites
	16,	            //bbHeight
	32,				//bbWidth
	0,				//bbOffx
	0,			//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyStatueFunc,	//updateFunction
	(short*)&statueGfxPal,
	&enemy_fire_statue_db
};

