#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "../misc.h"
#include "../characters.h"
#include "../item.h"
#include "../items.h"
#include "../mainChar.h"
#include "../sound.h"
#include "../gameState.h"
#include "../textData.h"
#include "itemGfx.h"
#include "doorGfx.h"

extern const int mainCharGfxData[];

#define Frame(x) (void *)&(enemyItemCharFrames[x])
#define Anim(x) (void *)&(enemyItemCharAnim[x])

#define itemFrameDef(x) {-8,-8,(x),SQUARE,SIZE_16,false,null}
#define itemAnimDef(x) {99999,(u16*)itemsTiles,Frame(x),null}
#define itemAnimDefKey(x) {99999,(u16*)doorGfxTiles,Frame(x),null}

const characterFrameDef enemyItemCharFrames[] =
{
	////FRAME: frame 0 (nothing-blank)
	{//0
		0,0,		//x,y offset
		28,		//tile
		SQUARE,		//shape (SQUARE/TALL/WIDE)
		SIZE_8,		//size
		false, 		//flip
		null	//nextSprite
	},
	itemFrameDef(0),   ////FRAME: frame 1 (red key)
	itemFrameDef(2),   ////FRAME: frame 2 (yello key)
	itemFrameDef(4),   ////FRAME: frame 3 (blue key)
	itemFrameDef(6),   ////FRAME: frame 4 (meat)
	itemFrameDef(8),   ////FRAME: frame 5 (attack up)
	itemFrameDef(10),   ////FRAME: frame 6 (hp up)
	itemFrameDef(12),   ////FRAME: frame 7 (armor up)
	itemFrameDef(14),   ////FRAME: frame 8 (bow)
	itemFrameDef(16),   ////FRAME: frame 9 (arrows)
	itemFrameDef(18),   ////FRAME: frame 10(lantern)
	itemFrameDef(20),   ////FRAME: frame 11(potion)
	itemFrameDef(22),   ////FRAME: frame 12(boots)
	itemFrameDef(24),   ////FRAME: frame 13(caltrops)
	itemFrameDef(26),   ////FRAME: frame 14(gold)
	itemFrameDef(67),   ////FRAME: frame 15(attack ring)
	itemFrameDef(69),   ////FRAME: frame 16(dynamite)
	itemFrameDef(79),   ////FRAME: frame 17(gold bag)
	itemFrameDef(79),   ////FRAME: frame 18(small bag)
};

const characterAnimDef enemyItemCharAnim[] = 
{
	{//0 - nothing (invisible)
		99999,
		PUN(u16*)mainCharGfxData,
		Frame(0),
		null
	},
	itemAnimDefKey(1),
	itemAnimDefKey(2),
	itemAnimDefKey(3),
	itemAnimDef(4),
	itemAnimDef(5),
	itemAnimDef(6),
	itemAnimDef(7),
	itemAnimDef(8),
	itemAnimDef(9),
	itemAnimDef(10),
	itemAnimDef(11),
	itemAnimDef(12),
	itemAnimDef(13),
	itemAnimDef(14),
	itemAnimDef(15),
	itemAnimDef(16),
	itemAnimDef(17),
	itemAnimDef(17),


};

void enemyItemSetKeyPal(Enemy * enemy)
{
	Sprite * key = enemy->status->spriteList[0];
	PaletteId lockPal = loadPalette((u16*)lockGfxPal);
	setSpritePalette(key, lockPal);
}

int enemyItemInit(Enemy * enemy)
{
	if (ITEM_SAVES_STATE(enemy->special) && (getGameState(ITEM_SAVE_STATE(enemy->special))))
	{
		enemy->itemDrop = ITEM_NONE;
	}
	
	enemy->state4 = 0;

	
	//crucial difference - if it's a reward, we need to draw it as blank for now, but preserve the itemDrop
	if (enemy->special & ITEM_REWARD)
	{
		initEnemyGfx(enemy, (u16*)itemsTiles, (characterAnimDef*) &enemyItemCharAnim[ITEM_NONE]);
		//use state3 to signal that it's in "not appeared" state
		enemy->state3 = 1;
		return 1;
	}
	initEnemyGfx(enemy, (u16*)itemsTiles, (characterAnimDef*) &enemyItemCharAnim[enemy->itemDrop]);

	//if the item is a key, use the custom door palette
	if (ITEM_IS_KEY(enemy->itemDrop))
	{
		enemyItemSetKeyPal(enemy);
	}

	return 1;
}

int enemyItemCollide(Enemy * enemy)
{
	//make sure it's not hidden for reward, or already picked up from a saved gamestate
	if ((enemy->itemDrop != ITEM_NONE) && (enemy->state3 != 1))
	{
		//mainCharAddInventory(enemy->itemDrop, 1);
		pickupItem(enemy->itemDrop);
		playGameSfx(SFX_TAKE_ITEM);
		char * msg = (char*)ItemText[enemy->itemDrop];
		if (msg != null) 
		{
			setStatusMessage(msg);
		}
		if (ITEM_SAVES_STATE(enemy->special))
		{
			setGameState(ITEM_SAVE_STATE(enemy->special), 1);
		}
		resetEnemy(enemy);
	}
	return 0;
}

int enemyItemUpdate(Enemy * enemy)
{
	enemy->state4++;
	if (enemy->state4 < 15)
	{
		return 1;
	}
	enemy->state4 = 0;
	//if it's a reward item
	if (enemy->special & ITEM_REWARD)
	{
		//and the enemies are all dead
		if (numEnemiesAlive() == 0)
		{
			//and it's not saved as being already picked up....
			if(!(ITEM_SAVES_STATE(enemy->special) && (getGameState(ITEM_SAVE_STATE(enemy->special)))))
			{
				setCharacterAnimation(enemy->status, (characterAnimDef*)(&enemyItemCharAnim[enemy->itemDrop]));
				enemy->state3 = 0;
				if (ITEM_IS_KEY(enemy->itemDrop))
				{
					enemyItemSetKeyPal(enemy);
				}
			}
		}
	}
	
	return 1;
}


int enemyItemFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyItemInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyItemUpdate(enemy);
			break;
		case ENEMY_DEAD:
			resetEnemy(enemy);
			return 1;
			break;
		case ENEMY_BULLET_COLLIDE:
			return 0;
			break;
		case ENEMY_MC_COLLIDE:
			return enemyItemCollide(enemy);
		case ENEMY_ENEMY_COLLIDE:
			return NO_COLLISION;
		break;
	}
	return 1;
}


const EnemyType enemy_item_def = 
{
	1,				//hp
	0,				//collideDamage
	1,				//numSprites
	14,				//bbHeight
	14,				//bbWidth
	-7,				//bbOffx
	-7,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyItemFunc,	//updateFunction
	(short*)&itemsPal,
	null
};



