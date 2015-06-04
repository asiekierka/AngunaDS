#include "../enemies.h"
#include "../characters.h"
#include "../spriteMgr.h"
#include "../level.h"
#include "../text.h"
#include "../mainChar.h"
#include "../bullet.h"
#include "graveyardObject.h"
#include "graveyardObjectGfx.h"

#define Frame(x) (void *)&(enemyGraveyardObjectCharFrames[x])
#define Anim(x) (void *)&(enemyGraveyardObjectCharAnim[x])


#define objGraveDefTall(x) {0,-16,(x),TALL,SIZE_32,false,null}
#define objAnimDef(x) {99999,(u16*)graveyardObjectGfxTiles,Frame(x),null}

typedef struct 
{
	int xOffset;
	int yOffset;
	int width;
	int height;
	bool dynamite;

} GraveyardObjectDef;

#define CAN_DYNAMITE state1

#define GY_SHAPE_STANDARD_16(d)    {2,2,12,12,(d)}
#define GY_SHAPE_WIDE_32(d)        {0,0,32,16,(d)}
#define GY_SHAPE_WIDE_64(d)        {0,0,64,16,(d)}
#define GY_SHAPE_NARROW_16_L(d)    {0,0,8,16,(d)}
#define GY_SHAPE_NARROW_16_R(d)    {8,0,8,16,(d)}
#define GY_SHAPE_SQUARE_24(d)      {0,0,24,24,(d)}
#define GY_SHAPE_THORNY_GRAVE(d)   {0,0,24,16,(d)}

const GraveyardObjectDef enemyGraveyardObjectDefs[] = 
{
	[GY_FENCE]       = GY_SHAPE_STANDARD_16(false),
	[GY_FENCE_2]     = GY_SHAPE_WIDE_32(false),
	[GY_FENCE_CAP_R] = GY_SHAPE_STANDARD_16(false),
	[GY_FENCE_CAP_L] = GY_SHAPE_STANDARD_16(false),
	[GY_FENCE_VERT_R]= GY_SHAPE_NARROW_16_R(false),
	[GY_FENCE_VERT_L]= GY_SHAPE_NARROW_16_L(false),
	[GY_GRAVE_1]     = GY_SHAPE_STANDARD_16(true),
	[GY_GRAVE_2]     = GY_SHAPE_STANDARD_16(true),
	[GY_GRAVE_3]     = GY_SHAPE_STANDARD_16(true),
	[GY_GRAVE_4]     = GY_SHAPE_STANDARD_16(true),
	[GY_GRAVE_5]     = GY_SHAPE_STANDARD_16(true),
	[GY_BIG_GRAVE]   = GY_SHAPE_SQUARE_24(true),
	[GY_BIG_GRAVE_B] = GY_SHAPE_SQUARE_24(true),
	[GY_THORNY_GRAVE]= GY_SHAPE_THORNY_GRAVE(true),
	[GY_STATUE]      = {0,0,16,32,true},
	[GY_STATUE_B]    = GY_SHAPE_STANDARD_16(true),
};

const characterFrameDef enemyGraveyardObjectCharFrames[] = 
{
	[GY_FENCE]        = {FRAME_SMALL_SQUARE(0,false)},
	[GY_FENCE_2]      = {FRAME_SMALL_SQUARE_N(0,false,Frame(GY_FENCE))},
	[GY_FENCE_CAP_R]  = {FRAME_SMALL_SQUARE(2,false)},
	[GY_FENCE_CAP_L]  = {FRAME_SMALL_SQUARE(2,true)},
	[GY_FENCE_VERT_R] = {FRAME_SMALL_SQUARE(64,true)},
	[GY_FENCE_VERT_L] = {FRAME_SMALL_SQUARE(64,false)},
	[GY_GRAVE_1]      = {FRAME_SMALL_SQUARE(66,false)},
	[GY_GRAVE_2]      = objGraveDefTall(4),
	[GY_GRAVE_3]      = objGraveDefTall(6),
	[GY_GRAVE_4]      = {FRAME_SMALL_SQUARE(76,false)},
	[GY_GRAVE_5]      = {FRAME_SMALL_SQUARE(78,false)},
	[GY_BIG_GRAVE]    = {0,-16,8,SQUARE,SIZE_32,false,Frame(GY_BIG_GRAVE_B)},
	[GY_BIG_GRAVE_B]  = {0, 16,12,WIDE,SIZE_32,false,null},
	[GY_THORNY_GRAVE] = {0,-16,16,SQUARE,SIZE_32,false,null},
	[GY_STATUE]       = {0,-16,20,SQUARE,SIZE_32,false,Frame(GY_STATUE_B)},
	[GY_STATUE_B]     = {0,16,24,WIDE,SIZE_32,false,null},

};

