#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "../sound.h"
#include "shieldHobgoblinGfx.h"
#include "sentryGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "../mainChar.h"
#include "../level.h"
#include "itemGfx.h"
#include "enemies/portraits/shieldHobgoblin_portrait.h"

#define Frame(x) (void *)&(enemyShieldHobgoblinCharFrames[x])
#define Anim(x) (void *)&(enemyShieldHobgoblinCharAnim[x])

#define TILE_L(x) {-12,-20,(x),SQUARE,SIZE_32, false, null}
#define TILE_L_F(x) {-20,-20,(x),SQUARE,SIZE_32, true, null}

#define TILE_R(x) {-20,-20,(x),SQUARE,SIZE_32, false, null}
#define TILE_R_F(x) {-12,-20,(x),SQUARE,SIZE_32, true, null}

#define TILE_LB(x) {-13,-20,(x),SQUARE,SIZE_32, false, null}
#define TILE_RB(x) {-21,-20,(x),SQUARE,SIZE_32, false, null}

enum
{
	SHIELD_N_A,
	SHIELD_N_B,
	SHIELD_N_C,

	SHIELD_S_A,
	SHIELD_S_B,
	SHIELD_S_C,

	SHIELD_L_A,
	SHIELD_L_B,
	SHIELD_L_C,

	SHIELD_R_A,
	SHIELD_R_B,
	SHIELD_R_C,

	SHIELD_RUN_N_A,
	SHIELD_RUN_N_B,
	SHIELD_RUN_N_C,
	SHIELD_RUN_N_D,

	SHIELD_RUN_S_A,
	SHIELD_RUN_S_B,
	SHIELD_RUN_S_C,
	SHIELD_RUN_S_D,

	SHIELD_RUN_L_A,
	SHIELD_RUN_L_B,
	SHIELD_RUN_L_C,
	SHIELD_RUN_L_D,

	SHIELD_RUN_R_A,
	SHIELD_RUN_R_B,
	SHIELD_RUN_R_C,
	SHIELD_RUN_R_D,

	
};

const characterFrameDef enemyShieldHobgoblinCharFrames[] = 
{
	[SHIELD_N_A] = TILE_L(0),
	[SHIELD_N_B] = TILE_R(3),
	[SHIELD_N_C] = TILE_L(7),

	[SHIELD_R_A] = TILE_R(10),
	[SHIELD_R_B] = TILE_L(14),
	[SHIELD_R_C] = TILE_R(17),

	[SHIELD_S_A] = TILE_L(21),
	[SHIELD_S_B] = TILE_R(24),
	[SHIELD_S_C] = TILE_R(28),

	[SHIELD_L_A] = TILE_LB(0),
	[SHIELD_L_B] = TILE_RB(3),
	[SHIELD_L_C] = TILE_LB(7),

};

#define walkN Anim(SHIELD_N_A)
#define walkL Anim(SHIELD_L_A)
#define walkR Anim(SHIELD_R_A)
#define walkS Anim(SHIELD_S_A)
#define runN  Anim(SHIELD_RUN_N_A)
#define runL Anim(SHIELD_RUN_L_A)
#define runR Anim(SHIELD_RUN_R_A)
#define runS Anim(SHIELD_RUN_S_A)

#define WATCH_SPEED 180
#define RUN_SPEED 10

#define SPEAR_TICK_MIN 60
#define SPEAR_TICK_RANGE 180

#define PAUSE_FRAMES 20

#define DIRECTION state1
#define FRAME_CTR state2
#define STATE state3
#define NOISE_TIMER state4

#define GFX_TILES shieldHobgoblinGfxTiles
#define SENTRY_TILES sentryGfxTiles

#define WALK(x,y) {0,(u16*)GFX_TILES,Frame(x),Anim(y)}
#define RUN(x,y) {RUN_SPEED,(u16*)GFX_TILES,Frame(x),Anim(y)}
#define RUNN(x,y) {RUN_SPEED,(u16*)SENTRY_TILES,Frame(x),Anim(y)}

#define SHIELD_DIR_MASK 0x3

