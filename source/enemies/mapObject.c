#include "../enemies.h"
#include "../characters.h"
#include "../spriteMgr.h"
#include "../level.h"
#include "../text.h"
#include "../mainChar.h"
#include "mapObjectGfx.h"
#include "deadTreeGfx.h"

#define Frame(x) (void *)&(enemyMapObjectCharFrames[x])
#define Anim(x) (void *)&(enemyMapObjectCharAnim[x])

#define OFFSET 0

#define objFrameDef(x) {0,-OFFSET,(x),SQUARE,SIZE_16,false,null}
#define objFrameDefTall(x) {0,0,(x),TALL,SIZE_32,false,null}
#define objFrameDefBig(x) {0,0,(x),SQUARE,SIZE_32,false,null}
#define objAnimDef(x) {99999,(u16*)map_objectGfxTiles,Frame(x),null}

#define objTreeCutFrame(x) {0,-16,(x),SQUARE,SIZE_32,false,null}
#define objTreeTopFrame(x) {-24,-32,(x),WIDE,SIZE_64,false,Frame(OBJ_STUMP)}
#define objTreeAnimDef(x) {99999,(u16*)deadTreeGfxTiles,Frame(x),null}

#define CHEST_STUCK_X_R -10
#define CHEST_STUCK_X_L 5 
#define CHEST_STUCK_Y_B -10
#define CHEST_STUCK_Y_T  4 

const characterFrameDef enemyMapObjectCharFrames[] = 
{
	[OBJ_SKEL] = objFrameDefTall(0),
	[OBJ_SKULL1] = objFrameDef(2),
	[OBJ_SKULL2] = objFrameDef(4),
	[OBJ_POT_DUMP1] = objFrameDef(6),
	[OBJ_POT_DUMP2] = objFrameDef(8),
	[OBJ_CHEST] = objFrameDef(10),
	[OBJ_POT1] = objFrameDef(12),
	[OBJ_POT2] = objFrameDef(14),
	[OBJ_POT3] = objFrameDef(16),
	[OBJ_CRATE1] = objFrameDef(18),
	[OBJ_CRATE2] = objFrameDef(20),
	[OBJ_CRATE3] = objFrameDef(22),
	[OBJ_BONES1] = objFrameDef(68),
	[OBJ_BONES2] = objFrameDef(70),
	[OBJ_BONES3] = objFrameDef(72),
	[OBJ_BONES4] = objFrameDef(74),
	[OBJ_BONES5] = objFrameDef(66),
	[OBJ_BUSH] = objFrameDef(82),
	[OBJ_STAIRS_DOWN] = objFrameDefBig(24),
	[OBJ_STAIRS_UP] = objFrameDefBig(28),
	[OBJ_CACTUS] = objFrameDef(84),
	[OBJ_TREE_DEAD_1] = objTreeTopFrame(0),
	[OBJ_TREE_DEAD_2] = objTreeTopFrame(7),
	[OBJ_TREE_DEAD_3] = objTreeTopFrame(14),
	[OBJ_TREE_DEAD_4] = objTreeCutFrame(28),
	[OBJ_STUMP] = objFrameDef(88),

};

