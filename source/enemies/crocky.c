#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "crockyGfx.h"
#include "../misc.h"
#include "../characters.h"
#include "../mainChar.h"
#include "../bullet.h"
#include "enemies/portraits/crocky_portrait.h"

#define Frame(x) (void *)&(enemyCrockyCharFrames[x])
#define Anim(x) (void *)&(enemyCrockyCharAnim[x])

#define TILES (u16*)crockyGfxTiles

#define CROCKY_BULLET_START 72


const characterFrameDef enemyCrockyCharFrames[] = 
{
	////FRAME: body 1
	{//0
		-12,-32,		//x,y offset
		0,			//tile
		TALL,		//shape (SQUARE/TALL/WIDE)
		SIZE_32,	//size
		false, 		//flip
		Frame(1) //nextSprite
	},
	{//1 - body 2
		4,-32,			//x,y offset
		2,				//tile
		TALL,			//shape (SQUARE/TALL/WIDE)
		SIZE_16,		//size
		false, 
		null 			//nextSprite
	},
	{//2 - neck piece
		-4,-4,
		3,
		SQUARE,
		SIZE_8,
		false,
		null
	},
	{//3 head up
		-8,-26,
		4,
		TALL,
		SIZE_32,
		false,
		null
	} ,
	{//4 head right
		-8, -12,
		6,
		WIDE,
		SIZE_32,
		false,
		null
	},
	{//5 head left
		-32, -12,
		6,
		WIDE,
		SIZE_32,
		true,
		null
	},
	{//6 upper-right
		-8, -26,
		10,
		TALL,
		SIZE_32,
		false,
		Frame(7)
	},
	{//7 upper-right
		8, -26,
		12,
		TALL,
		SIZE_16,
		false,
		null
	},
	{//8 upper-left
		-8, -26,
		10,
		TALL,
		SIZE_32,
		true,
		Frame(9)
	},
	{//9 upper-left
		-16, -26,
		12,
		TALL,
		SIZE_16,
		true,
		null
	},
	////FRAME: body 2
	{//10
		-12,-32,		//x,y offset
		14,			//tile
		TALL,		//shape (SQUARE/TALL/WIDE)
		SIZE_32,	//size
		false, 		//flip
		Frame(11) //nextSprite
	},
	{//11 - body 2
		4,-32,			//x,y offset
		16,				//tile
		TALL,			//shape (SQUARE/TALL/WIDE)
		SIZE_16,		//size
		false, 
		null 			//nextSprite
	},





};

const characterAnimDef enemyCrockyCharAnim[] = 
{
	{//0 - body
		30,
		TILES,
		Frame(0),
		Anim(7)
	},
	{//1 - neck piece
		999999,
		TILES,
		Frame(2),
		Anim(1)
	},
	{//2 - head up
		999999,
		TILES,
		Frame(3),
		Anim(2)
	},
	{//3 - head right
		999999,
		TILES,
		Frame(4),
		Anim(3)
	},
	{//4 - head left
		999999,
		TILES,
		Frame(5),
		Anim(4)
	},
	{//5 - head upper-right
		999999,
		TILES,
		Frame(6),
		Anim(5)
	},
	{//6 - head upper-left
		999999,
		TILES,
		Frame(8),
		Anim(6)
	},
	{//7 - body 2
		30,
		TILES,
		Frame(10),
		Anim(0)
	},





};

int enemyCrockyHeadFunc(Enemy * enemy, int func);

#define BBH_DIAG 20
#define BBW_DIAG 16
#define BBY_DIAG -20
#define BBX_DIAG_L -10
#define BBX_DIAG_R 6

#define BBH_FLAT 16
#define BBW_FLAT 24
#define BBY_FLAT -16
#define BBX_FLAT_L -20
#define BBX_FLAT_R -6

#define BBH_UP 24
#define BBW_UP 12
#define BBY_UP -20
#define BBX_UP -6

#define BULLET_RATE 160

const EnemyDatabaseDef enemy_crocky_db = 
{
	crocky_portrait_Tiles,
	crocky_portrait_Pal,
	crocky_portrait_Map,
	"FireCroc",
	"Fire breathing Crocodile",
	"Spits 3 fireballs",
	"at a time.",
		 null,
	7,11,0

};



/************ Body ********************/


int enemyCrockyBodyInit(Enemy * enemy)
{
	initEnemyGfx(enemy, TILES, (characterAnimDef*) enemyCrockyCharAnim);
	enemy->status->mobility = MOBILITY_GHOST;
	return 1;
}


int enemyCrockyBodyUpdate(Enemy * enemy)
{

	return 1;
}

int enemyCrockyBodyFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyCrockyBodyInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyCrockyBodyUpdate(enemy);
			break;
		case ENEMY_BULLET_COLLIDE:
			return 0;		//takes no normal damage
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}


	
/************ Neck ********************/


int enemyCrockyNeckInit(Enemy * enemy)
{
	initEnemyGfx(enemy, TILES, (characterAnimDef*) Anim(1));
	return 1;
}


