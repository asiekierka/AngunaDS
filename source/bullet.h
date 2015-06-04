#ifndef _BULLET_H_
#define _BULLET_H_

#include "spriteMgr.h"

#define TEAM_HERO 0
#define TEAM_ENEMY 1
#define TEAM_NONE 2

#define BULLET_INFINITE_TTL 9999

struct BulletDefStruct;

typedef void(*BulletDeadFunction)(int, void *);
typedef void(*BulletUpdateFunction)(struct BulletDefStruct *);

#define BULLET_HIT_NONE 0
#define BULLET_HIT_DMG 1
#define BULLET_HIT_ABSORB 2

enum
{
	BULLET_INIT,
	BULLET_OFFSCREEN,
	BULLET_TIMEOUT,
	BULLET_HIT
};


typedef struct BulletDefStruct
{
	int tile;
	int ttl;
	int xSpeed;
	int ySpeed;
	int xFrameskip;
	int yFrameskip;
	int team;
	bool pierce;
	int power;
	int x;
	int y;
	int width;
	int height;
	int sourceX;
	int sourceY;
	int frameCountX;
	int frameCountY;
	int spriteOffsetX;
	int spriteOffsetY;
	Sprite * sprite;
	BulletDeadFunction deadFunction;
	BulletUpdateFunction updateFunction;
	//void * lastHit;
	
} BulletDef;


//be careful with this function....definition only defines the bullet
//a copy of this struct will be made for the actual bullet.
//a pointer to this copy is returned by the function on success (null on failure)
BulletDef * newBullet(BulletDef * definition);
void updateBullets();
void initBullets();
void killBullet(BulletDef * bullet, int why);
void setBulletClockDirection(BulletDef * bullet, int clockpoint, int speedFactor);
void updateOneBulletDef(BulletDef * bullet);
void setBulletSpriteOffset(BulletDef * bullet, int offsetX, int offsetY);
bool bulletIsDynamite(BulletDef * bullet);
void initBulletSystem();

#endif
