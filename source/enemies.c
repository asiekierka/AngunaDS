#include "enemies.h"
#include "characters.h"
#include "text.h"
#include "level.h"
#include "bullet.h"
#include "misc.h"
#include "sound.h"
#include "item.h"
#include "items.h"
#include "spriteMgr.h"
#include "gameState.h"
#include "enemyDatabase.h"
#include "shop.h"


#define MAX_ENEMIES 25
#define MAX_ENEMY_SPRITES 128

#define ENEMY_BOUNCE_DISTANCE 16
#define ENEMY_BOUNCE_SPEED 4

Enemy enemyList[MAX_ENEMIES];
CharacterStatus enemyCharacters[MAX_ENEMIES];
Sprite * enemySprites[MAX_ENEMY_SPRITES];

int maxEnemy;
int maxSpriteList;

int Enemies_NumAlive;

int updateEnemy(Enemy * enemy);
bool isActualEnemy(Enemy * enemy);

Enemy * createNewEnemyWithSlot(EnemyType * enemyType, int x, int y, Enemy * enemy, CharacterStatus * status,
				int itemDrop, int special)
{
	Enemy * newEnemy = enemy;

	ASSERT(status != null, "Status is null");

	newEnemy->status = status;

	//set basic enemy info
	newEnemy->enemyType = enemyType;
	newEnemy->hp = enemyType->startHP;
	newEnemy->forceX = 0;
	newEnemy->forceY = 0;
	newEnemy->status->boundingBlockOffsetX = enemyType->boundingBlockOffsetX;
	newEnemy->status->boundingBlockOffsetY = enemyType->boundingBlockOffsetY;
	newEnemy->status->boundingBlockHeight = enemyType->boundingBlockHeight;
	newEnemy->status->boundingBlockWidth = enemyType->boundingBlockWidth;

	LevelData * level = getCurrentLevel();

	newEnemy->status->x = x;
	newEnemy->status->y = y;
	newEnemy->itemDrop = itemDrop;
	newEnemy->special = special;

	newEnemy->state1 = 0;
	newEnemy->state2 = 0;
	newEnemy->state3 = 0;
	newEnemy->state4 = 0;
	newEnemy->statePointer = null;

	newEnemy->trapped = 0;

	//reserve the sprites for the new enemy
	getMultipleSpritesWithOwner(&(enemySprites[maxSpriteList]), enemyType->numSprites, newEnemy->status);
	if (enemyType->numSprites > 10)
	{
		ASSERT(false, enemyType->databaseDef->name);
	}
	newEnemy->status->spriteList = &(enemySprites[maxSpriteList]);
	newEnemy->status->listSize = enemyType->numSprites;
	newEnemy->status->mobility = MOBILITY_NORMAL;
	newEnemy->status->priority = CHAR_PRIORITY_NORMAL;
	newEnemy->status->faded = false;
	maxSpriteList += enemyType->numSprites;

	//call the enemy's custom update function
	//gotta be careful with this one, will crash the game if not setup correctly....
	//also, the enemy init function MUST initialize the sprite animations
	newEnemy->enemyType->updateFunction(newEnemy, ENEMY_INIT);

	if (newEnemy->status->listSize > 10)
	{
		ASSERT(false, enemy->enemyType->databaseDef->name);
	}

	//set them active, and let them go play!
	newEnemy->active = true;
	updateEnemy(newEnemy);

	int maxTries = 5;

	if ((x == ENEMY_RANDOM_LOCATION) || (y == ENEMY_RANDOM_LOCATION))
	{
		int tries = 0;
		bool loop = true;

		while (loop)
		{
			newEnemy->status->x = dice(20,(level->width * 8) - 20);
			newEnemy->status->y = dice(20,(level->height * 8) - 20);
			tries++;

			if (moveEnemyCanBlock(newEnemy, 1, 1, true) && (moveEnemyCanBlock(newEnemy, -1, -1, true)))
			{
				loop = false;
			}
			if (!isClearOfDoor(newEnemy->status))
			{
				loop = true;
			}
			if (tries >= maxTries)
			{
				loop = false;
			}
		}

		if (tries >= maxTries)
		{
			enemy->hp = 0;
			resetEnemy(enemy);
		}

	}

	return newEnemy;
}

