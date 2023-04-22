#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "sentryGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "../mainChar.h"
#include "../level.h"
#include "itemGfx.h"
#include "enemies/portraits/sentry_portrait.h"

#define Frame(x) (void *)&(enemySentryCharFrames[x])
#define Anim(x) (void *)&(enemySentryCharAnim[x])

#define TILE_L(x) {-12,-20,(x),SQUARE,SIZE_32, false, null}
#define TILE_L_F(x) {-20,-20,(x),SQUARE,SIZE_32, true, null}

#define TILE_R(x) {-20,-20,(x),SQUARE,SIZE_32, false, null}
#define TILE_R_F(x) {-12,-20,(x),SQUARE,SIZE_32, true, null}


enum
{
	SENTRY_N_A,
	SENTRY_N_B,
	SENTRY_N_C,

	SENTRY_S_A,
	SENTRY_S_B,
	SENTRY_S_C,

	SENTRY_L_A,
	SENTRY_L_B,
	SENTRY_L_C,

	SENTRY_R_A,
	SENTRY_R_B,
	SENTRY_R_C,

	SENTRY_RUN_N_A,
	SENTRY_RUN_N_B,
	SENTRY_RUN_N_C,
	SENTRY_RUN_N_D,

	SENTRY_RUN_S_A,
	SENTRY_RUN_S_B,
	SENTRY_RUN_S_C,
	SENTRY_RUN_S_D,

	SENTRY_RUN_L_A,
	SENTRY_RUN_L_B,
	SENTRY_RUN_L_C,
	SENTRY_RUN_L_D,

	SENTRY_RUN_R_A,
	SENTRY_RUN_R_B,
	SENTRY_RUN_R_C,
	SENTRY_RUN_R_D,

	
};

const characterFrameDef enemySentryCharFrames[] = 
{
	[SENTRY_N_A] = TILE_L(0),
	[SENTRY_N_B] = TILE_R(3),
	[SENTRY_N_C] = TILE_L(7),

	[SENTRY_R_A] = TILE_R(10),
	[SENTRY_R_B] = TILE_L(14),
	[SENTRY_R_C] = TILE_R(17),

	[SENTRY_S_A] = TILE_L(21),
	[SENTRY_S_B] = TILE_R(24),
	[SENTRY_S_C] = TILE_R(28),

	[SENTRY_L_A] = TILE_R_F(10),
	[SENTRY_L_B] = TILE_L_F(14),
	[SENTRY_L_C] = TILE_R_F(17),

};

#define walkN Anim(SENTRY_N_A)
#define walkL Anim(SENTRY_L_A)
#define walkR Anim(SENTRY_R_A)
#define walkS Anim(SENTRY_S_A)
#define runN  Anim(SENTRY_RUN_N_A)
#define runL Anim(SENTRY_RUN_L_A)
#define runR Anim(SENTRY_RUN_R_A)
#define runS Anim(SENTRY_RUN_S_A)

#define WATCH_SPEED 180
#define RUN_SPEED 10

#define SPEAR_TICK_MIN 60
#define SPEAR_TICK_RANGE 180

#define PAUSE_FRAMES 20

#define DIRECTION state1
#define FRAME_CTR state2
#define STATE state3

#define GFX_TILES sentryGfxTiles

#define WALK(x,y) {0,(u16*)GFX_TILES,Frame(x),Anim(y)}
#define RUN(x,y) {RUN_SPEED,(u16*)GFX_TILES,Frame(x),Anim(y)}

#define SENTRY_DIR_MASK 0x3

#define SENTRY_STATE_LOOKING 1
#define SENTRY_STATE_CHANGEUP 0
#define SENTRY_STATE_SPOTTED 3
#define SENTRY_STATE_PAUSING 4


