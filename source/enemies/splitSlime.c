#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "slimeGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "../sound.h"
#include "../mainChar.h"
#include "enemies/portraits/slime_portrait.h"

#define Frame(x) (void *)&(enemySpitSlimeCharFrames[x])
#define Anim(x) (void *)&(enemySpitSlimeCharAnim[x])

#define PURPLE_SLIME_SHOOT_RATE 60

#define SM_SQ_POS -8,-8

const characterFrameDef enemySpitSlimeCharFrames[] = 
{
	////FRAME: north 1
	{//0
		FRAME_SMALL_SQUARE(78, false)
	},
	{//1
		FRAME_SMALL_SQUARE(80, false)
	},
	///spit n
	{//2
		FRAME_SMALL_SQUARE(82, false)
	},
	{//3
		FRAME_SMALL_SQUARE(84, false)
	},
	////FRAME: south 1
	{//4
		FRAME_SMALL_SQUARE(88, false)
	},
	{//5
		FRAME_SMALL_SQUARE(90, false)
	},
	{//6
		FRAME_SMALL_SQUARE(92, false)
	},
	{//7
		FRAME_SMALL_SQUARE(94, false)
	},
	////FRAME: east 1
	{//8
		FRAME_SMALL_SQUARE(24, true)
	},
	{//9
		FRAME_SMALL_SQUARE(26, true)
	},
	{//10
		FRAME_SMALL_SQUARE(28, true)
	},
	{//11
		FRAME_SMALL_SQUARE(30, true)
	},
	////FRAME: west 1
	{//12
		FRAME_SMALL_SQUARE(24, false)
	},
	{//13
		FRAME_SMALL_SQUARE(26, false)
	},
	{//14
		FRAME_SMALL_SQUARE(28, false)
	},
	{//15
		FRAME_SMALL_SQUARE(30, false)
	},


};

#define ANIM_RATE 20
#define FAST_RATE 7
#define ANIM_ENTRY_RATE(x,y,r) (r),(u16*)slimeGfxData,Frame(x),Anim(y)
#define ANIM_ENTRY_FAST(x,y) ANIM_ENTRY_RATE(x,y,FAST_RATE)
#define ANIM_ENTRY(x,y) ANIM_ENTRY_RATE(x,y,ANIM_RATE)

const characterAnimDef enemySpitSlimeCharAnim[] = 
{
	{//0 - north frame 1
		ANIM_ENTRY(0,1)
	},
	{//1 - north frame 2
		ANIM_ENTRY(1,0)
	},
	{//2 - n attack 1
		ANIM_ENTRY_FAST(3,3)
	},
	{//3 - n attack 2
		ANIM_ENTRY_FAST(2,0)
	},
	//////////////////////
	{//4 - south frame 1
		ANIM_ENTRY(4,5)
	},
	{//5 - south frame 2
		ANIM_ENTRY(5,4)
	},
	{//6 - s attack 1
		ANIM_ENTRY_FAST(7,7)
	},
	{//7 - s attack 2
		ANIM_ENTRY_FAST(6,4)
	},
	/////////////////////
	{//8 - e frame 1
		ANIM_ENTRY(8,9)
	},
	{//9 - e frame 2
		ANIM_ENTRY(9,8)
	},
	{//10 - e attack 1
		ANIM_ENTRY_FAST(11,11)
	},
	{//11 - e attack 2
		ANIM_ENTRY_FAST(10,8)
	},
	//////////////////
	{//12 - w frame 1
		ANIM_ENTRY(12,13)
	},
	{//13 - w frame 2
		ANIM_ENTRY(13,12)
	},
	{//14 - w attack 1
		ANIM_ENTRY_FAST(15,15)
	},
	{//15 - w attack 2
		ANIM_ENTRY_FAST(14,12)
	},
	
};


#define SLIME_N (characterAnimDef*)&enemySpitSlimeCharAnim[0]
#define SPIT_N (characterAnimDef*)&enemySpitSlimeCharAnim[2]
#define SLIME_S (characterAnimDef*)&enemySpitSlimeCharAnim[4]
#define SPIT_S (characterAnimDef*)&enemySpitSlimeCharAnim[6]
#define SLIME_E (characterAnimDef*)&enemySpitSlimeCharAnim[8]
#define SPIT_E (characterAnimDef*)&enemySpitSlimeCharAnim[10]
#define SLIME_W (characterAnimDef*)&enemySpitSlimeCharAnim[12]
#define SPIT_W (characterAnimDef*)&enemySpitSlimeCharAnim[14]

#define DIR_R 1
#define DIR_D 2
#define DIR_L 3
#define DIR_U 4

#define BULLET_L 76
#define BULLET_R 108
#define BULLET_U 77
#define BULLET_D 109

void enemySpitSlimeSetAnimation(Enemy * enemy)
{
	switch(enemy->state1)
	{
		case 1: 
			setCharacterAnimationForced(enemy->status, SLIME_E);
			break;
		case 2:
			setCharacterAnimationForced(enemy->status, SLIME_S);
			break;
		case 3:
			setCharacterAnimationForced(enemy->status, SLIME_W);
			break;
		case 4:
			setCharacterAnimationForced(enemy->status, SLIME_N);
			break;
	}

}


int enemySpitSlimeInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)slimeGfxData, (characterAnimDef*) enemySpitSlimeCharAnim);
	enemy->state1 = dice(1,4);
	enemySpitSlimeSetAnimation(enemy);
	return 1;
}

extern CharacterStatus * mcStatus;

void enemySpitSlimeShoot(Enemy * enemy)
{
	int xd = 0;
	int yd = 0;
	int bulletTile = 0;
	switch(enemy->state1)
	{
		case DIR_R: 
			xd = 1;
			bulletTile = BULLET_R;
			break;
		case DIR_D:
			yd = 1;
			bulletTile = BULLET_D;
			break;
		case DIR_L:
			xd = -1;
			bulletTile = BULLET_L;
			break;
		case DIR_U:
			yd = -1;
			bulletTile = BULLET_U;
			break;
	}
	
	switch(enemy->state1)
	{
		case 1: 
			setCharacterAnimationForced(enemy->status, SPIT_E);
			break;
		case 2:
			setCharacterAnimationForced(enemy->status, SPIT_S);
			break;
		case 3:
			setCharacterAnimationForced(enemy->status, SPIT_W);
			break;
		case 4:
			setCharacterAnimationForced(enemy->status, SPIT_N);
			break;
	}
	

	fireEnemyBullet(enemy, bulletTile, 6, 6, xd, yd);
	//BulletDef * bullet = fireEnemyBullet(enemy, bulletTile, 6, 6, xd, yd);
	//setSpritePalette(bullet->sprite, findPalette((u16*)&orangeSlimePal));
	
	playGameSfx(SFX_ENEMY_SPIT);
	
}


int enemySpitSlimeUpdate(Enemy * enemy)
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

	if ((enemy->state1 < 1) || (enemy->state1) > 4)
	{
		enemy->state1 = 1;
	}

	enemy->state4++;
	if (enemy->state4 > 20)
	{
		int shoot = dice(1, 40 - enemy->state4);
		if (shoot == 1)
		{
			enemy->state4 = 1;
			enemySpitSlimeShoot(enemy);
		}
	}


	int xd = 0;
	int yd = 0;

	switch(enemy->state1)
	{
		case 1: 
			xd = 1;
			break;
		case 2:
			yd = 1;
			break;
		case 3:
			xd = -1;
			break;
		case 4:
			yd = -1;
			break;
	}

	enemy->state2++;
	if (!moveEnemy(enemy, xd, yd) || (enemy->state2 > dice(20, 90)))
	{
		enemy->state2 = 0;
		enemy->state1 = dice(1,4);
		enemySpitSlimeSetAnimation(enemy);
	}


	return 1;
}

int enemyPurpleSlimeUpdate(Enemy * enemy)
{
	enemy->state3++;
	
	if (enemy->state3 >= PURPLE_SLIME_SHOOT_RATE)
	{
		enemy->state3 = 0;
		enemySpitSlimeShoot(enemy);
	}
	
	if (enemy->state3 & 1)
	{
		int tmp = enemy->state1;
		int dx = enemy->status->x - getMainCharX();
		int dy = enemy->status->y - getMainCharY();
		
		if (abs(dx) > abs(dy))
		{
			moveEnemy(enemy, 0, -unitVal(dy));
			moveEnemy(enemy, unitVal(dx), 0);
			if (dx > 0)
			{
				enemy->state1 = 3;
			}
			else
			{
				enemy->state1 = 1;
			}
		}
		else
		{
			moveEnemy(enemy, 0, unitVal(dy));
			moveEnemy(enemy, -unitVal(dx), 0);
			if (dy > 0)
			{
				enemy->state1 = 4;
			}
			else
			{
				enemy->state1 = 2;
			}
		}
		
		if (enemy->state1 != tmp)
		{
			enemySpitSlimeSetAnimation(enemy);
		}
		
	}
	
	return 1;
}

int enemySpitSlimeFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemySpitSlimeInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemySpitSlimeUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}



const EnemyDatabaseDef enemy_spit_slime_db = 
{
	slime_portrait_Tiles,
	 orange_slime_portraitPal,
	slime_portrait_Map,
	"Orange Slime",
	"Gross orange blob",
	"Spits blobs of goo.",
	     "",
	null,
	0,8,0
};


const EnemyType enemy_spit_slime_def = 
{
	3,				//hp
	2,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	2,				//bbOffx
	2,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemySpitSlimeFunc,	//updateFunction
	(short*)&orangeSlimePal,
	&enemy_spit_slime_db
};
	
int enemyPurpleSlimeFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemySpitSlimeInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyPurpleSlimeUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}



const EnemyDatabaseDef enemy_purpleSlime_db = 
{
	slime_portrait_Tiles,
	slime_portrait_Pal + 32,
	slime_portrait_Map,
	"Purple Slime",
	"Gross purple blob",
	"Retreats while ",
	     "spitting",
	null,
	0,7,0
};


const EnemyType enemy_purpleSlime_def = 
{
	8,				//hp
	4,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	2,				//bbOffx
	2,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyPurpleSlimeFunc,	//updateFunction
	(short*)&purpleSlimePal,
	&enemy_purpleSlime_db
};
	
	
