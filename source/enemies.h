#ifndef _ENEMIES_H_
#define _ENEMIES_H_

#include "characters.h"
#include "bullet.h"
#include "misc.h"

#define ENEMY_INIT 1
#define ENEMY_UPDATE 2
#define ENEMY_DESTROY 3
#define ENEMY_MC_COLLIDE 4
#define ENEMY_BULLET_COLLIDE 5
#define ENEMY_DEAD 6
#define ENEMY_ENEMY_COLLIDE 7
#define ENEMY_DYNAMITED 8

#define ENEMY_SAVES 0x8000
#define DOES_ENEMY_SAVE(x)	((x) & ENEMY_SAVES)
#define ENEMY_SAVE_SLOT(x)   ((x)<<0x17)
#define ENEMY_WHICH_SAVE_SLOT(x)   ((x)>>0x17)


#define DOES_DAMAGE 1

#define ENEMY_DIR_N 0
#define ENEMY_DIR_E 1
#define ENEMY_DIR_S 2
#define ENEMY_DIR_W 3
#define ENEMY_DIR_NULL 4

#define	ENEMY_N_A 0
#define ENEMY_N_B 1
#define ENEMY_S_A 2
#define ENEMY_S_B 3
#define ENEMY_E_A 4
#define ENEMY_E_B 5
#define ENEMY_W_A 6
#define ENEMY_W_B 7
#define ENEMY_EXTRA 8

#define ENEMY_DIR_TO_ANIM(x) ((x)*2)
#define ENEMY_XDIR(x) ((x)==1?1:((x)==3?-1:0))
#define ENEMY_YDIR(y) ((x)==2?1:((x)==0?-1:0))

#define ENEMY_FADE_TIME 15

//the next clockwise direction
#define DIR_CW(x) ((x)+1>3?0:(x)+1)
#define DIR_CCW(x) ((x)-1<0?3:(x)-1)


#define ENEMY_HIT_NOBOUNCE 3

#define ENEMY_COLLIDE_NO_DMG -1

#define NO_COLLISION 0

#define FRAME_SM_SQ_POS 0,0
#define FRAME_SMALL_SQUARE(start,flip) FRAME_SM_SQ_POS,(start),SQUARE,SIZE_16,(flip),null
#define FRAME_8_SQUARE(start,flip) FRAME_SM_SQ_POS,(start),SQUARE,SIZE_8,(flip),null
#define FRAME_SMALL_SQUARE_N(start,flip,next) FRAME_SM_SQ_POS,(start),SQUARE,SIZE_16,(flip),(next)
#define FRAME_32_SQUARE(start,flip) FRAME_SM_SQ_POS,(start),SQUARE,SIZE_32,(flip),null
#define FRAME_32_SQUARE_N(start,flip,next) FRAME_SM_SQ_POS,(start),SQUARE,SIZE_32,(flip),(next)


//"forward declare" the Enemy type so I can use it now
struct EnemyStruct;
typedef struct EnemyStruct Enemy;

typedef int(*EnemyUpdateFunction)(Enemy *, int );

typedef struct
{
	const u16 * portraitGfx;
	const u16 * portraitPal;
	const u16 * portraitMap;
	char * name;
	char * description1;
	char * description2;
	char * description3;
	char * description4;
	int desc2X;
	int desc3X;
	int desc4X;

} EnemyDatabaseDef;


typedef struct
{
	int startHP;
	int collideDamage;
	int numSprites;
	int boundingBlockHeight;
	int boundingBlockWidth;
	int boundingBlockOffsetX;
	int boundingBlockOffsetY;
	bool respawnOnScroll;
	bool killOffScreen;
	bool isEnemy;
	EnemyUpdateFunction updateFunction;
	short * palette;
	const EnemyDatabaseDef * databaseDef;

} EnemyType;

typedef struct
{
	int x;
	int y;
	EnemyType * enemyDef;
	int itemDrop;
	int special;
	
} EnemyStartData;


struct EnemyStruct
{
	CharacterStatus * status;
	EnemyType * enemyType;
	int hp;
	bool active;
	int state1;
	int state2;
	int state3;
	int state4;
	void * statePointer;
	int itemDrop;
	int special;
	int forceX;
	int forceY;
	int trapped;
	BulletDef * bulletCheck;

};

//these functions are to be called by other modules
Enemy * createNewEnemy(EnemyType * enemyType, int x, int y, int itemDrop, int special);
void updateAllEnemies();
void resetEnemies(); //kills all enemies -- releases their character sprites
void resetEnemy(Enemy * enemy); //kills one enemy and releases sprites
void initEnemies(int numEnemies, EnemyStartData * enemyStartData);
Enemy * checkCollidesWithEnemy(CharacterStatus * status);
bool checkMCNoDmgCollision(CharacterStatus * status);
Enemy * checkBulletCollidesWithEnemy(BulletDef * bullet);

//this uses state1 as timer, state2 as enemy state
bool enemyStateAdvance(Enemy * enemy, int timeout, int nextState);


//these are for being called by enemy definitions
bool moveEnemy(Enemy * enemy, int deltaX, int deltaY);
bool moveEnemyDirection(Enemy * enemy, int direction);
bool moveEnemyFast(Enemy * enemy, int deltaX, int deltaY);

//just like moveenemy, only enemies can block each other from moving
//if block is true, then this enemy will be blocked if other enemies in the way
bool moveEnemyCanBlock(Enemy * enemy, int deltaX, int deltaY, bool block);
void initEnemyGfx(Enemy * enemy, u16 * gfxData, characterAnimDef * firstAnimFrame);
void enemyBounceAwayFrom(Enemy * enemy, int x, int y);
int killEnemy(Enemy * enemy);
void killAllActualEnemies();
int findActualTileFromEnemyTile(Enemy * enemy, int tile);
bool checkEnemyCollidesWithAnyEnemy(Enemy * enemy);

Enemy * createNewEnemyWithSlot(EnemyType * enemyType, int x, int y, Enemy * enemy, CharacterStatus * status,
				int itemDrop, int special);

int numEnemiesAlive();
BulletDef * fireEnemyBullet(Enemy * enemy, int tile, int sourceOffX, int sourceOffY, int xd, int yd);
SignedPair getMovementFromEnemyDirection(int direction);
void initEnemySystem();



#endif
