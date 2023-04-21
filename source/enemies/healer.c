#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "../misc.h"
#include "../characters.h"
#include "../healer.h"
#include "vampireGfx.h"
#include "enemies/portraits/ogre_portrait.h"

#define Frame(x) (void *)&(enemyHealerCharFrames[x])
#define Anim(x) (void *)&(enemyHealerCharAnim[x])

#define TILE(x) {0,-16,(x),TALL,SIZE_32, false, null}
#define TILEF(x) {0,-16,(x),TALL,SIZE_32, true, null}

enum
{
	HEALER_S_A,
};

const characterFrameDef enemyHealerCharFrames[] = 
{
	[HEALER_S_A] = TILE(14),

};

#define walkS Anim(HEALER_S_A)

#define WALK_SPEED 9999

#define WALK(x,y) {WALK_SPEED,(u16*)vampireGfxTiles,Frame(x),Anim(y)}

#define TIMEOUT state1
#define HEALER_REPEAT_DELAY 90

const characterAnimDef enemyHealerCharAnim[] = 
{
	[HEALER_S_A] =  WALK(HEALER_S_A, HEALER_S_A),
};


int enemyHealerInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)vampireGfxTiles, (characterAnimDef*) enemyHealerCharAnim);
	return 1;
}

extern CharacterStatus * mcStatus;

int enemyHealerUpdate(Enemy * enemy)
{
	if (enemy->TIMEOUT > 0)
	{
		enemy->TIMEOUT--;
	}

	return 0;
}

int enemyHealerCollide(Enemy * enemy)
{
	if (enemy->TIMEOUT < 1)
	{
		doHealer(enemy->special);
		enemy->TIMEOUT = HEALER_REPEAT_DELAY;
	}

	return ENEMY_COLLIDE_NO_DMG;
}


int enemyHealerFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyHealerInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyHealerUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
		//can't kill a Healer!
		case ENEMY_BULLET_COLLIDE:
			enemyHealerCollide(enemy);
			return 0;
			break;
		case ENEMY_MC_COLLIDE:
			return enemyHealerCollide(enemy);
	}
	return 1;
}



const EnemyType enemy_healer_def = 
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
	enemyHealerFunc,	//updateFunction
	(short * )&oldManGfxPal,
	null
};
	