const characterAnimDef enemySentryCharAnim[] = 
{
	[SENTRY_N_A] =  WALK(SENTRY_N_A,SENTRY_N_A),
	[SENTRY_S_A] =  WALK(SENTRY_S_A,SENTRY_S_A),
	[SENTRY_L_A] =  WALK(SENTRY_L_A,SENTRY_L_A),
	[SENTRY_R_A] =  WALK(SENTRY_R_A,SENTRY_R_A),

	[SENTRY_RUN_N_A] =  RUN(SENTRY_N_A,SENTRY_RUN_N_B),
	[SENTRY_RUN_N_B] =  RUN(SENTRY_N_B,SENTRY_RUN_N_C),
	[SENTRY_RUN_N_C] =  RUN(SENTRY_N_C,SENTRY_RUN_N_D),
	[SENTRY_RUN_N_D] =  RUN(SENTRY_N_B,SENTRY_RUN_N_A),

	[SENTRY_RUN_S_A] =  RUN(SENTRY_S_A,SENTRY_RUN_S_B),
	[SENTRY_RUN_S_B] =  RUN(SENTRY_S_B,SENTRY_RUN_S_C),
	[SENTRY_RUN_S_C] =  RUN(SENTRY_S_C,SENTRY_RUN_S_D),
	[SENTRY_RUN_S_D] =  RUN(SENTRY_S_B,SENTRY_RUN_S_A),
	
	[SENTRY_RUN_L_A] =  RUN(SENTRY_L_A,SENTRY_RUN_L_B),
	[SENTRY_RUN_L_B] =  RUN(SENTRY_L_B,SENTRY_RUN_L_C),
	[SENTRY_RUN_L_C] =  RUN(SENTRY_L_C,SENTRY_RUN_L_D),
	[SENTRY_RUN_L_D] =  RUN(SENTRY_L_B,SENTRY_RUN_L_A),

	[SENTRY_RUN_R_A] =  RUN(SENTRY_R_A,SENTRY_RUN_R_B),
	[SENTRY_RUN_R_B] =  RUN(SENTRY_R_B,SENTRY_RUN_R_C),
	[SENTRY_RUN_R_C] =  RUN(SENTRY_R_C,SENTRY_RUN_R_D),
	[SENTRY_RUN_R_D] =  RUN(SENTRY_R_B,SENTRY_RUN_R_A),


};



void enemySentryUpdateWalkAnim(Enemy * enemy);

int enemySentryInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)GFX_TILES, (characterAnimDef*) enemySentryCharAnim);
	enemy->DIRECTION = dice(0,3);
	enemySentryUpdateWalkAnim(enemy);
	enemy->STATE = SENTRY_STATE_LOOKING;
	return 1;
}

int enemySentrySetDirection(Enemy * enemy, int axis, int xa, int ya)
{
		if (axis == 0)
		{
			if (xa < 0)
			{
				enemy->DIRECTION = ENEMY_DIR_W;
			}
			else
			{
				enemy->DIRECTION = ENEMY_DIR_E;
			}
		}
		else
		{
			if (ya < 0)
			{
				enemy->DIRECTION = ENEMY_DIR_N;
			}
			else
			{
				enemy->DIRECTION = ENEMY_DIR_S;
			}
		}

		return enemy->DIRECTION;
}

void enemySentryThrowSpear(Enemy * enemy)
{
	int xOff = -3;
	int yOff = -3;
	SignedPair dir = getMovementFromEnemyDirection(enemy->DIRECTION);
	BulletDef * bullet = fireEnemyBullet(enemy, 64, xOff, yOff, signedPairX(dir) * 2, signedPairY(dir) * 2); 
	if (bullet != null)
	{
		bullet->tile = 192;
		Sprite * sprite = bullet->sprite;
		if (bullet->xSpeed > 0)
		{
			setSpriteFlip(sprite, true);
		}
		if (bullet->ySpeed > 0)
		{
			setSpriteVFlip(sprite, true);
		}

		if (abs(bullet->xSpeed) > 0)
		{
			setSpriteShape(sprite, WIDE);
		}
		else
		{
			setSpriteShape(sprite, TALL);
			bullet->tile += 2;
		}

		setSpritePalette(sprite, findPalette((u16*)itemsPal));
		setSpriteStartTile(sprite, bullet->tile);
	}

}

void enemySentryUpdateWalkAnim(Enemy * enemy)
{
	//if tired of walking forward, change modes
	switch(enemy->DIRECTION)
	{
		case ENEMY_DIR_E: 
			setCharacterAnimation(enemy->status, walkR);
			break;
		case ENEMY_DIR_S:
			setCharacterAnimation(enemy->status, walkS);
			break;
		case ENEMY_DIR_W:
			setCharacterAnimation(enemy->status, walkL);
			break;
		case ENEMY_DIR_N:
			setCharacterAnimation(enemy->status, walkN);
			break;
	}
}