Enemy * createNewEnemy(EnemyType * enemyType, int x, int y, int itemDrop, int special)
{
	//if too many enemies, can't create a new one
	int toUse = -1;
	int i = 0;
	if (maxEnemy + 1 >= MAX_ENEMIES)
	{
		for (i = 0; i <= maxEnemy; i++)
		{
			Enemy * enemy = &(enemyList[i]);
			if (!(enemy->active))
			{
				toUse = i;
				i = maxEnemy;
			}
	
		}
		if (i == -1)
		{
			ASSERT(false, "max enemies hit");
			return null;
		}
	}
	else
	{
		maxEnemy++;
		toUse = maxEnemy;
	}



	//if not enough sprites left, can't create new enemy
	if (maxSpriteList + enemyType->numSprites >= MAX_ENEMY_SPRITES)
	{
		ASSERT(false, "no more sprites");
		return null;
	}

	return createNewEnemyWithSlot(enemyType, x, y, &(enemyList[toUse]), &(enemyCharacters[toUse]), itemDrop, special);

}

Enemy * blockers[MAX_ENEMIES];
int numEnemyBlockers;


void findBlockingEnemies()
{
	numEnemyBlockers = 0;
	int i = 0;
	for (i = 0; i <= maxEnemy; i++)
	{
		Enemy * enemy = &(enemyList[i]);
		if (enemy->active)
		{
			if (enemy->enemyType->updateFunction(enemy,ENEMY_ENEMY_COLLIDE) == ENEMY_COLLIDE_NO_DMG)
			{
				blockers[numEnemyBlockers] = enemy;
				numEnemyBlockers++;
			}
	
		}
	}
}

void updateAllEnemies()
{


	findBlockingEnemies();


	//call the update function for each enemy
	int i;
	int Enemies_NumAlive_tmp = 0;
	for (i = 0; i <= maxEnemy; i++)
	{
		Enemy * enemy = &(enemyList[i]);
		if (enemy->active)
		{
			updateEnemy(&(enemyList[i]));
			if (isActualEnemy(&(enemyList[i])))
			{
				Enemies_NumAlive_tmp++;
			}
		}
	}

	Enemies_NumAlive = Enemies_NumAlive_tmp;
	
	if (Enemies_NumAlive == 0)
	{
		levelAllEnemiesDead();
	}
	
}

void resetEnemy(Enemy * enemy)
{
	//relese character sprites
	if (enemy->status != null)
	{
		releaseMultipleSprites(enemy->status->spriteList, enemy->status->listSize);
	}
	enemy->status = null;
	enemy->enemyType = null;
	enemy->active = false;
}

void initEnemySystem()
{
	int i;
	for (i = 0; i < MAX_ENEMIES; i++)
	{
		enemyList[i].status = null;
		enemyList[i].enemyType = null;
		enemyList[i].active = false;
	}
	maxEnemy = -1;
	maxSpriteList = 0;
}

void resetEnemies()
{
	int i;
	for (i = 0; i <= maxEnemy; i++)
	{
		//removeFromOwnerList(enemyList[i].status);
		resetEnemy(&(enemyList[i]));
	}
	maxEnemy = -1;
	maxSpriteList = 0;
}

void killAllActualEnemies()
{
	int i;
	for (i = 0; i <= maxEnemy; i++)
	{
		Enemy * enemy = &enemyList[i];
		if (isActualEnemy(enemy))
		{
			resetEnemy(&enemyList[i]);
		}
	}
	
}


