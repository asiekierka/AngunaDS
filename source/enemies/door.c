#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "../misc.h"
#include "../characters.h"
#include "../item.h"
#include "../mainChar.h"
#include "../sound.h"
#include "../gameState.h"
#include "../door.h"
#include "../level.h"
#include "doorGfx.h"
#include "itemGfx.h"


#define Frame(x) (void *)&(enemyDoorCharFrames[x])
#define Anim(x) (void *)&(enemyDoorCharAnim[x])

#define DOOR_OPEN 1
#define DOOR_CLOSED 0
#define DELAY_BETWEEN_ENEMY_DEAD_CHECKS 2

#define DEAD state4

void enemyDoorOpen(Enemy * enemy);

enum
{
	NOTHING,
	HORIZ_DOOR,
	HORIZ_DOOR_B,
	VERT_DOOR_L,
	VERT_DOOR_L_B,
	VERT_DOOR_R,
	VERT_DOOR_R_B,
	HORIZ_DOOR_HIDDEN,
	VERT_DOOR_L_HIDDEN,
	VERT_DOOR_R_HIDDEN,
	HORIZ_DOOR_INVIS,
	VERT_DOOR_L_INVIS_A,
	VERT_DOOR_L_INVIS_B,
	VERT_DOOR_R_INVIS_A,
	VERT_DOOR_R_INVIS_B,
	
};

const characterFrameDef enemyDoorCharFrames[] =
{
	[NOTHING] ={
		0,0,
		0,
		SQUARE,
		SIZE_8,
		false,
		null
	},

	////FRAME: horiz door
	[HORIZ_DOOR] = {//0
		-8,-4,				//x,y offset
		TILE_SPECIAL,		//tile
		SQUARE,				//shape (SQUARE/TALL/WIDE)
		SIZE_16,			//size
		false, 				//flip
		Frame(HORIZ_DOOR_B)	//nextSprite
	},
	[HORIZ_DOOR_B] = {
		-16,-16,	
		0,	
		SQUARE,	
		SIZE_32,
		false, 
		null
	},


	//FRAME:  vertical door L
	[VERT_DOOR_L] = {
		6,-8,				//x,y offset
		TILE_SPECIAL,		//tile
		SQUARE,				//shape (SQUARE/TALL/WIDE)
		SIZE_16,			//size
		false, 				//flip
		Frame(VERT_DOOR_L_B)//nextSprite
	},
	[VERT_DOOR_L_B] = {
		//2
		8,-16,	
		4,	
		TALL,
		SIZE_16,
		false, 
		null
	},


	//FRAME:  vert door R
	[VERT_DOOR_R] = {
		-8,-8,				//x,y offset
		TILE_SPECIAL,		//tile
		SQUARE,				//shape (SQUARE/TALL/WIDE)
		SIZE_16,			//size
		false, 				//flip
		Frame(VERT_DOOR_R_B)//nextSprite
	},

	[VERT_DOOR_R_B] = {
		0,-16,		
		4,	
		TALL,
		SIZE_16,
		true, 	
		null
	},

	//FRAME: horiz door hidden
	[HORIZ_DOOR_HIDDEN] = {
		-16,-16,	
		5,	
		SQUARE,	
		SIZE_32,
		false, 
		null
	},

	//FRAME:  vert door L hidden
	[VERT_DOOR_L_HIDDEN] = {
		//2
		8,-16,	
		9,	
		TALL,
		SIZE_16,
		false, 
		null
	},

	//FRAME: vert door R hidden
	[VERT_DOOR_R_HIDDEN] = {
		0,-16,		
		9,	
		TALL,
		SIZE_16,
		true, 	
		null
	},


	[HORIZ_DOOR_INVIS] = {
		-16,-16,	
		28,	
		SQUARE,	
		SIZE_32,
		false, 
		null
	},

	[VERT_DOOR_L_INVIS_A] = {
		0,-16,
		90,SQUARE, SIZE_16, false, Frame(VERT_DOOR_L_INVIS_B)
	},

	[VERT_DOOR_L_INVIS_B] = {
		0,0,
		90,SQUARE, SIZE_16, false, null
	},


	[VERT_DOOR_R_INVIS_A] = {
		0,-16,
		90,SQUARE, SIZE_16, true, Frame(VERT_DOOR_R_INVIS_B)
	},

	[VERT_DOOR_R_INVIS_B] = {
		0,0,
		90,SQUARE, SIZE_16, true, null
	}




};


//for the blank tile....
extern const int mainCharGfxData[];

enum
{
	ANIM_NOTHING,
	ANIM_HORIZ_DOOR,
	ANIM_VERT_DOOR_L,
	ANIM_VERT_DOOR_R,
	ANIM_HORIZ_DOOR_HIDDEN,
	ANIM_VERT_DOOR_L_HIDDEN,
	ANIM_VERT_DOOR_R_HIDDEN,
	ANIM_HORIZ_DOOR_INVIS,
	ANIM_VERT_DOOR_L_INVIS,
	ANIM_VERT_DOOR_R_INVIS,
};

