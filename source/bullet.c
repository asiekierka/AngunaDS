#include "bullet.h"
#include "spriteMgr.h"
#include "misc.h"
#include "level.h"
#include "text.h"
#include "enemies.h"
#include "mainChar.h"
#include "characters.h"
#include "items.h"

#define BULLET_MAX 40

BulletDef bullets[BULLET_MAX];
int nextBullet;


void killBulletByNumber(int toUse, int why, void * target);


CharacterStatus bulletOwner;   //a dummy character used to hold the bullets for sorting
Sprite * bulletSprites[BULLET_MAX]; //the spritelist for the character



void initBulletSystem()
{
	//does init bullets, but also sets all sprites to null...
	//we should only do this the one time...every other time
	//that we want to init bullets, we need to make sure to
	//clear all their sprites
	int ctr;
	for (ctr = 0; ctr < BULLET_MAX; ctr++)
	{
		bullets[ctr].sprite = null;
	}
	initBullets();
}

void initBullets()
{
	int ctr;

	nextBullet = 0;
	for (ctr = 0; ctr < BULLET_MAX; ctr++)
	{
		bullets[ctr].ttl = 0;
		bullets[ctr].deadFunction = null;
		killBulletByNumber(ctr, BULLET_INIT, null);
	}

	bulletOwner.y = 10;
	bulletOwner.listSize = BULLET_MAX;
	bulletOwner.spriteList = (SpriteList)bulletSprites;
	bulletOwner.priority = CHAR_PRIORITY_LOW;
	resetBulletItems();

	
}

extern void itemCaltropsDead(int x, void * y);
extern void itemDynamiteDead(int x, void * y);

void killBulletByNumber(int toUse, int why, void * target)
{
	bullets[toUse].ttl = 0;
	if (bullets[toUse].deadFunction != null)
	{
		if (bullets[toUse].deadFunction != itemCaltropsDead)
		if (bullets[toUse].deadFunction != itemDynamiteDead)
		{
			writeText(3,3,"bad dead function for bullet");
		}
		bullets[toUse].deadFunction(why, target);
	}
	bulletSprites[toUse] = null;

	if (bullets[toUse].sprite != null)
	{
		releaseSprite(bullets[toUse].sprite);
	}
}

BulletDef * setBulletDef(int toUse, BulletDef * definition)
{
	bullets[toUse] = *definition;
	BulletDef * bullet = (BulletDef*)&(bullets[toUse]);
	bullet->deadFunction = null;
	bullet->updateFunction = null;
	//bullet->lastHit = null;

	//bullet->sprite = getSprite(bullet->sprite);
	bullet->sprite = getSpriteWithOwner(bullet->sprite, &bulletOwner);
	bulletSprites[toUse] = bullet->sprite;

	//increments nextBullet to the next one in the list
	nextBullet = toUse++;
	if (nextBullet == BULLET_MAX)
	{
		nextBullet = 0;
	}
	

	setSpriteShape(bullet->sprite, SQUARE);
	setSpriteSize(bullet->sprite, SIZE_8);
	setSpritePosition(bullet->sprite, worldToScreenX(bullet->x), worldToScreenY(bullet->y));
	setSpriteStartTile(bullet->sprite, bullet->tile);
	bullet->frameCountX = 0;
	bullet->frameCountY = 0;

	bullet->spriteOffsetX = 0;
	bullet->spriteOffsetY = 0;

	return bullet; 
}

BulletDef * newBullet(BulletDef * definition)
{
	int toUse;
	toUse = nextBullet;
	int startPoint = -1;
	while (toUse != startPoint)
	{
		//loop through until we find a dead bullet
		if (bullets[toUse].ttl == 0)
		{
			return setBulletDef(toUse, definition);
		}
		toUse++;

		//if we get to the last bullet, loop back to the first
		if (toUse >= BULLET_MAX)
		{
			toUse = 0;
			startPoint = nextBullet;
		}
	}

	//if we're back at the starting point, there are 
	//no more bullets, can't create it
	//writeText(5,5,"could not create bullet!");
	//writeText(5,6,"could not create bullet!");
	//writeText(5,7,"could not create bullet!");
	//writeText(5,8,"could not create bullet!");
	
	return null;
}

int getBulletNum(BulletDef * bullet)
{
	int ctr = 0;
	BulletDef * tmp;
	tmp = (BulletDef*)&(bullets[0]);
	while (ctr < BULLET_MAX)
	{
		if (tmp == bullet)
		{
			return ctr;
		}
		tmp++;
		ctr++;
	}
	return -1;
}