void initEnemies(int numEnemies, EnemyStartData * enemyStartData)
{
	TRACE();
	resetEnemies();

	int i;
	//loop through and create each enemy in the init list
	
	Enemy * enemy;
	int enemyCnt = 0;
	for (i = 0; i < numEnemies; i++)
	{
		//don't create the enemy if it's saved as dead
		int saver = enemyStartData[i].itemDrop;
		if ((!DOES_ENEMY_SAVE(saver)) || (!getGameState(ENEMY_WHICH_SAVE_SLOT(saver))))
		{
			enemy = createNewEnemy(enemyStartData[i].enemyDef, enemyStartData[i].x, enemyStartData[i].y, 
						enemyStartData[i].itemDrop, enemyStartData[i].special);

			if (isActualEnemy(enemy))
			{
				enemyCnt++;
			}
		}
	}

	//track how many actual actual ENEMIES are alive (not items/doors/etc)
	Enemies_NumAlive = enemyCnt;
	
}

int updateEnemy(Enemy * enemy)
{
	TRACE();
	if (!enemy->active)
	{
		return 0;
	}

	if (enemy->trapped > 0)
	{
		enemy->trapped--;
	}

	updateCharacter(enemy->status);
	repositionCharacter(enemy->status);
	if (enemy->forceX != 0)
	{
		if (enemy->forceX > 0)
		{
			enemy->forceX -= ENEMY_BOUNCE_SPEED;
			enemy->forceX = max(0, enemy->forceX);
			moveEnemy(enemy, ENEMY_BOUNCE_SPEED, 0);
		}
		else
		{
			enemy->forceX += ENEMY_BOUNCE_SPEED;
			enemy->forceX = min(0, enemy->forceX);
			moveEnemy(enemy, -ENEMY_BOUNCE_SPEED, 0);
		}
	}

	if (enemy->forceY != 0)
	{
		if (enemy->forceY > 0)
		{
			enemy->forceY -= ENEMY_BOUNCE_SPEED;
			enemy->forceY = max(0, enemy->forceY);
			moveEnemy(enemy, 0, ENEMY_BOUNCE_SPEED);
		}
		else
		{
			enemy->forceY += ENEMY_BOUNCE_SPEED;
			enemy->forceY = min(0, enemy->forceY);
			moveEnemy(enemy, 0, -ENEMY_BOUNCE_SPEED);
		}
	}

	return enemy->enemyType->updateFunction(enemy, ENEMY_UPDATE);
}

bool moveEnemy(Enemy * enemy, int deltaX, int deltaY)
{
	return moveEnemyCanBlock(enemy, deltaX, deltaY, false);
}

bool moveEnemyFast(Enemy * enemy, int deltaX, int deltaY)
{
	enemy->status->x += deltaX;
	enemy->status->y += deltaY;
	
	return true;
}

bool moveEnemyCanBlock(Enemy * enemy, int deltaX, int deltaY, bool block)
{

	if (enemy->trapped)
	{
		return false;
	}

	int oldX = enemy->status->x;
	int oldY = enemy->status->y;

	enemy->status->x += deltaX;
	enemy->status->y += deltaY;

	if (block)
	{
		if (checkEnemyCollidesWithAnyEnemy(enemy))
		{
			enemy->status->x = oldX;
			enemy->status->y = oldY;
			return false;
		}
	}

	int tmpx = enemy->status->x;
	int tmpy = enemy->status->y;

	if (checkWallCollision(enemy->status))
	{
		enemy->status->y = oldY;
		if (checkWallCollision(enemy->status))
		{
			enemy->status->x = oldX;
		}
	}

	int i;
	if (enemy->status->mobility < MOBILITY_GHOST)
	{
		for (i = 0; i < numEnemyBlockers; i++)
		{
			//don't let an enemy block itself
			if (enemy->status != blockers[i]->status)
			{
				if (charactersCollide(enemy->status, blockers[i]->status))
				{
					enemy->status->y = oldY;
					if (charactersCollide(enemy->status, blockers[i]->status))
					{
						enemy->status->x = oldX;
					}
				}
			}
		}
	}
	
	if (enemy->status->mobility < MOBILITY_GHOST)
	{
		enemy->status->x = max(enemy->status->x, 16);
		enemy->status->y = max(enemy->status->y, 16);
		enemy->status->x = min(enemy->status->x, getLevelXBuffer()); 
		enemy->status->y = min(enemy->status->y, getLevelYBuffer());
	}

	if ((enemy->status->y == tmpy) &&(enemy->status->x == tmpx))
	{
		return true;
	
	}

	return false;

}