const characterAnimDef enemyGraveyardObjectCharAnim[] = 
{
	[GY_FENCE] 			= objAnimDef(GY_FENCE),
	[GY_FENCE_2] 		= objAnimDef(GY_FENCE_2),
	[GY_FENCE_CAP_R]    = objAnimDef(GY_FENCE_CAP_R),
	[GY_FENCE_CAP_L]    = objAnimDef(GY_FENCE_CAP_L),
	[GY_FENCE_VERT_R] 	= objAnimDef(GY_FENCE_VERT_R),
	[GY_FENCE_VERT_L] 	= objAnimDef(GY_FENCE_VERT_L),
	[GY_GRAVE_1] 		= objAnimDef(GY_GRAVE_1),
	[GY_GRAVE_2] 		= objAnimDef(GY_GRAVE_2),
	[GY_GRAVE_3] 		= objAnimDef(GY_GRAVE_3),
	[GY_GRAVE_4] 		= objAnimDef(GY_GRAVE_4),
	[GY_GRAVE_5] 		= objAnimDef(GY_GRAVE_5),
	[GY_BIG_GRAVE] 		= objAnimDef(GY_BIG_GRAVE),
	[GY_BIG_GRAVE_B] 	= objAnimDef(GY_BIG_GRAVE_B),
	[GY_THORNY_GRAVE] 	= objAnimDef(GY_THORNY_GRAVE),
	[GY_STATUE] 		= objAnimDef(GY_STATUE),
	[GY_STATUE_B] 		= objAnimDef(GY_STATUE_B),

	
};



int enemyGraveyardObjectInit(Enemy * enemy)
{

	const GraveyardObjectDef * objectDef = &enemyGraveyardObjectDefs[enemy->special];
	enemy->status->boundingBlockOffsetX = objectDef->xOffset;
	enemy->status->boundingBlockOffsetY = objectDef->yOffset;
	enemy->status->boundingBlockWidth = objectDef->width;
	enemy->status->boundingBlockHeight = objectDef->height;
	enemy->CAN_DYNAMITE = objectDef->dynamite;

	initEnemyGfx(enemy, (u16*)graveyardObjectGfxTiles, (characterAnimDef * )&enemyGraveyardObjectCharAnim[enemy->special]);
	enemy->status->priority = CHAR_PRIORITY_NORMAL;

	return 0;
}

int enemyGraveyardObjectUpdate(Enemy * enemy)
{
	return 0;
}

int enemyGraveyardObjectBulletCollide(Enemy * enemy)
{
	return BULLET_HIT_ABSORB;
}

int enemyGraveyardObjectCollide(Enemy * enemy)
{
	return ENEMY_COLLIDE_NO_DMG;
}

int enemyGraveyardObjectEnemyCollide(Enemy * enemy)
{
	return ENEMY_COLLIDE_NO_DMG;
}


int enemyGraveyardObjectDead(Enemy * enemy)
{
	killEnemy(enemy);
	return 1;
}


int enemyGraveyardObjectFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyGraveyardObjectInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyGraveyardObjectUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return enemyGraveyardObjectDead(enemy);
			return 1;
			break;
		case ENEMY_BULLET_COLLIDE:
			return enemyGraveyardObjectBulletCollide(enemy);
			break;
		case ENEMY_MC_COLLIDE:
			return enemyGraveyardObjectCollide(enemy);
			break;
		case ENEMY_ENEMY_COLLIDE:
			return enemyGraveyardObjectEnemyCollide(enemy);
			break;
		case ENEMY_DYNAMITED:
			return enemy->hp = 0;
			break;
	}
	return 1;
}


const EnemyType enemy_graveyard_object_def = 
{
	1,				//hp
	0,				//collideDamage
	2,				//numSprites
	1,				//bbHeight -- gets reset by def
	1,				//bbWidth -- gets reset by def
	1,				//bbOffx 
	1,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyGraveyardObjectFunc,	//updateFunction
	(short*)&graveyardObjectGfxPal,
	//&map_object_db
	null
};

