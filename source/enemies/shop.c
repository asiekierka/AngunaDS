#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "../misc.h"
#include "../characters.h"
#include "../shop.h"
#include "vampireGfx.h"
#include "portraits/ogre_portrait.h"

#define Frame(x) (void *)&(enemyShopCharFrames[x])
#define Anim(x) (void *)&(enemyShopCharAnim[x])

#define TILE(x) {0,-16,(x),TALL,SIZE_32, false, null}
#define TILEF(x) {0,-16,(x),TALL,SIZE_32, true, null}

enum
{
	SHOP_S_A,
};

const characterFrameDef enemyShopCharFrames[] = 
{
	[SHOP_S_A] = TILE(12),

};

#define walkS Anim(SHOP_S_A)

#define WALK_SPEED 9999

#define WALK(x,y) {WALK_SPEED,(u16*)vampireGfxTiles,Frame(x),Anim(y)}

#define TIMEOUT state1
#define SHOP_REPEAT_DELAY 90

const characterAnimDef enemyShopCharAnim[] = 
{
	[SHOP_S_A] =  WALK(SHOP_S_A, SHOP_S_A),
};


int enemyShopInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)vampireGfxTiles, (characterAnimDef*) enemyShopCharAnim);
	return 1;
}

extern CharacterStatus * mcStatus;

int enemyShopUpdate(Enemy * enemy)
{
	if (enemy->TIMEOUT > 0)
	{
		enemy->TIMEOUT--;
	}

	return 0;
}

int enemyShopCollide(Enemy * enemy)
{
	if (enemy->TIMEOUT < 1)
	{
		doShop(enemy->special);
		enemy->TIMEOUT = SHOP_REPEAT_DELAY;
	}

	return ENEMY_COLLIDE_NO_DMG;
}


int enemyShopFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyShopInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyShopUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
		//can't kill a shop!
		case ENEMY_BULLET_COLLIDE:
			enemyShopCollide(enemy);
			return 0;
			break;
		case ENEMY_MC_COLLIDE:
			return enemyShopCollide(enemy);
	}
	return 1;
}



const EnemyType enemy_shop_def = 
{
	1,				//hp
	0,				//collideDamage
	1,				//numSprites
	16,				//bbHeight
	12,				//bbWidth
	3,				//bbOffx
	0,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyShopFunc,	//updateFunction
	(short * )&shopKeeperGfxPal,
	null
};
	