void initEnemyGfx(Enemy * enemy, u16 * gfxData, characterAnimDef * firstAnimFrame)
{
	enemy->status->gfxBlock = loadSpriteGfxBlock(gfxData);
	loadAndSetPalette(enemy->status, PUN(u16*)enemy->enemyType->palette);
	setCharacterAnimationForced(enemy->status, firstAnimFrame);
}

bool somethingCollidesWithOneEnemy(Enemy * enemy, int x, int y, int x2, int y2, bool isMC)
{
	if (!enemy->active)
	{
		return false;
	}

	int leftX =  enemy->status->x + enemy->status->boundingBlockOffsetX;

	if (x2 < leftX)
	{
		return false;
	}

	int rightX = leftX + enemy->status->boundingBlockWidth;

	if (x > rightX)
	{
		return false;
	}
	int topY =  enemy->status->y + enemy->status->boundingBlockOffsetY;

	if (y2 < topY)
	{
		return false;
	}

	int bottomY = topY + enemy->status->boundingBlockHeight;

	if (y > bottomY)
	{
		return false;
	}

	if (isMC)
	{
		if (enemy->enemyType->updateFunction(enemy, ENEMY_MC_COLLIDE) == 0)
		{
			return false;
		
		}
	}
	
	return true;
	
}


bool collidesWithOneEnemy(Enemy * enemy, int x, int y, int x2, int y2)
{
	return somethingCollidesWithOneEnemy(enemy, x, y, x2, y2, true);
}


bool checkEnemyCollidesWithAnyEnemy(Enemy * enemy)
{
	if (enemy->status->mobility == MOBILITY_GHOST)
	{
		return false;
	}

	int i;
	for (i = 0; i <= maxEnemy; i++)
	{
		Enemy * checkEnemy = &(enemyList[i]);
		if ((enemy != checkEnemy) && (checkEnemy->active))
		{
			if (checkEnemy->enemyType->updateFunction(checkEnemy, ENEMY_ENEMY_COLLIDE) != NO_COLLISION)
			{
				if (charactersCollide(enemy->status, checkEnemy->status))
				{
					return true;
				}
			}
		}
	
	}
	return false;
	
}

Enemy * checkCollidesWithEnemy(CharacterStatus * status)
{
	int x = getX1(status);
	int y = getY1(status);
	int x2 = getX2(status);
	int y2 = getY2(status);
	int i;
	Enemy * collision = null;
	for (i = 0; i <= maxEnemy; i++)
	{
		if (collidesWithOneEnemy(&(enemyList[i]), x, y, x2, y2))
		{
			return (Enemy *)(&(enemyList[i]));
	
		}
	}
	return collision;
}

bool checkMCNoDmgCollision(CharacterStatus * status)
{
	int x = getX1(status);
	int y = getY1(status);
	int x2 = getX2(status);
	int y2 = getY2(status);
	int i;
	Enemy * collision = null;
	for (i = 0; i <= maxEnemy; i++)
	{
		if (collidesWithOneEnemy(&(enemyList[i]), x, y, x2, y2))
		{
			collision = (Enemy *)(&(enemyList[i]));
			int collResult = collision->enemyType->updateFunction(collision, ENEMY_MC_COLLIDE);
			if (collResult == ENEMY_COLLIDE_NO_DMG)
			{
				return true;
			}
	
		}
	}
	return false;

	
}