int enemyCrockyNeckUpdate(Enemy * enemy)
{
	return 1;
}

int enemyCrockyNeckFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyCrockyNeckInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyCrockyNeckUpdate(enemy);
			break;
		case ENEMY_BULLET_COLLIDE:
			return 0;		//takes no normal damage
			break;
		case ENEMY_DEAD:
			return killEnemy(enemy);
			break;
	}
	return 1;
}



/************ Head ********************/


int enemyCrockyHeadInit(Enemy * enemy)
{
	initEnemyGfx(enemy, TILES, (characterAnimDef*) Anim(2));
	enemy->state4 = 0;
	return 1;
}

void enemyCrockyHeadShootBullet(Enemy * enemy, int speedX, int speedY)
{
	//new bullet!
	if ((speedX == 0) && (speedY == 0))
	{
		return;
	}
	BulletDef bullet;
	bullet.team = TEAM_ENEMY;
	bullet.tile = findActualTileFromEnemyTile(enemy, CROCKY_BULLET_START);
	bullet.ttl = 9999;
	bullet.xSpeed = unitVal(speedX);
	bullet.ySpeed = unitVal(speedY);
	bullet.xFrameskip = abs(speedX);
	bullet.yFrameskip = abs(speedY);
	bullet.pierce = false;
	if (enemy->special > 0)
	{
		bullet.power = enemy->special;
	}
	else
	{
		bullet.power = 3;
	}
	bullet.x = enemy->state1 - 8;
	bullet.y = enemy->state2 + 8;
	bullet.width = 16;
	bullet.height = 16;
	bullet.sourceX = enemy->status->x;
	bullet.sourceY = enemy->status->y;
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
		setSpritePalette(createdBullet->sprite, findPalette((u16*)crockyGfxPal));
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


#define HEAD_R  4
#define HEAD_UR 3
#define HEAD_U  2
#define HEAD_UL 1
#define HEAD_L  0


SignedPair enemyCrockHeadReposition(Enemy * enemy, int position)
{
	s16 bulletX = -10;
	s16 bulletY = -10;
	switch (position)
	{
		case HEAD_R:
			setCharacterAnimation(enemy->status, Anim(3));
			setBoundingBlock(enemy->status, BBW_FLAT, BBH_FLAT, BBX_FLAT_R, BBY_FLAT);
			enemy->state1 = enemy->status->x + BBW_FLAT + BBX_FLAT_R;
			enemy->state2 = enemy->status->y + BBY_FLAT;
			break;
		case HEAD_UR:
			bulletY = -5 + dice(0,3);
			bulletX = 1 + dice(0,1);
			setCharacterAnimation(enemy->status, Anim(5));
			setBoundingBlock(enemy->status, BBW_DIAG, BBH_DIAG, BBX_DIAG_R, BBY_DIAG);
			enemy->state1 = enemy->status->x - BBX_DIAG_R + BBW_DIAG;
			enemy->state2 = enemy->status->y + BBY_DIAG + BBY_DIAG;
			break;
		case HEAD_L:
			bulletY = -5 + dice(0,3);
			bulletX = -2 + dice(0,1);
			setCharacterAnimation(enemy->status, Anim(4));
			setBoundingBlock(enemy->status, BBW_FLAT, BBH_FLAT, BBX_FLAT_L, BBY_FLAT);
			enemy->state1 = enemy->status->x - BBW_FLAT + BBX_FLAT_L;
			enemy->state2 = enemy->status->y + BBY_FLAT;
			break;
		case HEAD_UL:
			setCharacterAnimation(enemy->status, Anim(6));
			setBoundingBlock(enemy->status, BBW_DIAG, BBH_DIAG, BBX_DIAG_L, BBY_DIAG);
			enemy->state1 = enemy->status->x - BBW_DIAG;
			enemy->state2 = enemy->status->y + BBY_DIAG + BBY_DIAG;
			break;
		case HEAD_U:
			bulletX = -3 + dice(0,6);
			setCharacterAnimation(enemy->status, Anim(2));
			setBoundingBlock(enemy->status, BBW_UP, BBH_UP, BBX_UP, BBY_UP);
			enemy->state1 = enemy->status->x;
			enemy->state2 = enemy->status->y + BBY_UP;
			break;
	}

	return signedPair(bulletX, bulletY);
	
}

void enemyCrockyMoveX(Enemy * enemy, int xd)
{
	Enemy * bodyPart = enemy;
	if (moveEnemy(bodyPart, xd, 0))
	{
		//bodyPart->status->x += xd;

		bodyPart--;
		bodyPart->status->x += xd;

		//and the neck
		bodyPart++;
		bodyPart++;
		bodyPart->status->x += xd;
		bodyPart++;
		bodyPart->status->x += xd;
	}
}

int enemyCrockyHeadUpdate(Enemy * enemy)
{
	CharacterStatus * mcStatus = getMainCharStatus();
	int dx = mcStatus->x - enemy->status->x;
	int dy = enemy->status->y - mcStatus->y;
	int bulletX = -10;
	int bulletY = -10;
	int position = HEAD_U;
	int moveDir = 0;
	if ((dx > 0) && (dx > (dy >> 1)))
	{
		if ((dx >> 1) > dy)
		{
			//head facing right
			position = HEAD_R;
		}
		else
		{
			//head upper-right
			position = HEAD_UR;
		}
	}
	else if ((dx < 0) && (-dx > (dy >> 1)))
	{
		if (((abs(dx)) >> 1) > abs(dy))
		{
			//head facing left
			position = HEAD_L;
		}
		else
		{
			//head upper-left
			position = HEAD_UL;
		}
		//enemy->enemyType = (EnemyType * ) &enemy_crockyHead_def;
	}
	enemyCrockHeadReposition(enemy, position);

	moveDir = unitVal(dx);

	enemy->state3++;
	if (enemy->state3 > 1)
	{
		enemy->state3 = 0;
		enemyCrockyMoveX(enemy, moveDir);
	}


	if (enemy->state4 > BULLET_RATE)
	{
		enemy->state4 = 0;

		if (bulletX == -10)
		{
			bulletX = (unitVal(dx) * dice(1,3));
		}
		if (bulletY == -10)
		{
			bulletY = (-unitVal(dy) * dice(1,3));
		}
		//enemyCrockyHeadShootBullet(enemy, bulletX, bulletY);
		//enemyCrockyHeadShootBullet(enemy, bulletX + dice(0,2), bulletY + dice(0,1));
		//enemyCrockyHeadShootBullet(enemy, bulletX + dice(0,1), bulletY + dice(0,2));
		if (position == HEAD_L)
		{
			enemyCrockyHeadShootBullet(enemy, -2, 2);	
		}
		if (position <= HEAD_UL)
		{
			enemyCrockyHeadShootBullet(enemy, -1, 0);	
		}
		if (position <= HEAD_U)
		{
			enemyCrockyHeadShootBullet(enemy, -2, -2);	
		}
		if ((position <= HEAD_UR) && (position >= HEAD_UL))
		{
			enemyCrockyHeadShootBullet(enemy, 0, -1);	
		}
		if (position == HEAD_R)
		{
			enemyCrockyHeadShootBullet(enemy, 2, 2);	
		}
		if (position >= HEAD_UR)
		{
			enemyCrockyHeadShootBullet(enemy, 1, 0);	
		}
		if (position >= HEAD_U)
		{
			enemyCrockyHeadShootBullet(enemy, 2, -2);	
		}
	}

	enemy->state4++;

	return 1;
}

int enemyCrockyHeadKill(Enemy * enemy)
{
	Enemy * bodyPart = enemy;

	//kill the body 
	bodyPart--;
	bodyPart->enemyType->updateFunction(bodyPart, ENEMY_DEAD);

	//and the neck
	bodyPart++;
	bodyPart++;
	bodyPart->enemyType->updateFunction(bodyPart, ENEMY_DEAD);
	bodyPart++;
	bodyPart->enemyType->updateFunction(bodyPart, ENEMY_DEAD);

	return killEnemy(enemy);
	
}



int enemyCrockyHeadFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyCrockyHeadInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyCrockyHeadUpdate(enemy);
			break;
		case ENEMY_BULLET_COLLIDE:
			return ENEMY_HIT_NOBOUNCE;	
			break;
		case ENEMY_DEAD:
			return enemyCrockyHeadKill(enemy);
			return killEnemy(enemy);
			break;
	}
	return 1;
}



