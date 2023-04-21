#include "mainChar.h"
#include "spriteMgr.h"
#include "level.h"
#include "characters.h"
#include "text.h"
#include "bullet.h"
#include "misc.h"
#include "sound.h"
#include "item.h"
#include "input.h"
#include "enemies/itemGfx.h"
#include "enemies/enemyDeathGfx.h"
#include "gfx/mainCharGfx.h"
#include "subscreen.h"
#include "splash.h"
#include "items.h"
#include "enemies.h"
#include "darkness.h"
#include "gameState.h"
#include "config.h"

//#define SIDESCROLL

//extern const unsigned short mc_downTiles[];
//extern const unsigned short mc_leftTiles[];
//extern const unsigned short mc_upTiles[];

#define HORIZ_SPEED 1
#define VERT_SPEED 0

#define MC_MAX_NECESSARY_SPRITES 4

#define MC_STATE_STANDING 0
#define MC_STATE_WALK_LEFT 1
#define MC_STATE_WALK_RIGHT 2
#define MC_STATE_WALK_UP 4
#define MC_STATE_WALK_DOWN 8
#define MC_STATE_ITEM 16
#define MC_STATE_ATTACKING 32

#define MC_FACE_LEFT 0
#define MC_FACE_RIGHT 1
#define MC_FACE_UP 2
#define MC_FACE_DOWN 3

#define MC_ATTACK_STATE_TIME 17
#define MC_ATTACK_STATE_TRIGGER (MC_ATTACK_STATE_TIME-1)
#define MC_ATTACK_CREATE_BULLET_FRAME (MC_ATTACK_STATE_TIME - 5)

#define MC_FADE_TIME 30
#define MC_HEALTH_BAR_START_TILE 165

#define MC_BOUNCE_DISTANCE 32
#define MC_BOUNCE_SPEED 4

#define MC_BULLET_INIT_R_X 20
#define MC_BULLET_INIT_R_Y (8-12)
#define MC_BULLET_INIT_L_X -16
#define MC_BULLET_INIT_L_Y (8-12)
#define MC_BULLET_INIT_U_X 6
#define MC_BULLET_INIT_U_Y (-8-12)
#define MC_BULLET_INIT_D_X 6
#define MC_BULLET_INIT_D_Y (29-12)

#ifndef MC_START_HP
#define MC_START_HP 24
#endif

#ifndef MC_START_POWER
#define MC_START_POWER 1
#endif


#ifndef MC_START_ARMOR
#define MC_START_ARMOR 0
#endif


#ifndef START_ITEM_NBR
#define START_ITEM_NBR 0
#define START_ITEMS {};
#define START_ITEM_QTY {};
#endif

#define LIFE_UPS 11 
#define ARMOR_UPS 5 
#define ATTACK_UPS 6 


#define MC_SCREEN_STYLE_SCROLL_EDGE	60 //screen scrolls when character is X pixels from edge
//other future options could be:
//MC_SCREEN_STYLE_SCROLL_TIGHT  //character is always centered
//MC_SCREEN_STYLE_NONE			//screen does not scroll w/ character


extern const u32 mainCharGfxData[];

extern const characterAnimDef * mainCharAnimWalkN;
extern const characterAnimDef * mainCharAnimWalkS;
extern const characterAnimDef * mainCharAnimWalkE;
extern const characterAnimDef * mainCharAnimWalkW;
extern const characterAnimDef * mainCharAnimAttackW;
extern const characterAnimDef * mainCharAnimItemN;
extern const characterAnimDef * mainCharAnimItemS;
extern const characterAnimDef * mainCharAnimItemE;
extern const characterAnimDef * mainCharAnimItemW;


u16 * gfxToReload;

Sprite * mcSprite[MC_MAX_NECESSARY_SPRITES];
Sprite * currentItemSprite;

u16 mcState;
u16 mcLastState;
int mcStateTimer;
u16 mcControlState;
int mcAttackTimer;
u16 mcFacing;

int mcForceX;
int mcForceY;
int mcAllowAnyMovementAnim;
int stealButtons;

int mcX;
int mcY;
int mcHP;
int mcHPMax;
int mcAttackBonus;
//int mcInventory[NUM_ITEMS];

int lastX;
int lastY;

int mcPower;
int mcArmor;

int mcGold;

