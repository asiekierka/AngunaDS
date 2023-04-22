#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "slimeGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "slimeGfx.h"
#include "enemies/portraits/zipper_portrait.h"

#define Frame(x) (void *)&(enemyZipperCharFrames[x])
#define Anim(x) (void *)&(enemyZipperCharAnim[x])


const characterFrameDef enemyZipperCharFrames[] = 
{
	////FRAME: north 1
	{//0
		FRAME_SMALL_SQUARE(4,false)
	},
	////FRAME: north 2
	{//1
		FRAME_SMALL_SQUARE(6,false)
	},
	////FRAME: south 1
	{//2
		FRAME_SMALL_SQUARE(72,false)
	},
	////FRAME: south 2
	{//3
		FRAME_SMALL_SQUARE(74,false)
	},
	////FRAME: east 1
	{//4
		FRAME_SMALL_SQUARE(8,true)
	},
	////FRAME: east 2
	{//5
		FRAME_SMALL_SQUARE(10,true)
	},
	////FRAME: west 1
	{//6
		FRAME_SMALL_SQUARE(8,false)
	},
	////FRAME: west 2
	{//7
		FRAME_SMALL_SQUARE(10,false)
	},

};

#define ANIM_ENTRY(x,y) 9,(u16*)slimeGfxData,Frame(x),Anim(y)

#define SLIME_N (characterAnimDef*)&enemyZipperCharAnim[0]
#define SLIME_S (characterAnimDef*)&enemyZipperCharAnim[2]
#define SLIME_E (characterAnimDef*)&enemyZipperCharAnim[4]
#define SLIME_W (characterAnimDef*)&enemyZipperCharAnim[6]


const characterAnimDef enemyZipperCharAnim[] = 
{
	{//0 - north frame 1
		ANIM_ENTRY(0,1)
	},
	{//1 - north frame 2
		ANIM_ENTRY(1,0)
	},
	{//2 - south frame 1
		ANIM_ENTRY(2,3)
	},
	{//3 - south frame 2
		ANIM_ENTRY(3,2)
	},
	{//4 - east frame 1
		ANIM_ENTRY(4,5)
	},
	{//5 - east frame 2
		ANIM_ENTRY(5,4)
	},
	{//6 - west frame 1
		ANIM_ENTRY(6,7)
	},
	{//7 - west frame 2
		ANIM_ENTRY(7,6)
	},
};




int enemyZipperInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)slimeGfxData, (characterAnimDef*) enemyZipperCharAnim);
	enemy->state3 = dice(0,3);
	return 1;
}

extern CharacterStatus * mcStatus;

int enemyZipperUpdate(Enemy * enemy)
{

	int xd = 0;
	int yd = 0;

	switch(enemy->state3)
	{
		case 0:
			xd = 1;
			break;
		case 1:
			xd = -1;
			break;
		case 2:
			yd = 1;
			break;
		case 3:
			yd =-1;
			break;
	}

	enemy->state2++;
	if (!moveEnemy(enemy, xd, yd) || (enemy->state2 > dice(20, 90)))
	{
		enemy->state2 = 0;
		enemy->state3 = dice(0,3);
		switch (enemy->state3)
		{
			case 0:
				setCharacterAnimation(enemy->status, SLIME_E);
				break;
			case 1:
				setCharacterAnimation(enemy->status, SLIME_W);
				break;
			case 2:
				setCharacterAnimation(enemy->status, SLIME_S);
				break;
			case 3:
				setCharacterAnimation(enemy->status, SLIME_N);
				break;

	
		}
	}

	return 1;
}

int enemyZipperFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyZipperInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyZipperUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_zipper_db = 
{
	zipper_portraitTiles,
	zipper_portraitPal,
	zipper_portraitMap,
	"Green Slime",
	"Slimy little green blob",
	"Zips back and forth in ",
	     "random movements.",
	null,
	0,5,0
	
};


const EnemyType enemy_zipper_def = 
{
	1,				//hp
	2,				//collideDamage
	1,				//numSprites
	12,				//bbHeight
	12,				//bbWidth
	3,				//bbOffx
	3,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyZipperFunc,	//updateFunction
	(short*)&greenSlimePal,
	&enemy_zipper_db
};

const EnemyType enemy_hard_zipper_def = 
{
	15,				//hp
	8,				//collideDamage
	1,				//numSprites
	12,				//bbHeight
	12,				//bbWidth
	3,				//bbOffx
	3,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyZipperFunc,	//updateFunction
	(short*)&greenSlimePal,
	&enemy_zipper_db
};
	