#define DOOR_ANIM(x) {99999,PUN(u16*)mainCharGfxData,Frame(x),null}

const characterAnimDef enemyDoorCharAnim[] = 
{
	[ANIM_NOTHING] = DOOR_ANIM(NOTHING),
	[ANIM_HORIZ_DOOR] = DOOR_ANIM(HORIZ_DOOR),
	[ANIM_VERT_DOOR_L] = DOOR_ANIM(VERT_DOOR_L),
	[ANIM_VERT_DOOR_R] = DOOR_ANIM(VERT_DOOR_R),
	[ANIM_HORIZ_DOOR_HIDDEN] = DOOR_ANIM(HORIZ_DOOR_HIDDEN),
	[ANIM_VERT_DOOR_L_HIDDEN] = DOOR_ANIM(VERT_DOOR_L_HIDDEN),
	[ANIM_VERT_DOOR_R_HIDDEN] = DOOR_ANIM(VERT_DOOR_R_HIDDEN),
	[ANIM_HORIZ_DOOR_INVIS] = DOOR_ANIM(HORIZ_DOOR_INVIS),
	[ANIM_VERT_DOOR_L_INVIS] = DOOR_ANIM(VERT_DOOR_L_INVIS_A),
	[ANIM_VERT_DOOR_R_INVIS] = DOOR_ANIM(VERT_DOOR_R_INVIS_A),
};

void enemyDoorSetInvisPalette(Enemy * enemy)
{

	LevelData * level = getCurrentLevel();
	u16 * pal = level->palette + 16;
	PaletteId bgPal = loadPalette(pal);

	int i;
	for (i = 0; i < 5; i++)
	{
		Sprite * door = enemy->status->spriteList[i];
		setSpritePalette(door, bgPal);
	}
	
}

int enemyDoorInit(Enemy * enemy)
{
	if (ITEM_SAVES_STATE(enemy->special) && (getGameState(ITEM_SAVE_STATE(enemy->special))))
	{
		enemy->DEAD = true;
	}

	int lockTile = DOOR_COLOR_TO_TILE(enemy->itemDrop);
	if (lockTile == TEST_HAS_NO_LOCK)
	{
		lockTile = SET_NO_LOCK_TILE;
	}
	else
	{

		if (DOOR_DIRECTION(enemy->itemDrop) == DOOR_VERTICAL_L)
		{
			lockTile += 2;
		}
		
		if (DOOR_DIRECTION(enemy->itemDrop) == DOOR_VERTICAL_R)
		{
			lockTile += 4;
		}
	}

	enemy->status->special = lockTile;


	if (DOOR_APPEARS_LATE(enemy->special))
	{
		initEnemyGfx(enemy, (u16*)doorGfxTiles, (characterAnimDef*) &enemyDoorCharAnim[ANIM_NOTHING]);
		enemy->state1 = DOOR_OPEN;
	}
	else
	{
		initEnemyGfx(enemy, (u16*)doorGfxTiles, (characterAnimDef*) &enemyDoorCharAnim[DOOR_FRAME(enemy->itemDrop)]);
		enemy->state1 = DOOR_CLOSED;
	}

	//if it has a lock, change the lock's palette
	if (lockTile != SET_NO_LOCK_TILE)
	{
		Sprite * lock = enemy->status->spriteList[0];
		PaletteId lockPal = loadPalette((u16*)lockGfxPal);
		setSpritePalette(lock, lockPal);
	}
	else
	{
		if (enemy->itemDrop & DOOR_INVIS)
		{
			enemyDoorSetInvisPalette(enemy);
		}
	}

	if (IS_VERTICAL(DOOR_DIRECTION(enemy->itemDrop)))
	{
		//for vertical doors, swap the collision data
		int tmp;
		tmp = enemy->status->boundingBlockOffsetX;
		enemy->status->boundingBlockOffsetX = enemy->status->boundingBlockOffsetY;
		enemy->status->boundingBlockOffsetY = tmp;

		tmp = enemy->status->boundingBlockWidth;
		enemy->status->boundingBlockWidth = enemy->status->boundingBlockHeight;
		enemy->status->boundingBlockHeight = tmp;
	
	}
	return 1;
}


void enemyDoorClearShadows(Enemy * enemy)
{
	int tilex = enemy->status->x >> 3;
	int tiley = enemy->status->y >> 3;
	tilex+=2;
	int tile = getTileAtWorldCoord(tilex, tiley);

	if (tile == 84)
	{
		writeTileToWorldCoord(tilex, tiley, 24);
		writeTileToWorldCoord(tilex, tiley-2, 24);

		writeTileToWorldCoord(++tilex, tiley, 25);
		writeTileToWorldCoord(tilex, tiley-2, 25);

		writeTileToWorldCoord(--tilex, ++tiley, 26);
		writeTileToWorldCoord(tilex, tiley-2, 26);

		writeTileToWorldCoord(++tilex, tiley, 27);
		writeTileToWorldCoord(tilex, tiley-2, 27);

	}
}