int mcXDir;
int mcYDir;

CharacterStatus mcStatusMem;

CharacterStatus * mcStatus = &mcStatusMem;


void mcMoveUp();
bool mcMoveDown();
void mcMoveLeft();
void mcMoveRight();
void mcAttack();

void mcStartJump();
void mcJump();

void processMcMovement();

int mcJumpCtr;
int mcFaded;

bool onGround;



void initMainChar()
{
	int i;

	for (i = 0; i < MC_MAX_NECESSARY_SPRITES; i++)
	{
		mcSprite[i] = null;
	}
	getMultipleSpritesWithOwner(mcSprite, MC_MAX_NECESSARY_SPRITES, mcStatus);
	//currentItemSprite = getSprite(null);
	//initSprite(currentItemSprite);
	//setSpriteShape(currentItemSprite, SQUARE);
	//setSpriteSize(currentItemSprite, SIZE_16);
	//setSpriteStartTile(currentItemSprite, 252 + (5 * 4));
	//setSpritePosition(currentItemSprite, 64,3);

	setMainCharPos(20,20);
	mcStatus->x = T(8);
	mcStatus->y = T(2);
	mcStatus->spriteList = mcSprite;
	mcStatus->listSize = MC_MAX_NECESSARY_SPRITES;

	mcStatus->boundingBlockHeight = 10;
	mcStatus->boundingBlockWidth = 15;
	mcStatus->boundingBlockOffsetX = -4;
	mcStatus->boundingBlockOffsetY = 0;

	mcStatus->mobility = MOBILITY_NORMAL;
	mcStatus->priority = CHAR_PRIORITY_NORMAL;

	forceLoadSpriteGfxBlock((u16*)mc_downTiles, 0);
	//forceLoadSpriteGfxBlock((u16*)mc_downTiles + 2048, 1);
	forceLoadSpriteGfxBlock((u16*)itemsTiles, 1);
	forceLoadSpriteGfxBlock((u16*)enemyDeathGfxTiles, 2);
	forceLoadPalette((u16*)mc_Pal, 0);
	setMaxLockedGfxBlock(2);
	setMaxLockedPalette(2);

	loadAndSetPalette(mcStatus, (u16*)mc_Pal);

	gfxToReload = null;

	mcHP = MC_START_HP;
	mcHPMax = MC_START_HP;
	mcFaded = 0;
	mcStatus->faded = 0;
	mcForceX = 0;
	mcForceY = 0;

	mcPower = MC_START_POWER;
	mcArmor = MC_START_ARMOR;
	mcGold = 0;
	mcAttackBonus = 0;

	setCharacterAnimationForced(mcStatus, (characterAnimDef*)&(mainCharAnimWalkS[0]));
	mcStateTimer = 0;
	mcAttackTimer = 0;
	mcFacing = MC_FACE_DOWN;
	mcState = MC_STATE_STANDING;
	mcLastState = MC_STATE_STANDING;
	mcJumpCtr = 0;
	mcControlState = 0;

	clearInventory();
	int startItems[] = START_ITEMS;
	int startQty[] = START_ITEM_QTY;

	for (i = 0; i < START_ITEM_NBR; i++)
	{
		mainCharAddInventory(startItems[i], startQty[i]);
	}

	
	
}

void releaseMainCharSprites()
{
	releaseMultipleSprites(mcSprite, MC_MAX_NECESSARY_SPRITES); 
}


void mcUnstick()
{
	while (checkWallCollision(mcStatus))
	{
		mcStatus->x++;
		mcStatus->y++;
	}
}


void bounceAwayFrom(int x, int y)
{
	int cx = centerX(mcStatus);
	int cy = centerY(mcStatus);

	int dx = cx - x;
	int dy = cy - y;

	if (abs(dx) > abs(dy))
	{
		mcForceX += MC_BOUNCE_DISTANCE * unitVal(dx);
	}
	else
	{
		mcForceY += MC_BOUNCE_DISTANCE * unitVal(dy);
	}
	
}

void ensureMCOnOwnerList()
{
	addToOwnerList(mcStatus);
}