bool enemyTakeHit(Enemy * enemy, BulletDef * bullet)
{
	if (isCharacterImmuneToBullet(enemy->status, bullet))
	{
		return 0;
	}
	if (bulletIsDynamite(bullet)) 
	{
		enemy->enemyType->updateFunction(enemy, ENEMY_DYNAMITED);
	}
	enemy->bulletCheck = bullet;
	int isHit = enemy->enemyType->updateFunction(enemy, ENEMY_BULLET_COLLIDE);
	if ((isHit == BULLET_HIT_DMG) || (isHit == ENEMY_HIT_NOBOUNCE))
	{
		playGameSfx(SFX_ENEMY_HIT);
		enemy->hp -= bullet->power;
		setLastHitEnemy(enemy);
		if (isHit != ENEMY_HIT_NOBOUNCE)
		{
			enemyBounceAwayFrom(enemy, bullet->sourceX, bullet->sourceY);
		}
		enemy->status->faded = ENEMY_FADE_TIME;
		enemy->trapped = 0;
		setCharacterBulletImmunity(enemy->status, bullet);
	}
	if (enemy->hp <= 0)
	{
		enemy->enemyType->updateFunction(enemy, ENEMY_DEAD);
	}

	return (isHit > 0);
}

bool checkAgainstBullet(Enemy * enemy, BulletDef * bullet)
{
	if (somethingCollidesWithOneEnemy(enemy, bullet->x, bullet->y, bullet->x + bullet->width, bullet->y + bullet->height, false))
	{
		return enemyTakeHit(enemy, bullet);
	}
	return false;
}

Enemy * checkBulletCollidesWithEnemy(BulletDef * bullet)
{
	int i;
	for (i = 0; i <= maxEnemy; i++)
	{
		if ((bullet->team == TEAM_HERO) && checkAgainstBullet(&(enemyList[i]), bullet))
		{
			return &(enemyList[i]);
		}
	}
	return null;
}

void enemyBounceAwayFrom(Enemy * enemy, int x, int y)
{
	CharacterStatus * status = enemy->status;
	int cx = centerX(status);
	int cy = centerY(status);

	int dx = cx - x;
	int dy = cy - y;

	if (abs(dx) > abs(dy))
	{
		enemy->forceX += ENEMY_BOUNCE_DISTANCE * unitVal(dx);
	}
	else
	{
		enemy->forceY += ENEMY_BOUNCE_DISTANCE * unitVal(dy);
	}
}


int getRandomDrop(int dropType)
{
	int tmp;
	switch(dropType)
	{
		case ITEM_RANDOM_1:
			tmp = dice(0,10);
			switch (tmp)
			{
				case 0:
					if (didBuyDynamite()) 
					{
						return ITEM_DYNAMITE;
					}
					break;
				case 1:
					if (dice(0,100) == 2)
					{
						return ITEM_POTION;
					}
					break;
				case 2:
				case 5:
					return ITEM_GOLD;
				case 3:
					return ITEM_MEAT;
				case 4:
					if (getItemQty(ITEM_BOW))
					{
						return ITEM_ARROWS;
					}
					break;
				case 6:
					return ITEM_SMALL_BAG;
					break;
				case 7:
					if (didBuyTraps())
					{
						return ITEM_CALTROPS;
					}
					break;
			}
			return ITEM_NONE;

		default:
			return ITEM_NONE;
	}
}

extern const unsigned short spiderPal[];
extern const EnemyType enemy_death_def;
extern const EnemyType enemy_map_object_def;