const characterAnimDef enemyMapObjectCharAnim[] = 
{
	[OBJ_SKEL] =        objAnimDef(OBJ_SKEL),
	[OBJ_SKULL1] =      objAnimDef(OBJ_SKULL1),
	[OBJ_SKULL2] =      objAnimDef(OBJ_SKULL2),
	[OBJ_POT_DUMP1] =   objAnimDef(OBJ_POT_DUMP1),
	[OBJ_POT_DUMP2] =   objAnimDef(OBJ_POT_DUMP2),
	[OBJ_CHEST] =       objAnimDef(OBJ_CHEST),
	[OBJ_POT1] =        objAnimDef(OBJ_POT1),
	[OBJ_POT2] =        objAnimDef(OBJ_POT2),
	[OBJ_POT3] =        objAnimDef(OBJ_POT3),
	[OBJ_CRATE1] =      objAnimDef(OBJ_CRATE1),
	[OBJ_CRATE2] =      objAnimDef(OBJ_CRATE2),
	[OBJ_CRATE3] =      objAnimDef(OBJ_CRATE3),
	[OBJ_BONES1] =      objAnimDef(OBJ_BONES1),
	[OBJ_BONES2] =      objAnimDef(OBJ_BONES2),
	[OBJ_BONES3] =      objAnimDef(OBJ_BONES3),
	[OBJ_BONES4] =      objAnimDef(OBJ_BONES4),
	[OBJ_BONES5] =      objAnimDef(OBJ_BONES5),
	[OBJ_BUSH] =        objAnimDef(OBJ_BUSH),
	[OBJ_STAIRS_UP] =   objAnimDef(OBJ_STAIRS_UP),
	[OBJ_STAIRS_DOWN] = objAnimDef(OBJ_STAIRS_DOWN),
	[OBJ_CACTUS] =      objAnimDef(OBJ_CACTUS),
	[OBJ_TREE_DEAD_1] =	objTreeAnimDef(OBJ_TREE_DEAD_1),
	[OBJ_TREE_DEAD_2] =	objTreeAnimDef(OBJ_TREE_DEAD_2),
	[OBJ_TREE_DEAD_3] =	objTreeAnimDef(OBJ_TREE_DEAD_3),
	[OBJ_TREE_DEAD_4] =	objTreeAnimDef(OBJ_TREE_DEAD_4),
	[OBJ_STUMP] = 		objTreeAnimDef(OBJ_TREE_DEAD_4),
	
};

void enemyMapObjectSetPal(Enemy * enemy, u16 * map_objectPal)
{
	PaletteId specialPal = loadPalette(map_objectPal);
	Sprite * obj = enemy->status->spriteList[0];
	setSpritePalette(obj, specialPal);
	if (enemy->status->listSize > 1)
	{
		Sprite * obj2 = enemy->status->spriteList[1];
		setSpritePalette(obj2, specialPal);
	}
}


int enemyMapObjectInit(Enemy * enemy)
{
	switch(enemy->special)
	{
		case OBJ_SKEL:
		case OBJ_SKULL1:
		case OBJ_SKULL2:
		case OBJ_BONES1:
		case OBJ_BONES2:
		case OBJ_BONES3:
		case OBJ_BONES4:
		case OBJ_BONES5:
		case OBJ_STAIRS_DOWN:
		case OBJ_STAIRS_UP:
			enemy->state1 = OBJ_MAP;
			break;
		case OBJ_CRATE1:
		case OBJ_CRATE2:
		case OBJ_CRATE3:
		case OBJ_CHEST:
			enemy->state1 = OBJ_PUSH;
			break;
		case OBJ_TREE_DEAD_1:
		case OBJ_TREE_DEAD_2:
		case OBJ_TREE_DEAD_3:
		case OBJ_TREE_DEAD_4:
			enemy->state1 = OBJ_OBSTACLE;
			break;
		
		default:
			enemy->state1 = OBJ_BREAK;
	}

	if ((enemy->special >= OBJ_TREE_DEAD_1) && (enemy->special <= OBJ_TREE_DEAD_4))
	{
		initEnemyGfx(enemy, (u16*)deadTreeGfxTiles, (characterAnimDef * )&enemyMapObjectCharAnim[enemy->special]);
		enemyMapObjectSetPal(enemy, (u16*)deadTreeGfxPal);
	}
	else
	{
		initEnemyGfx(enemy, (u16*)map_objectGfxTiles, (characterAnimDef * )&enemyMapObjectCharAnim[enemy->special]);
	}
	if (enemy->state1 == OBJ_MAP)
	{
		enemy->status->priority = CHAR_PRIORITY_MAP;
	}
	else
	{
		enemy->status->priority = CHAR_PRIORITY_NORMAL;
		enemy->status->y += OFFSET;
	
	}

	if (enemy->special == OBJ_BUSH)
	{
		enemyMapObjectSetPal(enemy, (u16*)map_objectBushPal);
	}

	if (enemy->special == OBJ_CACTUS)
	{
		enemyMapObjectSetPal(enemy, (u16*)map_objectCactusPal);
	}


	if ((enemy->special == OBJ_STAIRS_DOWN) ||
		(enemy->special == OBJ_STAIRS_UP))
	{
		enemyMapObjectSetPal(enemy, (u16*)map_objectDoorPal);
	}

	return 0;
}