void updateMainChar()
{
	
	mcStatus->gfxBlock = 0;
	itemCoolDownTick();
	

	if (mcHP < 1)
	{
		doGameOver();
		updateSaveToRespawn();
	}
	
	mcState = MC_STATE_STANDING;
	if (mcAttackTimer > 0)
	{
		mcState |= MC_STATE_ATTACKING;
	}
	if (!(mcState & MC_STATE_ATTACKING))	
	{
		if (mcControlState & MC_STATE_WALK_LEFT)
		{
			//check to see if can start walking left...

			//stop walking right
			mcState &= ~MC_STATE_WALK_RIGHT;
			
			//start walking left
			mcState |= MC_STATE_WALK_LEFT;
		}
		if (mcControlState & MC_STATE_WALK_RIGHT)
		{
			//check to see if can start walking right...

			//stop walking left
			mcState &= ~MC_STATE_WALK_LEFT;
			
			//start walking right
			mcState |= MC_STATE_WALK_RIGHT;
		}
		if (mcControlState & MC_STATE_WALK_UP)
		{
			//check to see if can start walking up...

			//stop walking down
			mcState &= ~MC_STATE_WALK_DOWN;
			
			//start walking up
			mcState |= MC_STATE_WALK_UP;
		}
		if (mcControlState & MC_STATE_WALK_DOWN)
		{
			//check to see if can start walking up...

			//stop walking up
			mcState &= ~MC_STATE_WALK_UP;
			
			//start walking down
			mcState |= MC_STATE_WALK_DOWN;
		}
		if (mcControlState & MC_STATE_ATTACKING)
		{
			//check to see if we can start making an attack now
			{
				mcState |= MC_STATE_ATTACKING;
				mcAttackTimer = MC_ATTACK_STATE_TIME;
				playGameSfx(SFX_SWORD);
				//mcAttack();
			}
		}
		if (mcControlState & MC_STATE_ITEM)
		{
			//if (onGround)
			//{
				//mcStartJump();
			//}
			if (useCurrentItem())
			{
				switch (mcFacing)
				{
					case MC_FACE_UP:
						setCharacterAnimationForced(mcStatus, (characterAnimDef*)mainCharAnimItemN);
						break;
					case MC_FACE_DOWN:
						setCharacterAnimationForced(mcStatus, (characterAnimDef*)mainCharAnimItemS);
						break;
					case MC_FACE_RIGHT:
						setCharacterAnimationForced(mcStatus, (characterAnimDef*)mainCharAnimItemE);
						break;
					case MC_FACE_LEFT:
						setCharacterAnimationForced(mcStatus, (characterAnimDef*)mainCharAnimItemW);
						break;
				}
				mcState = MC_STATE_ITEM;
			}
		}
	}

	if ((mcState == MC_STATE_STANDING) && (mcLastState == MC_STATE_ITEM))
	{
		mcState = MC_STATE_ITEM;	
	}


	lastX = mcStatus->x;
	lastY = mcStatus->y;

	//updateMCDisplay();


	int setVert = false;
	if (mcState & MC_STATE_WALK_UP)
	{
		mcMoveUp();
		//mcFacing = MC_FACE_UP;
		setVert = true;
	}
	else
	if (mcState & MC_STATE_WALK_DOWN)
	{
		mcMoveDown();
		//mcFacing = MC_FACE_DOWN;
		setVert = true;
	}

	if (mcState & MC_STATE_WALK_LEFT)
	{
		mcMoveLeft();
		if (!setVert)
		{
			//mcFacing = MC_FACE_LEFT;
		}
	}
	else
	if (mcState & MC_STATE_WALK_RIGHT)
	{
		mcMoveRight();	
		if (!setVert)
		{
			//mcFacing = MC_FACE_RIGHT;
		}
	}

	if (mcState & MC_STATE_ATTACKING)
	{
		mcAttackTimer--;
		if (mcAttackTimer == MC_ATTACK_CREATE_BULLET_FRAME)
		{
			mcAttack();
		}
		if (mcAttackTimer == 0)
		{
			mcState &= (~MC_STATE_ATTACKING);
			mcAllowAnyMovementAnim = 2;
		}
	}

#ifdef SIDESCROLL
	if (mcMoveDown())
	if (mcMoveDown())
	mcMoveDown();

	mcJump();
#endif
	int ctr;
	
	if (mcForceX != 0)
	{
		if (mcForceX > 0)
		{
			mcForceX -= MC_BOUNCE_SPEED;
			mcForceX = max(0, mcForceX);
			int stopBounceAt = ((getLevelWidth() << 3) - 8);
			for (ctr = 0; ctr < MC_BOUNCE_SPEED; ctr++)
			{
				mcMoveRight();
				//if all the way to the right, stop the bounce
				if (mcStatus->x > stopBounceAt)
				{
					ctr = MC_BOUNCE_SPEED;
					mcForceX = 0;
				}
			}
		}
		else
		{
			mcForceX += MC_BOUNCE_SPEED;
			mcForceX = min(0, mcForceX);
			for (ctr = 0; ctr < MC_BOUNCE_SPEED; ctr++)
			{
				mcMoveLeft();
				//if all the way to the right, stop the bounce
				if (mcStatus->x < 8)
				{
					ctr = MC_BOUNCE_SPEED;
					mcForceX = 0;
				}
			}
		}
	}

	if (mcForceY != 0)
	{
		if (mcForceY > 0)
		{
			mcForceY -= MC_BOUNCE_SPEED;
			mcForceY = max(0, mcForceY);
			int stopBounceAt = ((getLevelHeight() << 3) - 8);
			for (ctr = 0; ctr < MC_BOUNCE_SPEED; ctr++)
			{
				mcMoveDown();
				//if all the way to the bottom, stop the bounce
				if (mcStatus->y > stopBounceAt)
				{
					ctr = MC_BOUNCE_SPEED;
					mcForceY = 0;
				}
			}
		}
		else
		{
			mcForceY += MC_BOUNCE_SPEED;
			mcForceY = min(0, mcForceY);
			for (ctr = 0; ctr < MC_BOUNCE_SPEED; ctr++)
			{
				mcMoveUp();
				//if all the way to the top, stop the bounce
				if (mcStatus->y < 8)
				{
					ctr = MC_BOUNCE_SPEED;
					mcForceY = 0;
				}
			}
		}
	}

	processMcMovement();

	
	//Enemy * collider = checkCollidesWithEnemy(getX1(mcStatus), getY1(mcStatus), getX2(mcStatus), getY2(mcStatus));
	
	Enemy * collider = checkCollidesWithEnemy(mcStatus);
	if (collider != null)
	{
		int doesDmg = collider->enemyType->updateFunction(collider, ENEMY_MC_COLLIDE);

		if (doesDmg == 1)
		{
			int dmg = collider->enemyType->collideDamage;
			mcTakeHit(dmg, centerX(collider->status), centerY(collider->status));
		}
		else if (doesDmg == ENEMY_COLLIDE_NO_DMG)
		{
			//here do a smart collision
			int tmpy = mcStatus->y;
			mcStatus->y = lastY;

			if (charactersCollide(mcStatus, collider->status))
			{
				mcStatus->y = tmpy;
				mcStatus->x = lastX;

				if (charactersCollide(mcStatus, collider->status))
				{
					mcStatus->y = lastY;
				}
	
			}
	
		}
	}
	
	
	if (mcFaded > 0)
	{
		mcFaded--;
	}

	updateCharacter(mcStatus);
	repositionCharacter(mcStatus);

	updateMCDisplay();

	mcLastState = mcState;

	checkDoors(mcStatus);
	
}