void enemySentrySetRunAnim(Enemy * enemy)
{
	switch(enemy->DIRECTION)
	{
		case ENEMY_DIR_E: 
			setCharacterAnimation(enemy->status, runR);
			break;
		case ENEMY_DIR_S:
			setCharacterAnimation(enemy->status, runS);
			break;
		case ENEMY_DIR_W:
			setCharacterAnimation(enemy->status, runL);
			break;
		case ENEMY_DIR_N:
			setCharacterAnimation(enemy->status, runN);
			break;
	}
}

void enemySentryRunNewDirection(Enemy * enemy, int oldDirection)
{
	int newDir = dice(0,2);
	if (oldDirection == ENEMY_DIR_NULL)
	{
		newDir = dice(0,3);
	}
	else
	{
		if (newDir == oldDirection)
		{
			newDir = 3;
		}
	}
	enemy->DIRECTION = newDir;
	enemySentrySetRunAnim(enemy);

}


int enemySentryLineCallback(int x, int y)
{
	if (tileIsWallCollision(x >> 1, y >> 1, MOBILITY_FLYING))
	{
		return true;
	}
	return false;
}

bool enemySentrySpotsPlayer(Enemy * enemy)
{
	CharacterStatus * mcStatus = getMainCharStatus();

	int mcX = mcStatus->x >> 2;
	int mcY = mcStatus->y >> 2;

	int eX = enemy->status->x >> 2;
	int eY = enemy->status->y >> 2;

	
	int dx = mcX - eX;
	int dy = mcY - eY;

	switch (enemy->DIRECTION)
	{
		case ENEMY_DIR_N:
			if ((dy > 0) ||  (abs(dx) > abs(dy)))
				return false;
			break;
		case ENEMY_DIR_S:
			if ((dy < 0) ||  (abs(dx) > abs(dy)))
				return false;
			break;
		case ENEMY_DIR_W:
			if ((dx > 0) ||  (abs(dx) < abs(dy)))
				return false;
			break;
		case ENEMY_DIR_E:
			if ((dx < 0) ||  (abs(dx) < abs(dy)))
				return false;
			break;
	}

	if ((eX < 0) || (eY < 0) || (mcX < 0) || (mcY < 0))
	{
		return false;
	}
	/*
	if (bresenhamLine(eX, eY, mcX, mcY, enemySentryLineCallback) > 0)
	{
		return false;
	}
	*/


	return true;
}


int enemySentryUpdate(Enemy * enemy)
{

	enemy->FRAME_CTR++;

	if (enemy->STATE == SENTRY_STATE_LOOKING)
	{
		if (enemySentrySpotsPlayer(enemy))
		{
			enemy->STATE = SENTRY_STATE_SPOTTED;
			enemySentrySetRunAnim(enemy);
		}
		if (enemy->FRAME_CTR > WATCH_SPEED)
		{
			enemy->DIRECTION++;
			enemy->DIRECTION &= SENTRY_DIR_MASK;
			enemySentryUpdateWalkAnim(enemy);
			enemy->FRAME_CTR = 0;
		}
	}

	if (enemy->STATE == SENTRY_STATE_SPOTTED)
	{
		if (!moveEnemyDirection(enemy, enemy->DIRECTION))
		{
			enemySentryRunNewDirection(enemy, enemy->DIRECTION);
		}

		if (enemy->FRAME_CTR > (SPEAR_TICK_MIN + dice(0,SPEAR_TICK_RANGE)))
		{
			enemy->FRAME_CTR = 0;
			enemySentryThrowSpear(enemy);
			enemy->STATE = SENTRY_STATE_PAUSING;
		}
					 
	}

	if (enemy->STATE == SENTRY_STATE_PAUSING)
	{
		if (enemy->FRAME_CTR > PAUSE_FRAMES)
		{
			enemy->FRAME_CTR = 0;
			enemySentryRunNewDirection(enemy, enemy->DIRECTION);
			enemy->STATE = SENTRY_STATE_SPOTTED;
		}
	}

	return 0;


}


int enemySentryFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemySentryInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemySentryUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_sentry_db = 
{
	sentry_portraitTiles,
	sentry_portraitPal,
	sentry_portraitMap,
	"Hobgoblin Sentry",
	"Watches for the player",
	"Keeps a keen eye",
	     "out for you",
	null,
	7,8,0
};


const EnemyType enemy_sentry_def = 
{
	12,				//hp
	5,				//collideDamage
	1,				//numSprites
	19,				//bbHeight
	16,				//bbWidth
	-8,				//bbOffx
	-12,			//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemySentryFunc,	//updateFunction
	(short * )&sentryGfxPal,
	&enemy_sentry_db
};

	