#define SHIELD_STATE_LOOKING 1
#define SHIELD_STATE_CHANGEUP 0
#define SHIELD_STATE_SPOTTED 3
#define SHIELD_STATE_PAUSING 4


const characterAnimDef enemyShieldHobgoblinCharAnim[] = 
{
	[SHIELD_N_A] =  WALK(SHIELD_N_A,SHIELD_N_A),
	[SHIELD_S_A] =  WALK(SHIELD_S_A,SHIELD_S_A),
	[SHIELD_L_A] =  WALK(SHIELD_L_A,SHIELD_L_A),
	[SHIELD_R_A] =  WALK(SHIELD_R_A,SHIELD_R_A),

	[SHIELD_RUN_N_A] =  RUNN(SHIELD_N_A,SHIELD_RUN_N_B),
	[SHIELD_RUN_N_B] =  RUNN(SHIELD_N_B,SHIELD_RUN_N_C),
	[SHIELD_RUN_N_C] =  RUNN(SHIELD_N_C,SHIELD_RUN_N_D),
	[SHIELD_RUN_N_D] =  RUNN(SHIELD_N_B,SHIELD_RUN_N_A),

	[SHIELD_RUN_S_A] =  RUN(SHIELD_S_A,SHIELD_RUN_S_B),
	[SHIELD_RUN_S_B] =  RUN(SHIELD_S_B,SHIELD_RUN_S_C),
	[SHIELD_RUN_S_C] =  RUN(SHIELD_S_C,SHIELD_RUN_S_D),
	[SHIELD_RUN_S_D] =  RUN(SHIELD_S_B,SHIELD_RUN_S_A),
	
	[SHIELD_RUN_L_A] =  RUN(SHIELD_L_A,SHIELD_RUN_L_B),
	[SHIELD_RUN_L_B] =  RUN(SHIELD_L_B,SHIELD_RUN_L_C),
	[SHIELD_RUN_L_C] =  RUN(SHIELD_L_C,SHIELD_RUN_L_D),
	[SHIELD_RUN_L_D] =  RUN(SHIELD_L_B,SHIELD_RUN_L_A),

	[SHIELD_RUN_R_A] =  RUN(SHIELD_R_A,SHIELD_RUN_R_B),
	[SHIELD_RUN_R_B] =  RUN(SHIELD_R_B,SHIELD_RUN_R_C),
	[SHIELD_RUN_R_C] =  RUN(SHIELD_R_C,SHIELD_RUN_R_D),
	[SHIELD_RUN_R_D] =  RUN(SHIELD_R_B,SHIELD_RUN_R_A),


};



void enemyShieldHobgoblinRunNewDirection(Enemy * enemy, int oldDirection);

int enemyShieldHobgoblinInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)GFX_TILES, (characterAnimDef*) enemyShieldHobgoblinCharAnim);
	initEnemyGfx(enemy, (u16*)SENTRY_TILES, (characterAnimDef*) enemyShieldHobgoblinCharAnim);
	enemy->STATE = SHIELD_STATE_SPOTTED;
	enemy->NOISE_TIMER = 0;
	enemyShieldHobgoblinRunNewDirection(enemy, ENEMY_DIR_NULL);
	return 1;
}


void enemyShieldHobgoblinThrowSpear(Enemy * enemy)
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




void enemyShieldHobgoblinSetRunAnim(Enemy * enemy)
{
	switch(enemy->DIRECTION)
	{
		case ENEMY_DIR_E: 
			enemy->status->gfxBlock = loadSpriteGfxBlock((u16*)GFX_TILES);
			setCharacterAnimation(enemy->status, runR);
			break;
		case ENEMY_DIR_S:
			enemy->status->gfxBlock = loadSpriteGfxBlock((u16*)GFX_TILES);
			setCharacterAnimation(enemy->status, runS);
			break;
		case ENEMY_DIR_W:
			enemy->status->gfxBlock = loadSpriteGfxBlock((u16*)GFX_TILES);
			setCharacterAnimation(enemy->status, runL);
			break;
		case ENEMY_DIR_N:
			enemy->status->gfxBlock = loadSpriteGfxBlock((u16*)SENTRY_TILES);
			setCharacterAnimation(enemy->status, runN);
			break;
	}
}