void processControls()
{
	mcControlState = MC_STATE_STANDING;
	if (isKeyDown(KEY_LEFT))
	{
		mcControlState |= MC_STATE_WALK_LEFT;
	}
	if (isKeyDown(KEY_RIGHT))
	{
		mcControlState |= MC_STATE_WALK_RIGHT;
	}
	if (isKeyDown(KEY_UP))
	{
		mcControlState |= MC_STATE_WALK_UP;
	}
	if (isKeyDown(KEY_DOWN))
	{
		mcControlState |= MC_STATE_WALK_DOWN;
	}
	if (isKeyPress(KEY_A))
	{
		if (!checkAndClearStealButtons())
			mcControlState |= MC_STATE_ATTACKING;
	}
	if (isKeyPress( KEY_B))
	{
		if (!checkAndClearStealButtons())
			mcControlState |= MC_STATE_ITEM;
	}
	if (isKeyPress( KEY_START))
	{
		if (!checkAndClearStealButtons())
			pauseGame();	
	}
	if (isKeyPress( KEY_SELECT))
	{
		if (!checkAndClearStealButtons())
			showSaveScreen();
	}
	
	if (isKeyPress(KEY_L))
	{
		playGameSfx(SFX_SHOW_SUBSCREEN);
		previousSubscreen();
	}
	if (isKeyPress(KEY_R))
	{
		playGameSfx(SFX_SHOW_SUBSCREEN);
		nextSubscreen();
	}
	if (isKeyPress( KEY_Y))
	{
		if (selectPrevItem())
		{
			playGameSfx(SFX_CHANGE_ITEM);	
		}
	}
	if (isKeyPress( KEY_X))
	{
		if (selectNextItem())
		{
			playGameSfx(SFX_CHANGE_ITEM);
		}
	}
	
}


