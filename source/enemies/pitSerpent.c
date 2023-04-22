#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "crockyGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "enemies/portraits/crocky_portrait.h"

#define Frame(x) (void *)&(enemyPitSerpentCharFrames[x])
#define Anim(x) (void *)&(enemyPitSerpentCharAnim[x])

#define SM_SQ_POS -8,-8

#define NECK(x,y,z) {(x),(y),3,SQUARE,SIZE_8,false,(z)}
#define BLANK(z) {0,0,35,SQUARE,SIZE_8,false,(z)}

#define PIT_SERPENT_ANIM(x) {9999,(u16*)crockyGfxTiles,Frame(x),Anim(x)}
#define PIT_SERPENT_HIDE_TIME 60
#define PIT_SERPENT_APPEAR_TIME 60
#define PIT_SERPENT_ATTACK_TIME 60
#define PIT_SERPENT_RETREAT_TIME 60
#define PIT_SERPENT_START_OFFSET ((PIT_SERPENT_APPEAR_TIME >> 1) + 20)

#define PIT_SERPENT_BULLET_START 72

#define PS_SHORT 16
#define PS_LONG 24
#define PS_HALF (PS_SHORT >> 1)

#define PIT_SERPENT_SPRITES 10

#define APPEAR_DELAY 18



const characterFrameDef enemyPitSerpentCharFrames[] = 
{
	[ENEMY_DIR_N] = {// head up
		-8,-32,
		4,
		TALL,
		SIZE_32,
		false,
		Frame(4),
	} ,
	[ENEMY_DIR_S] = {// head down
		-8,0,
		4,
		TALL,
		SIZE_32,
		FLIP_V_ONLY,
		Frame(13),
		
	} ,
	[ENEMY_DIR_E] = {// head right
		0, -8,
		6,
		WIDE,
		SIZE_32,
		false,
		Frame(22),
	},
	[ENEMY_DIR_W] = {// head left
		-32, -8,
		6,
		WIDE,
		SIZE_32,
		true,
		Frame(31),
	},
	//4
	NECK(-4,0,FRAME_NEXT),
	NECK(-4,5,FRAME_NEXT),
	NECK(-4,10,FRAME_NEXT),
	NECK(-4,15,FRAME_NEXT),
	NECK(-4,20,FRAME_NEXT),
	NECK(-4,25,FRAME_NEXT),
	NECK(-4,30,FRAME_NEXT),
	NECK(-4,35,FRAME_NEXT),
	NECK(-4,40,null),
	//13
	NECK(-4,-5,FRAME_NEXT),
	NECK(-4,-10,FRAME_NEXT),
	NECK(-4,-15,FRAME_NEXT),
	NECK(-4,-20,FRAME_NEXT),
	NECK(-4,-25,FRAME_NEXT),
	NECK(-4,-30,FRAME_NEXT),
	NECK(-4,-35,FRAME_NEXT),
	NECK(-4,-40,FRAME_NEXT),
	NECK(-4,-45,null),
	//22
	NECK(-5,-4,FRAME_NEXT),
	NECK(-10,-4,FRAME_NEXT),
	NECK(-15,-4,FRAME_NEXT),
	NECK(-20,-4,FRAME_NEXT),
	NECK(-25,-4,FRAME_NEXT),
	NECK(-30,-4,FRAME_NEXT),
	NECK(-35,-4,FRAME_NEXT),
	NECK(-40,-4,FRAME_NEXT),
	NECK(-45,-4,null),
	//31
	NECK(0,-4,FRAME_NEXT),
	NECK(5,-4,FRAME_NEXT),
	NECK(10,-4,FRAME_NEXT),
	NECK(15,-4,FRAME_NEXT),
	NECK(20,-4,FRAME_NEXT),
	NECK(25,-4,FRAME_NEXT),
	NECK(30,-4,FRAME_NEXT),
	NECK(35,-4,FRAME_NEXT),
	NECK(40,-4,null),
	//40
	BLANK(null),
	





};



//state1 = timer
//state2 = state
//state3 = facing

enum
{
	PIT_SERPENT_STATE_HIDING,
	PIT_SERPENT_STATE_APPEARING,
	PIT_SERPENT_STATE_ATTACKING,
	PIT_SERPENT_STATE_RETREATING,
};

const characterAnimDef enemyPitSerpentCharAnim[] = 
{

[ENEMY_DIR_N] =	PIT_SERPENT_ANIM(ENEMY_DIR_N),
[ENEMY_DIR_S] =	PIT_SERPENT_ANIM(ENEMY_DIR_S),
[ENEMY_DIR_E] =	PIT_SERPENT_ANIM(ENEMY_DIR_E),
[ENEMY_DIR_W] =	PIT_SERPENT_ANIM(ENEMY_DIR_W),
[ENEMY_DIR_NULL] =	PIT_SERPENT_ANIM(40),

};


int enemyPitSerpentInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)crockyGfxTiles, Anim(ENEMY_DIR_N));
	enemy->status->mobility = MOBILITY_GHOST;
	enemy->state1 = 9999;
	enemy->state2 = PIT_SERPENT_STATE_RETREATING;
	enemy->status->priority = CHAR_PRIORITY_MAP;
	int i;
	for (i = 0; i < PIT_SERPENT_SPRITES; i++)
	{
		setSpritePriority(enemy->status->spriteList[i], SPRITE_PRIORITY_BG);
	
	}
	return 1;
}

void enemyPitSerpentShootBullet(Enemy * enemy, int speedX, int speedY)
{
	//new bullet!
	if ((speedX == 0) && (speedY == 0))
	{
		return;
	}

	int orgX = enemy->status->x;
	int orgY = enemy->status->y;

	switch (enemy->state3)
	{
		case ENEMY_DIR_N:
			orgY -= PS_LONG;
			orgX -= 8;
			break;
		case ENEMY_DIR_S:
			orgY += PS_LONG;
			orgX -= 8;
			break;
		case ENEMY_DIR_E:
			orgX += PS_LONG;
			orgY -= 8;
			break;
		case ENEMY_DIR_W:
			orgX -= PS_LONG;
			orgY -= 8;
			break;
	}


	BulletDef bullet;
	bullet.team = TEAM_ENEMY;
	bullet.tile = findActualTileFromEnemyTile(enemy, PIT_SERPENT_BULLET_START);
	bullet.ttl = 9999;
	bullet.xSpeed = unitVal(speedX);
	bullet.ySpeed = unitVal(speedY);
	bullet.xFrameskip = abs(speedX);
	bullet.yFrameskip = abs(speedY);
	bullet.pierce = false;
	bullet.power = 3;
	bullet.x = orgX;
	bullet.y = orgY;
	bullet.width = 16;
	bullet.height = 16;
	bullet.sourceX = orgX;
	bullet.sourceY = orgY;
	bullet.sprite = null;

	if (bullet.ySpeed == 0)
	{
		bullet.tile -= 2;
	}


	BulletDef * createdBullet;
	createdBullet = newBullet((BulletDef*)&bullet);
	if (createdBullet != null)
	{
		setSpriteSize(createdBullet->sprite, SIZE_16);
		setSpritePalette(createdBullet->sprite, findPalette((u16*)pitSerpentPal));
		if (bullet.xSpeed < 0)
		{
			setSpriteFlip(createdBullet->sprite, true);
		}
		if (bullet.ySpeed > 0)
		{
			setSpriteVFlip(createdBullet->sprite, true);
		}
	}
}

			
void enemyPitSerpentUpdateHiding(Enemy * enemy)
{
	if (enemyStateAdvance(enemy, PIT_SERPENT_HIDE_TIME, PIT_SERPENT_STATE_APPEARING))
	{
		//select new facing direction
		enemy->state3 = dice(0,3);
		int offset = dice(0,40) - 20;
		enemy->status->x = T(7);
		enemy->status->y = T(5);
		setCharacterAnimation(enemy->status, Anim(ENEMY_DIR_NULL));
		switch (enemy->state3)
		{
			case ENEMY_DIR_N:
				setBoundingBlock(enemy->status, PS_SHORT, PS_LONG, -PS_HALF, -PS_LONG);
				moveEnemyFast(enemy, offset, PIT_SERPENT_START_OFFSET);
				break;
			case ENEMY_DIR_S:
				setBoundingBlock(enemy->status, PS_SHORT, PS_LONG, -PS_HALF, 0);
				moveEnemyFast(enemy, offset, -PIT_SERPENT_START_OFFSET);
				break;
			case ENEMY_DIR_E:
				setBoundingBlock(enemy->status, PS_LONG, PS_SHORT, 0, -PS_HALF);
				moveEnemyFast(enemy, -PIT_SERPENT_START_OFFSET, offset);
				break;
			case ENEMY_DIR_W:
				setBoundingBlock(enemy->status, PS_LONG, PS_SHORT, -PS_LONG, -PS_HALF);
				moveEnemyFast(enemy, PIT_SERPENT_START_OFFSET, offset);
				break;
	
		}
	
	}
}


void enemyPitSerpentShoot(Enemy * enemy)
{
	switch (enemy->state3)
	{
		case ENEMY_DIR_N:
			enemyPitSerpentShootBullet(enemy, -2,-2);
			enemyPitSerpentShootBullet(enemy, 0,-1);
			enemyPitSerpentShootBullet(enemy, 2,-2);
			break;
		case ENEMY_DIR_S:
			enemyPitSerpentShootBullet(enemy, -2,2);
			enemyPitSerpentShootBullet(enemy, 0,1);
			enemyPitSerpentShootBullet(enemy, 2,2);
			break;
		case ENEMY_DIR_E:
			enemyPitSerpentShootBullet(enemy, 2,-2);
			enemyPitSerpentShootBullet(enemy, 1,0);
			enemyPitSerpentShootBullet(enemy, 2,2);
			break;
		case ENEMY_DIR_W:
			enemyPitSerpentShootBullet(enemy, -2,-2);
			enemyPitSerpentShootBullet(enemy, -1,0);
			enemyPitSerpentShootBullet(enemy, -2,2);
			break;
	}
}


