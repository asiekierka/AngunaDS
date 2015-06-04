#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "multiSmall2Gfx.h"
#include "../misc.h"
#include "../characters.h"
#include "portraits/flame_portrait.h"

#define Frame(x) (void *)&(enemyWallHuggerCharFrames[x])
#define Anim(x) (void *)&(enemyWallHuggerCharAnim[x])


const characterFrameDef enemyWallHuggerCharFrames[] = 
{
	{//0
		FRAME_SMALL_SQUARE(20,false)
	},
	{//1
		FRAME_SMALL_SQUARE(22,false)
	},
	{//2
		FRAME_SMALL_SQUARE(24,false)
	},

};

#define ANIM_ENTRY(x,y) 9,(u16*)multiSmall2GfxTiles,Frame(x),Anim(y)

#define FRAME_1 (characterAnimDef*)&enemyWallHuggerCharAnim[0]
#define FRAME_2 (characterAnimDef*)&enemyWallHuggerCharAnim[1]
#define FRAME_3 (characterAnimDef*)&enemyWallHuggerCharAnim[2]


const characterAnimDef enemyWallHuggerCharAnim[] = 
{
	{//0 - north frame 1
		ANIM_ENTRY(0,1)
	},
	{//1 - north frame 2
		ANIM_ENTRY(1,2)
	},
	{//2 - south frame 1
		ANIM_ENTRY(2,3)
	},
	{//3 - south frame 2
		ANIM_ENTRY(1,0)
	},
};




int enemyWallHuggerInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)multiSmall2GfxTiles, (characterAnimDef*) enemyWallHuggerCharAnim);
	enemy->state3 = dice(0,3);
	return 1;
}

extern CharacterStatus * mcStatus;

int enemyWallHuggerUpdate(Enemy * enemy)
{


	//if we've hit a wall, change our movement to clockwise
	if (!moveEnemyDirection(enemy, enemy->state3))
	{
		enemy->state3 = DIR_CW(enemy->state3);
		enemy->state4 = 0;
	}
	else
	{
		//make sure we're still sitting on a wall...
		//if not, then change our movement to counter clockwise
		//but if he's tired of looping (state4  >= 8), don't do it....
		if (enemy->state4 < 32)
		{
			int tmp = enemy->state3;
			enemy->state3 = DIR_CCW(enemy->state3);
			if (!(moveEnemyDirection(enemy, enemy->state3)))
			{
				enemy->state3 = tmp;
			}
			else
			{
				enemy->state4++;
			}
		}
	}
	

	return 1;
}

int enemyWallHuggerFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyWallHuggerInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyWallHuggerUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_wallHugger_db = 
{
	flame_portrait_Tiles,
	flame_portrait_Pal,
	flame_portrait_Map,
	"Will O Wisp",
	"Eerie burning flame",
	"Circles along",
	"the wall",
	null,
	4,5,0
	
};


const EnemyType enemy_wallHugger_def = 
{
	4,				//hp
	4,				//collideDamage
	1,				//numSprites
	12,				//bbHeight
	12,				//bbWidth
	3,				//bbOffx
	3,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyWallHuggerFunc,	//updateFunction
	(short*)&flamePal,
	&enemy_wallHugger_db
};
	