void setMainCharPos(int x, int y)
{
	mcX = x;
	mcY = y;
}

void reloadMCGfx()
{
	if (gfxToReload == null)
	{
		return;
	}

	loadSpriteGfx(gfxToReload, 0, 128);

	gfxToReload = null;

	
}

int updateMCDisplay()
{
	//set animation for the current state
	//this is all display code...
	if (mcState == MC_STATE_STANDING)
	{
		switch (mcFacing)
		{
			case MC_FACE_UP:
			case MC_FACE_DOWN:
				stopAnimation(mcStatus);
				break;
			case MC_FACE_RIGHT:
				setCharacterAnimation(mcStatus, (characterAnimDef*)mainCharAnimRestR);
				break;
			case MC_FACE_LEFT:
				setCharacterAnimation(mcStatus, (characterAnimDef*)mainCharAnimRest);
				break;
		}
		
	}

	if ((mcState & MC_STATE_WALK_LEFT) && 
				(
				   ((!(mcLastState & MC_STATE_WALK_UP)) && 			//not going diagonal
				   (!(mcLastState & MC_STATE_WALK_DOWN)))

				  ||

				   mcAllowAnyMovementAnim						//or just starting after attack
				)
		)
	{
		mcFacing = MC_FACE_LEFT;
		if (setCharacterAnimation(mcStatus, (characterAnimDef*)mainCharAnimWalkW))
			gfxToReload = (u16*)mc_leftTiles;
	}
	else
	if ((mcState & MC_STATE_WALK_RIGHT) && 
				(
				   ((!(mcLastState & MC_STATE_WALK_UP)) && 			//not going diagonal
				   (!(mcLastState & MC_STATE_WALK_DOWN)))

				  ||

				   mcAllowAnyMovementAnim						//or just starting after attack
				)
		)
	{
		mcFacing = MC_FACE_RIGHT;
		if (setCharacterAnimation(mcStatus, (characterAnimDef*)mainCharAnimWalkE))
			gfxToReload = (u16*)mc_leftTiles;
	}
	else
	if ((mcState & MC_STATE_WALK_UP) && 
				(!(mcLastState & MC_STATE_WALK_LEFT)) && 
				(!(mcLastState & MC_STATE_WALK_RIGHT)) 
		)
	{
		mcFacing = MC_FACE_UP;
		if (setCharacterAnimation(mcStatus, (characterAnimDef*)mainCharAnimWalkN))
			gfxToReload = (u16*)mc_upTiles;
	}
	else
	if ((mcState & MC_STATE_WALK_DOWN) && 
				(!(mcLastState & MC_STATE_WALK_LEFT)) && 
				(!(mcLastState & MC_STATE_WALK_RIGHT)) 
		)
	{

		mcFacing = MC_FACE_DOWN;
		if (setCharacterAnimation(mcStatus, (characterAnimDef*)mainCharAnimWalkS))
			gfxToReload = (u16*)mc_downTiles;
	}

	if (mcAllowAnyMovementAnim > 0)
	{
		mcAllowAnyMovementAnim--;	
	}
	
	if (mcState & MC_STATE_ATTACKING)
	{
		if (mcAttackTimer == MC_ATTACK_STATE_TRIGGER)
		{
			switch (mcFacing)
			{
				case MC_FACE_DOWN:
					setCharacterAnimationForced(mcStatus, (characterAnimDef*)mainCharAnimAttackS);
					break;
				case MC_FACE_UP:
					setCharacterAnimationForced(mcStatus, (characterAnimDef*)mainCharAnimAttackN);
					break;
				case MC_FACE_LEFT:
					setCharacterAnimationForced(mcStatus, (characterAnimDef*)mainCharAnimAttackW);
					break;
				case MC_FACE_RIGHT:
					setCharacterAnimationForced(mcStatus, (characterAnimDef*)mainCharAnimAttackE);
					break;
			}
		}
	
	}

	return 0;
	
}