int enemyMapObjectUpdate(Enemy * enemy)
{
	if (enemy->state2 != 0)
	{
		enemy->state2 -= unitVal(enemy->state3);
	}

	if (enemy->state3 != 0)
	{
		enemy->state3 -= unitVal(enemy->state3);
	}

	return 0;
}

int enemyMapObjectBulletCollide(Enemy * enemy)
{
	if (enemy->state1 == OBJ_BREAK)
	{
		return BULLET_HIT_DMG;
	}
	if (enemy->state1 == OBJ_PUSH)
	{
		return BULLET_HIT_ABSORB;
	}
	return 0;
}

bool enemyMapObjectCanPush(Enemy * enemy)
{
	CharacterStatus * mc = getMainCharStatus();
	int dx = enemy->status->x - mc->x;
	int dy = enemy->status->y - mc->y;

	//if move horizontal
	if(abs(dx) > abs(dy))
	{
		if (  ((dx < 0) && (dx > CHEST_STUCK_X_R)) ||
		      ((dx > 0) && (dx < CHEST_STUCK_X_L)) )
		{
			enemy->state1 = OBJ_BREAK;
		}

		int dirX = unitVal(dx);
		//if the enemy can move, no collision
		if (moveEnemyCanBlock(enemy, dirX, 0, true))
		{
			enemy->state2 = unitVal(dx) * 2;
			return true;
		}
		return false;
	}
	else  //else move vertical
	{
		if (  ((dy < 0) && (dy > CHEST_STUCK_Y_B)) ||
		      ((dy > 0) && (dy < CHEST_STUCK_Y_T))  )
		{
			enemy->state1 = OBJ_BREAK;
		}
		int dirY = unitVal(dy);
		//if the enemy can move, no collision
		if (moveEnemyCanBlock(enemy, 0, dirY, true))
		{
			enemy->state3 = unitVal(dy) * 2;
			return true;
		}
		return false;
	}
	
}

int enemyMapObjectCollide(Enemy * enemy)
{
	//if it's a map level object, no collision
	if (enemy->state1 == OBJ_MAP)
	{
		return 0;
	}
	
	//if it's a push object, we'll try to move it
	if (enemy->state1 == OBJ_PUSH)
	{
		if(enemyMapObjectCanPush(enemy))
		{
			return 0;
		}
	}

	//otherwise, a no-damage collision

	return ENEMY_COLLIDE_NO_DMG;
}

int enemyMapObjectEnemyCollide(Enemy * enemy)
{
	//if it's a map level object, no collision
	if (enemy->state1 == OBJ_MAP)
	{
		return NO_COLLISION;
	}
	
	//otherwise, a no-damage collision
	return ENEMY_COLLIDE_NO_DMG;
}


int enemyMapObjectDead(Enemy * enemy)
{
	if (enemy->state1 == OBJ_BREAK)
	{
		enemy->status->y -= 4;
		return killEnemy(enemy);
	}
	resetEnemy(enemy);
	return 1;
	
}


int enemyMapObjectFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyMapObjectInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyMapObjectUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return enemyMapObjectDead(enemy);
			resetEnemy(enemy);
			return 1;
			break;
		case ENEMY_BULLET_COLLIDE:
			return enemyMapObjectBulletCollide(enemy);
			break;
		case ENEMY_MC_COLLIDE:
			return enemyMapObjectCollide(enemy);
			break;
		case ENEMY_ENEMY_COLLIDE:
			return enemyMapObjectEnemyCollide(enemy);
			break;
	}
	return 1;
}

//const EnemyDatabaseDef map_object_db = 
//{
	//null,
	//null,
	//null,
	//"map object"
//};

const EnemyType enemy_map_object_def = 
{
	1,				//hp
	0,				//collideDamage
	1,				//numSprites
	14 - OFFSET,	//bbHeight
	14,				//bbWidth
	1,				//bbOffx
	1,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyMapObjectFunc,	//updateFunction
	(short*)&map_objectGfxPal,
	//&map_object_db
	null
};

const EnemyType enemy_bigmap_object_def = 
{
	1,				//hp
	0,				//collideDamage
	2,				//numSprites
	14 - OFFSET,	//bbHeight
	14,				//bbWidth
	1,				//bbOffx
	1,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyMapObjectFunc,	//updateFunction
	(short*)&map_objectGfxPal,
	//&map_object_db
	null
};