void enemyDoorOpen(Enemy * enemy)
{
	if (enemy->state1 == DOOR_OPEN)
	{
		return;
	}

	enemy->state1 = DOOR_OPEN;
	playGameSfx(SFX_DOOR);

						//resetEnemy(enemy);

	initEnemyGfx(enemy, (u16*)doorGfxTiles, (characterAnimDef*) &enemyDoorCharAnim[ANIM_NOTHING]);


	if (DOOR_APPEARS_LATE(enemy->special))
	{
		enemy->DEAD = true;
		enemyDoorClearShadows(enemy);
	}

	if (ITEM_SAVES_STATE(enemy->special))
	{
		setGameState(ITEM_SAVE_STATE(enemy->special), 1);
	}
	
}




void enemyDoorClose(Enemy * enemy) 
{
	if (enemy->state1 == DOOR_CLOSED) 
	{
		return;
	}

	enemy->state1 = DOOR_CLOSED;
	initEnemyGfx(enemy, (u16*)doorGfxTiles, (characterAnimDef*) &enemyDoorCharAnim[DOOR_FRAME(enemy->itemDrop)]);
	if (enemy->itemDrop & DOOR_INVIS)
	{
		enemyDoorSetInvisPalette(enemy);
	}
	playGameSfx(SFX_DOOR);

}


int enemyDoorCollide(Enemy * enemy)
{
	//if the door hasn't appeared yet, collision does nothing
	if (enemy->state1 == DOOR_OPEN)
	{
		return 0;
	}

	//if the door is fake, collision does nothing
	if (enemy->special & DOOR_FAKE)
	{
		return 0;
	}
	
	//if they have the key, open the door
	int key = DOOR_KEY(enemy->itemDrop);
	if (mainCharInventory(key))
	{
		mainCharAddInventory(key, -1);
		enemyDoorOpen(enemy);
	}
	else
	{
		//else it acts like a wall
		//returnMainCharToLastGoodPosition(enemy->itemDrop);
		return ENEMY_COLLIDE_NO_DMG;
	}
	//either way, doesn't act like an enemy
	return 0;
}

int enemyDoorEnemyCollide(Enemy * enemy)
{
	if (enemy->state1 == DOOR_OPEN)
	{
		return 0;
	}

	return ENEMY_COLLIDE_NO_DMG;
	
}

int enemyDoorUpdate(Enemy * enemy)
{
	//if the door is doa, kill it now
	if (enemy->DEAD == true)
	{
		resetEnemy(enemy);
		return 1;
	}

	//check for late appearing doors
	if (enemy->state1 == DOOR_OPEN)
	{
		int ePos;
		int mcPos;
		if (IS_VERTICAL(DOOR_DIRECTION(enemy->itemDrop)))
		{
			ePos = enemy->status->x;
			mcPos = getMainCharX();
		}
		else
		{
			ePos = enemy->status->y;
			mcPos = getMainCharY();
		}

		if (enemy->special & DOOR_APPEARS_AFTER)
		{
			if (ePos - 8 > mcPos + 32)
			{
				enemyDoorClose(enemy);
			}
		}
		else if (enemy->special & DOOR_APPEARS_BEFORE)
		{
			if (ePos + 16 < mcPos - 8)
			{
				enemyDoorClose(enemy);
			}
		}
	}


	//check for auto-opening doors as enemy reward
	enemy->state3++;
	if (enemy->state3 < DELAY_BETWEEN_ENEMY_DEAD_CHECKS)
	{
		return 1;
	}
	if (enemy->special & ITEM_REWARD)
	{
		if (numEnemiesAlive() == 0)
		{
			enemyDoorOpen(enemy);
			enemy->DEAD = true;
			return 1;
		}
	}

	//check switch-operated doors
	int toggleNum = DOOR_GET_TOGGLE(enemy);
	if (toggleNum > 0) 
	{
		int toggleState = getToggleState(toggleNum);
		if (toggleState == DOOR_OPEN_ON_TOGGLE(enemy))
		{
			enemyDoorOpen(enemy);
		}
		else
		{
			enemyDoorClose(enemy);
		}

	}

	return 1;
}


int enemyDoorFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyDoorInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyDoorUpdate(enemy);
			break;
		case ENEMY_DEAD:
			resetEnemy(enemy);
			return 1;
			break;
		case ENEMY_BULLET_COLLIDE:
			return 0;
			break;
		case ENEMY_ENEMY_COLLIDE:
			return enemyDoorEnemyCollide(enemy);
			break;
		case ENEMY_MC_COLLIDE:
			return enemyDoorCollide(enemy);
			break;
	}
	return 1;
}


const EnemyType enemy_door_def = 
{
	1,				//hp
	0,				//collideDamage
	5,				//numSprites
	15,				//bbHeight
	32,				//bbWidth
	-16,			//bbOffx
	0,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyDoorFunc,	//updateFunction
	(short*)&doorGfxPal,
	null
};