void mcMoveUp()
{
	mcYDir--;
}

bool mcMoveDown()
{
	mcYDir++;
	return true;
}

void mcMoveLeft()
{
	mcXDir--;
}

void mcMoveRight()
{
	mcXDir++;
}

bool checkMCBlockerCollision()
{
	//here check to see if the mainchar ran into a blocker enemy
	return checkMCNoDmgCollision(mcStatus);
}

void processMcSlideForVert(int dy, int wallCollide)
{
	int move = 0;
	if (dy < 0)
	{
		if((wallCollide & COLLIDE_TL) && (!(wallCollide & COLLIDE_TR)))
		{
			move = 1;
		}
		else
		if((wallCollide & COLLIDE_TR) && (!(wallCollide & COLLIDE_TL)))
		{
			move = -1;
		}
	}
	else
	{
		if((wallCollide & COLLIDE_BL) && (!(wallCollide & COLLIDE_BR)))
		{
			move = 1;
		}
		else
		if((wallCollide & COLLIDE_BR) && (!(wallCollide & COLLIDE_BL)))
		{
			move = -1;
		}
	}

	if (move == 1)
	{
		int left = getX1(mcStatus);
		int offset = left - ((left >> 4)<<4);
		if ((offset > 11) && (offset < 16))
		{
			mcXDir++;
		}
	}
	else if (move == -1)
	{
		int right = getX2(mcStatus);
		int offset = right - ((right >> 4)<<4);
		if ((offset < 6) && (offset >= 0))
		{
			mcXDir--;
		}
	}
	
}

void processMcSlideForHoriz(int dx, int wallCollide)
{
	int move = 0;
	if (dx < 0)
	{
		if((wallCollide & COLLIDE_TL) && (!(wallCollide & COLLIDE_BL)))
		{
			move = 1;
		}
		else
		if((wallCollide & COLLIDE_BL) && (!(wallCollide & COLLIDE_TL)))
		{
			move = -1;
		}
	}
	else
	{
		if((wallCollide & COLLIDE_TR) && (!(wallCollide & COLLIDE_BR)))
		{
			move = 1;
		}
		else
		if((wallCollide & COLLIDE_BR) && (!(wallCollide & COLLIDE_TR)))
		{
			move = -1;
		}
	}

	if (move == 1)
	{
		int left = getY1(mcStatus);
		int offset = left - ((left >> 4)<<4);
		if ((offset > 9) && (offset < 16))
		{
			mcYDir++;
		}
	}
	else if (move == -1)
	{
		int right = getY2(mcStatus);
		int offset = right - ((right >> 4)<<4);
		if ((offset < 8) && (offset >= 0))
		{
			mcYDir--;
		}
	}
	
}

void mcProcessMoveVert(int dy)
{
	int oldy = mcStatus->y;
	int i;

	mcStatus->y += dy;

	int wallCollide = checkWallCollision(mcStatus);

	bool enemyBlocked = checkMCBlockerCollision();

	if ((wallCollide) || (enemyBlocked))
	{
		mcStatus->y = oldy;

		if (!enemyBlocked)
		{
			processMcSlideForVert(dy, wallCollide);
		}

	}
	else
	{
		if (dy < 0)
		{
			for (i = 0; i < -dy; i++)
			{
				if (mcStatus->screenY < MC_SCREEN_STYLE_SCROLL_EDGE)
				{
					scrollWorldY(-1);	
				}
			}
		}
		else
		{
			for (i = 0; i < dy; i++)
			{
				if (mcStatus->screenY > SCREEN_HEIGHT - MC_SCREEN_STYLE_SCROLL_EDGE)
				{
					scrollWorldY(1);	
				}
			}
		}
	}
}