const EnemyType enemy_crockyHead_def = 
{
	20,				//hp
	4,				//collideDamage
	2,				//numSprites
	BBH_UP,				//bbHeight
	BBW_UP,				//bbWidth
	BBX_UP,			//bbOffx
	BBY_UP,			//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyCrockyHeadFunc,	//updateFunction
	(short*)&crockyGfxPal,
	&enemy_crocky_db
		
};

const EnemyType enemy_crockyHeadFinal_def = 
{
	60,				//hp
	24,				//collideDamage
	2,				//numSprites
	BBH_UP,				//bbHeight
	BBW_UP,				//bbWidth
	BBX_UP,			//bbOffx
	BBY_UP,			//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyCrockyHeadFunc,	//updateFunction
	(short*)&crockyGfxPal,
	&enemy_crocky_db
		
};

const EnemyType enemy_crockyBody_def = 
{
	1,				//hp
	6,				//collideDamage
	2,				//numSprites
	32,				//bbHeight
	24,				//bbWidth
	-12,				//bbOffx
	-32,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyCrockyBodyFunc,	//updateFunction
	(short*)&crockyGfxPal,
	null
};

const EnemyType enemy_crockyNeck_def = 
{
	1,				//hp
	4,				//collideDamage
	1,				//numSprites
	8,				//bbHeight
	8,				//bbWidth
	-4,				//bbOffx
	-4,				//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyCrockyNeckFunc,	//updateFunction
	(short*)&crockyGfxPal,
	null
};