int killEnemy(Enemy * enemy)
{
	if (enemy->enemyType->databaseDef != null) {
		addEnemyToDatabase(enemy->enemyType);
	}
	int saver = enemy->itemDrop;
	if (DOES_ENEMY_SAVE(saver))
	{
		setGameState(ENEMY_WHICH_SAVE_SLOT(saver), true);
	}
	int itemDrop = enemy->itemDrop & ITEM_DROP_MASK;
	if (IS_RANDOM(itemDrop))
	{
		itemDrop = getRandomDrop(itemDrop);
	}
	else
	{
		itemDrop = GET_ITEM(itemDrop);
	}
	releaseMultipleSprites(enemy->status->spriteList, enemy->status->listSize);
	short * pal = enemy->enemyType->palette;

	//special case for bushes to not bleed red...
	if (enemy->enemyType == &enemy_map_object_def)
	{
		if ((enemy->special == OBJ_BUSH) || (enemy->special == OBJ_CACTUS))
		{
			pal = &(spiderPal[0]);
		}
	}
	createNewEnemyWithSlot((EnemyType *)&(enemy_death_def), centerX(enemy->status), centerY(enemy->status),
						enemy, enemy->status, itemDrop, 0);
	Sprite * explosionSprite = enemy->status->spriteList[0];
	setSpritePalette(explosionSprite, loadPalette((u16*)pal));

	enemy->status->faded = 1;

	return 1;
}


int numEnemiesAlive()
{
	return Enemies_NumAlive;
}

bool isActualEnemy(Enemy * enemy)
{
	return (enemy != null && enemy->active && enemy->enemyType->isEnemy);
}

int findActualTileFromEnemyTile(Enemy * enemy, int tile)
{
	return tile + spriteGfxBlockFirstTileNum(enemy->status->gfxBlock);
}

BulletDef * fireEnemyBullet(Enemy * enemy, int tile, int sourceOffX, int sourceOffY, int xd, int yd)
{
	int sourceX = enemy->status->x + sourceOffX;
	int sourceY = enemy->status->y + sourceOffY;

	BulletDef bullet;
	bullet.team = TEAM_ENEMY;
	bullet.tile = findActualTileFromEnemyTile(enemy, tile);
	bullet.ttl = BULLET_INFINITE_TTL;
	bullet.xSpeed = xd;
	bullet.ySpeed = yd;
	bullet.xFrameskip = 0;
	bullet.yFrameskip = 1;
	bullet.pierce = 0;
	bullet.power = enemy->enemyType->collideDamage;
	bullet.x = sourceX;
	bullet.y = sourceY;
	bullet.width = 8;
	bullet.height = 8;
	bullet.sourceX = sourceX;
	bullet.sourceY = sourceY;
	bullet.sprite = null;

	BulletDef * createdBullet;
	createdBullet = newBullet((BulletDef*)&bullet);	
	if (createdBullet == null)
	{
		return null;
	}
	PaletteId pal = findPalette((u16*)enemy->enemyType->palette); 
	setSpritePalette(createdBullet->sprite, pal);
	updateOneBulletDef(createdBullet);


	return createdBullet;
}

bool enemyStateAdvance(Enemy * enemy, int timeout, int nextState)
{
	if (enemy->state1 >= timeout)
	{
		enemy->state1 = 0;
		enemy->state2 = nextState;
		return true;
	}

	return false;
	
}

SignedPair getMovementFromEnemyDirection(int direction)
{
	s16 yd = 0;
	s16 xd = 0;
	switch(direction)
	{
		case ENEMY_DIR_N:
			yd = -1;
			break;
		case ENEMY_DIR_S:
			yd = 1;
			break;
		case ENEMY_DIR_E:
			xd = 1;
			break;
		case ENEMY_DIR_W:
			xd =-1;
			break;
	}
	
	return signedPair(xd, yd);
}

bool moveEnemyDirection(Enemy * enemy, int direction)
{
	SignedPair movement = getMovementFromEnemyDirection(direction);
	return moveEnemy(enemy, signedPairX(movement), signedPairY(movement));
	
}
bool moveEnemyDirection(Enemy * enemy, int direction);