void mcProcessMoveHoriz(int dx)
{
	int i;
	int oldx = mcStatus->x;
	mcStatus->x += dx;

	int wallCollide = checkWallCollision(mcStatus);

	bool enemyBlocked = checkMCBlockerCollision();

	if ((wallCollide) || (enemyBlocked))
	{
		mcStatus->x = oldx;

		if (!enemyBlocked)
		{
			processMcSlideForHoriz(dx, wallCollide);
		}
	}
	else
	{
		if (dx < 0)
		{
			for (i = 0; i < -dx; i++)
			{
				if (mcStatus->screenX <  MC_SCREEN_STYLE_SCROLL_EDGE)
				{
					scrollWorldX(-1);	
				}
			}
		}
		else
		{
			for (i = 0; i < dx; i++)
			{
				if (mcStatus->screenX > SCREEN_WIDTH - MC_SCREEN_STYLE_SCROLL_EDGE)
				{
					scrollWorldX(1);	
				}
			}
		}
	}

}


void mcStartJump()
{
	mcJumpCtr = 60;
}

void mcJump()
{
	if (mcJumpCtr < 1) return;
	mcMoveUp();
	if (mcJumpCtr > 30) mcMoveUp();
	if (mcJumpCtr > 40) mcMoveUp();
	if (mcJumpCtr > 50) mcMoveUp();
	mcJumpCtr--;
}

void mcAttack()
{
	BulletDef bullet;

	bullet.tile = 100;
	bullet.ttl = 15;
	bullet.xSpeed = 0;
	bullet.ySpeed = 0;
	bullet.xFrameskip = 0;
	bullet.yFrameskip = 0;
	bullet.team = TEAM_HERO;
	bullet.pierce = true;
	bullet.power = getMainCharPower();
	bullet.sourceX = centerX(mcStatus);
	bullet.sourceY = centerY(mcStatus);
	int xmod = 0;
	int ymod = 0;
	switch (mcFacing)
	{
		case MC_FACE_DOWN:
			xmod = MC_BULLET_INIT_D_X - 4;
			ymod = MC_BULLET_INIT_D_Y - 2;
			break;

		case MC_FACE_UP:
			xmod = MC_BULLET_INIT_U_X - 4;
			ymod = MC_BULLET_INIT_U_Y - 3;
			break;

		case MC_FACE_LEFT:
			xmod = MC_BULLET_INIT_L_X;
			ymod = MC_BULLET_INIT_L_Y;
			break;

		case MC_FACE_RIGHT:
			xmod = MC_BULLET_INIT_R_X;
			ymod = MC_BULLET_INIT_R_Y;
			break;
	}

	bullet.x = mcStatus->x + xmod - 2;
	bullet.y = mcStatus->y + ymod - 2;
	bullet.height = 12;
	bullet.width = 12;
	bullet.sprite = null;

	newBullet(&bullet);


}

int mainCharMeatPickup()
{
	mcAddHP(ITEM_MEAT_HEALTH);
	return 0;
}

int mainCharAttackUp()
{
	mcPower += 1;
	return 0;
}

int mainCharLifeUp()
{
	mcHPMax += 4;
	mcHP = mcHPMax;
	return 0;
}

int mainCharDefenceUp()
{
	mcArmor += 1;
	return 0;
}

void addMainCharGold(int amount)
{
	mcGold += amount;
	if (mcGold > 999)
	{
		mcGold = 999;
	}
	if (mcGold < 0)
	{
		mcGold = 0;
	}
}

void mainCharAddInventory(int item, int qty)
{
	switch(item)
	{
		case ITEM_MEAT:
			mcAddHP(ITEM_MEAT_HEALTH * qty);
			break;
		case ITEM_ATTACK_UP:
			mcPower+=qty;
			break;
		case ITEM_LIFE_UP:
			mcHPMax+=(8*qty);
			break;
		case ITEM_DEFENCE_UP:
			mcArmor+=qty;
			break;
			
		default:
			//mcInventory[item] += qty;
			//mcInventory[item] = max(mcInventory[item], 0);
			adjustInventory(item,qty);
			break;
	}

}
	
int mainCharInventory(int item)	
{
	//return mcInventory[item];
	return getItemQty(item);
}
void returnMainCharToLastGoodPosition(int whichAxis)
{
	if (whichAxis & AXIS_X)
	{
		mcStatus->x = lastX;
	}

	if (whichAxis & AXIS_Y)
	{
		mcStatus->y = lastY;
	}

}

int getMainCharX()
{
	return mcStatus->x;
}

int getMainCharY()
{
	return mcStatus->y;
	
}