void enemyPitSerpentUpdateAppearing(Enemy * enemy)
{
	if(enemyStateAdvance(enemy, PIT_SERPENT_APPEAR_TIME, PIT_SERPENT_STATE_ATTACKING))
	{
		enemyPitSerpentShoot(enemy);
	
	}

	switch (enemy->state3)
	{
		case ENEMY_DIR_N:
			moveEnemy(enemy, 0,-1);
			break;
		case ENEMY_DIR_S:
			moveEnemy(enemy, 0,1);
			break;
		case ENEMY_DIR_E:
			moveEnemy(enemy, 1,0);
			break;
		case ENEMY_DIR_W:
			moveEnemy(enemy, -1,0);
			break;
	
	}

	int i;
	Sprite ** segment = enemy->status->spriteList;
	for (i = 0; i < PIT_SERPENT_SPRITES; i++)
	{
		if (enemy->state1 > (i*6) + APPEAR_DELAY)
		{
			setSpritePriority(segment[i], SPRITE_PRIORITY_NORMAL);

			
		}
	
	}

	if (enemy->state1 == 8)
	{

		setCharacterAnimation(enemy->status, Anim(enemy->state3));
	}

	if (enemy->state1 == APPEAR_DELAY + 55)
	{
		switch (enemy->state3)
		{
			case ENEMY_DIR_N:
				setBoundingBlock(enemy->status, PS_SHORT, PS_LONG, -PS_HALF, -PS_LONG);
				break;
			case ENEMY_DIR_S:
				setBoundingBlock(enemy->status, PS_SHORT, PS_LONG, -PS_HALF, 0);
				break;
			case ENEMY_DIR_E:
				setBoundingBlock(enemy->status, PS_LONG, PS_SHORT, 0, -PS_HALF);
				break;
			case ENEMY_DIR_W:
				setBoundingBlock(enemy->status, PS_LONG, PS_SHORT, -PS_LONG, -PS_HALF);
				break;
	
		}
	}


}

void enemyPitSerpentUpdateAttacking(Enemy * enemy)
{
	enemyStateAdvance(enemy, PIT_SERPENT_ATTACK_TIME, PIT_SERPENT_STATE_RETREATING);

}

void enemyPitSerpentUpdateRetreating(Enemy * enemy)
{
	enemyStateAdvance(enemy, PIT_SERPENT_RETREAT_TIME, PIT_SERPENT_STATE_HIDING);
	switch (enemy->state3)
	{
		case ENEMY_DIR_S:
			moveEnemy(enemy, 0,-1);
			break;
		case ENEMY_DIR_N:
			moveEnemy(enemy, 0,1);
			break;
		case ENEMY_DIR_W:
			moveEnemy(enemy, 1,0);
			break;
		case ENEMY_DIR_E:
			moveEnemy(enemy, -1,0);
			break;
		
	}

	int i;
	Sprite ** segment = enemy->status->spriteList;
	for (i = 0; i < PIT_SERPENT_SPRITES; i++)
	{
		if (enemy->state1 > (i*6) - APPEAR_DELAY)
		{
			setSpritePriority(segment[9 - i], SPRITE_PRIORITY_BG);
			if (i > 5)
			{
				setBoundingBlock(enemy->status, 0, 0, 1000, 1000);
			}
		}
	
	}



}
	


int enemyPitSerpentUpdate(Enemy * enemy)
{
	enemy->state1++;

	switch(enemy->state2)
	{
		case PIT_SERPENT_STATE_HIDING:
			enemyPitSerpentUpdateHiding(enemy);
			break;
		case PIT_SERPENT_STATE_APPEARING:
			enemyPitSerpentUpdateAppearing(enemy);
			break;
		case PIT_SERPENT_STATE_ATTACKING:
			enemyPitSerpentUpdateAttacking(enemy);
			break;
		case PIT_SERPENT_STATE_RETREATING:
			enemyPitSerpentUpdateRetreating(enemy);
			break;
	
	}


	return 1;
}

int enemyPitSerpentFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyPitSerpentInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyPitSerpentUpdate(enemy);
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
		case ENEMY_BULLET_COLLIDE:
			return ENEMY_HIT_NOBOUNCE;
			break;
	}
	return 1;
}


const EnemyDatabaseDef enemy_pit_serpent_db = 
{
	crocky_portraitTiles,
	crocky_portraitPal + 16,
	crocky_portraitMap,
	"Pit Serpent",
	"Snake from the depths",
	"Comes out of","pits and",
	        "breathes fire",
	9,9,9

};
	

const EnemyType enemy_pit_serpent_def = 
{
	20,				//hp
	2,				//collideDamage
	PIT_SERPENT_SPRITES,	//numSprites
	0,				//bbHeight
	0,				//bbWidth
	1000,				//bbOffx
	1000,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyPitSerpentFunc,	//updateFunction
	(short*)&pitSerpentPal,
	&enemy_pit_serpent_db
};

	