void updateOneBullet(int toUse)
{
	//updates one bullet
	BulletDef * bullet = (BulletDef*)&(bullets[toUse]);

	if (bullet->ttl < 1)
	{
		return;
	}

	if (bullets[toUse].updateFunction != null)
	{
		bullets[toUse].updateFunction(&(bullets[toUse]));
	}

	if (bullet->xSpeed != 0)
	if (bullet->frameCountX++ >= bullet->xFrameskip)
	{
		bullet->x += bullet->xSpeed;
		bullet->frameCountX = 1;
	}

	if (bullet->ySpeed != 0)
	if (bullet->frameCountY++ >= bullet->yFrameskip)
	{
		bullet->y += bullet->ySpeed;
		bullet->frameCountY = 1;
	}

	if (!objectIsOnScreen(bullet->width, bullet->height, worldToScreenX(bullet->x - bullet->spriteOffsetX), worldToScreenY(bullet->y - bullet->spriteOffsetY)))
	{
		setSpritePosition(bullet->sprite, -16, -16);
	}
	else
	{
		setSpritePosition(bullet->sprite, 
						  worldToScreenX(bullet->x) + bullet->spriteOffsetX,
						  worldToScreenY(bullet->y) + bullet->spriteOffsetY);
	}


	//if offscreen, kill the bullet
	//if (!objectIsOnScreen(bullet->width, bullet->height, worldToScreenX(bullet->x), worldToScreenY(bullet->y)))
	if (!objectIsInGameSpace(bullet->x,bullet->y))
	{
		killBulletByNumber(toUse, BULLET_OFFSCREEN, null);
		return;
	}
	

	Enemy * collider = checkBulletCollidesWithEnemy(bullet);
	if (collider != null)
	{
		if (!bullet->pierce)
		{
			killBulletByNumber(toUse, BULLET_HIT, (void*)collider);
			return;
		}
	
	}

	if (checkBulletCollidesWithMC(bullet))
	{
		if (!bullet->pierce)
		{
			killBulletByNumber(toUse, BULLET_HIT, null);
			return;
		}
	}

	
	bullet->ttl--;

	//if bullet has run out of time, kill the bullet
	if (bullet->ttl <= 0)
	{
		killBulletByNumber(toUse, BULLET_TIMEOUT, null);
	}
	
}

extern void itemDynamiteUpdate(BulletDef * bullet);
bool bulletIsDynamite(BulletDef * bullet)
{
	return bullet->updateFunction == itemDynamiteUpdate;
}


void updateOneBulletDef(BulletDef * bullet)
{
	int num = getBulletNum(bullet);
	updateOneBullet(num);
}



//iteratively updates each bullet
void updateBullets()
{
	int toUse = 0;

	while (toUse < BULLET_MAX)
	{
		updateOneBullet(toUse);
	
		toUse++;
	}
}




void killBullet(BulletDef * bullet, int why)
{
	//look for the bullet matchign the pointer, and kill it by number
	int ctr = 0;
	BulletDef * tmp;
	tmp = (BulletDef*)&(bullets[0]);
	while (ctr < BULLET_MAX)
	{
		if (tmp == bullet)
		{
			killBulletByNumber(ctr, why, null);
			return;
		}
		tmp++;
		ctr++;
	}
}

void setBulletClockDirection(BulletDef * bullet, int clockpoint, int speedFactor)
{
	bullet->xFrameskip = 1;
	bullet->yFrameskip = 1;
	bullet->xSpeed = 0;
	bullet->ySpeed = 0;
	int slowf = 2;
	int fastf = 3;
	int speed = 1;

	if (speedFactor >= 1)
	{
		slowf = 1;
		fastf = 2;
		if (speedFactor >= 2)
		{
			speed *= speedFactor;
		}
	}

	switch (clockpoint)
	{
		case 0:
		case 12:
			bullet->ySpeed = -speed;
			break;
		case 1:
		case 11:
			bullet->ySpeed = -speed;
			bullet->yFrameskip = slowf;
			bullet->xSpeed = speed;
			bullet->xFrameskip = fastf;
			break;
		case 2:
		case 10:
			bullet->ySpeed = -speed;
			bullet->yFrameskip = fastf;
			bullet->xSpeed = speed;
			bullet->xFrameskip = slowf;
			break;
		case 3:
		case 9:
			bullet->xSpeed = speed;
			break;
		case 4:
		case 8:
			bullet->ySpeed = speed;
			bullet->yFrameskip = fastf;
			bullet->xSpeed = speed;
			bullet->xFrameskip = slowf;
			break;
		case 5:
		case 7:
			bullet->ySpeed = speed;
			bullet->yFrameskip = slowf;
			bullet->xSpeed = speed;
			bullet->xFrameskip = fastf;
			break;
		case 6:
			bullet->ySpeed = speed;
			break;
	}
	
	if (clockpoint > 6)
	{
		bullet->xSpeed = -bullet->xSpeed;
	}
	
}

void setBulletSpriteOffset(BulletDef * bullet, int offsetX, int offsetY)
{
	bullet->spriteOffsetX = offsetX;
	bullet->spriteOffsetY = offsetY;
}