void setAttackBonus(int bonus)
{
	mcAttackBonus = bonus;
}


int getMainCharBasePower()
{
	return mcPower;
}

int getMainCharPower()
{
	return mcPower + mcAttackBonus;
}

int getMainCharGold()
{
	return mcGold;
}


int getMainCharMaxHP()
{
	return mcHPMax;
}

int getMainCharArmor()
{
	return mcArmor;
}

int getMainCharHP()
{
	return mcHP;
}

void mcAddHP(int delta)
{
	mcHP += delta;
	mcHP = min(mcHP, mcHPMax);
}

int checkBulletCollidesWithMC(BulletDef * bullet)
{
	if (bullet->team == TEAM_ENEMY)
	{
		int leftX = getX1(mcStatus);
		int x2 = bullet->x + bullet->width;
		if(x2 < leftX)
		{
			return false;
		}

		int rightX = getX2(mcStatus);
		if(bullet->x > rightX)
		{
			return false;
		}

		int topY = getY1(mcStatus);
		int y2 = bullet->y + bullet->height;
		if (y2 < topY)
		{
			return false;
		}

		int bottomY = getY2(mcStatus);
		if (bullet->y > bottomY)
		{
			return false;
		}

		mcTakeHit(bullet->power, bullet->sourceX, bullet->sourceY);
		return true;
	}
	
	return false;
}

void mcTakeHit(int dmg, int srcX, int srcY)
{
	dmg = max(1, dmg - mcArmor);
	if ((mcFaded == 0) && (dmg > 0))
	{
		mcHP -= dmg;
		mcFaded = MC_FADE_TIME;
		mcStatus->faded = MC_FADE_TIME;
		playGameSfx(SFX_TAKE_HIT);
		bounceAwayFrom(srcX, srcY);
	}
}

CharacterStatus * getMainCharStatus()
{
	return mcStatus;
}

void processMcMovement()
{
	if (mcXDir != 0)
	{
		mcProcessMoveHoriz(mcXDir);
	}
	mcXDir = 0;

	if (mcYDir != 0)
	{
		mcProcessMoveVert(mcYDir);
	}
	mcYDir = 0;

	if (mcXDir != 0)
	{
		mcProcessMoveHoriz(mcXDir);
	}
	mcXDir = 0;

	if (mcYDir != 0)
	{
		mcProcessMoveVert(mcYDir);
	}

	mcYDir = 0;
	
}


int mcXFacing()
{
	if (mcFacing == MC_FACE_RIGHT) return 1;
	if (mcFacing == MC_FACE_LEFT) return -1;
	return 0;
}

int mcYFacing()
{
	if (mcFacing == MC_FACE_DOWN) return 1;
	if (mcFacing == MC_FACE_UP) return -1;
	return 0;
	
}

bool isMCAttacking()
{
	return (mcAttackTimer > 0);
	
}

void loadMCFromSave(int hp, int hpMax, int power, int armor, int x, int y, int gold)
{
	 mcHP = hp;
	 mcHPMax = hpMax;
	 mcPower = power;
	 mcArmor = armor;
	 mcStatus->x = x;
	 mcStatus->y = y;
	 mcGold = gold;
}

bool checkCharacterCollidesWithMC(CharacterStatus * character)
{
	return charactersCollide(mcStatus, character);
}

int getPercentComplete()
{
	int attackUps = getMainCharBasePower() - MC_START_POWER;
	int armorUps = getMainCharArmor() - MC_START_ARMOR;
	int lifeUps = (getMainCharMaxHP() - MC_START_HP) >> 2;

	int total = attackUps + armorUps + lifeUps;

	total += mainCharInventory(ITEM_BOW);
	total += mainCharInventory(ITEM_LANTERN);
	total += mainCharInventory(ITEM_BOOTS);
	total += mainCharInventory(ITEM_ATTACK_RING);
	total += mainCharInventory(ITEM_CALTROPS) > 0 ? 1 : 0;

	int outOf = LIFE_UPS + ARMOR_UPS + ATTACK_UPS + 5;

	int percent = total * 100 / outOf;

	return percent;
	
}

void setStealButtons()
{
	stealButtons = true;
}

bool checkAndClearStealButtons()
{
	bool tmp = stealButtons;
	stealButtons = false;
	return tmp;
}