void enemyShieldHobgoblinRunNewDirection(Enemy * enemy, int oldDirection)
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
	enemyShieldHobgoblinSetRunAnim(enemy);

}




int enemyShieldHobgoblinUpdate(Enemy * enemy)
{

	enemy->FRAME_CTR++;
	if (enemy->NOISE_TIMER > 0)
	{
		enemy->NOISE_TIMER--;
	}

	if (enemy->STATE == SHIELD_STATE_LOOKING)
	{
			enemy->STATE = SHIELD_STATE_SPOTTED;
			enemyShieldHobgoblinRunNewDirection(enemy, ENEMY_DIR_NULL);
	}

	if (enemy->STATE == SHIELD_STATE_SPOTTED)
	{
		if (!moveEnemyDirection(enemy, enemy->DIRECTION))
		{
			enemyShieldHobgoblinRunNewDirection(enemy, enemy->DIRECTION);
		}

		if (enemy->FRAME_CTR > (SPEAR_TICK_MIN + dice(0,SPEAR_TICK_RANGE)))
		{
			enemy->FRAME_CTR = 0;
			enemyShieldHobgoblinThrowSpear(enemy);
			enemy->STATE = SHIELD_STATE_PAUSING;
		}
					 
	}

	if (enemy->STATE == SHIELD_STATE_PAUSING)
	{
		if (enemy->FRAME_CTR > PAUSE_FRAMES)
		{
			enemy->FRAME_CTR = 0;
			enemyShieldHobgoblinRunNewDirection(enemy, enemy->DIRECTION);
			enemy->STATE = SHIELD_STATE_SPOTTED;
		}
	}

	return 0;


}

int enemyShieldHobgoblinBulletCollide(Enemy * enemy)
{
	BulletDef * bullet = enemy->bulletCheck;
	int sourceX = bullet->sourceX;
	int sourceY = bullet->sourceY;

	int dx = sourceX - enemy->status->x;
	int dy = sourceY - enemy->status->y;

	bool blocked = false;

	bool northBlock = ((dy < 0) && (abs(dy) > abs(dx)));
	bool southBlock = ((dy > 0) && (abs(dy) > abs(dx)));
	bool leftBlock  = ((dx < 0) && (abs(dy) < abs(dx)));
	bool rightBlock = ((dx > 0) && (abs(dy) < abs(dx)));

	switch(enemy->DIRECTION)
	{
		case ENEMY_DIR_N:
			blocked = northBlock || leftBlock;
			break;
		case ENEMY_DIR_S:
			blocked = southBlock || rightBlock;
			break;
		case ENEMY_DIR_E:
			blocked = rightBlock || northBlock;
			break;
		case ENEMY_DIR_W:
			blocked = leftBlock || southBlock;
			break;
	}

	if (blocked)
	{
		if (enemy->NOISE_TIMER == 0)
		{
			playGameSfx(SFX_ARMOR);
			enemy->NOISE_TIMER = 15;
		}
		return NO_COLLISION;
	}

	return DOES_DAMAGE;
	
}


int enemyShieldHobgoblinFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyShieldHobgoblinInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyShieldHobgoblinUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
		case ENEMY_BULLET_COLLIDE:
			return enemyShieldHobgoblinBulletCollide(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_shieldHobgoblin_db = 
{
	shieldHobgoblin_portrait_Tiles,
	shieldHobgoblin_portrait_Pal,
	shieldHobgoblin_portrait_Map,
	"Shield Hobgoblin",
	"Carries a big shield",
	"This guy can",
	     "defend himself",
	null,
	7,7,0
};


const EnemyType enemy_shieldHobgoblin_def = 
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
	enemyShieldHobgoblinFunc,	//updateFunction
	(short * )&shieldHobgoblinGfxPal,
	&enemy_shieldHobgoblin_db
};

	
