#ifndef _CHARACTERS_H_
#define _CHARACTERS_H_

#include "misc.h"
#include "spriteMgr.h"
#include "bullet.h"

#define FLIP_V_ONLY 1010
#define FLIP_BOTH 1011
#define FLIP_NONE 0 
#define FLIP_H_ONLY 1
#define TILE_SPECIAL -1

#define CHAR_PRIORITY_MAP 0
#define CHAR_PRIORITY_LOW 1
#define CHAR_PRIORITY_NORMAL 2
#define CHAR_PRIORITY_HIGH 3

#define COLLIDE_WAS_COLLISION (1<<5)

#define COLLIDE_TR (1<<0)
#define COLLIDE_TL (1<<1)
#define COLLIDE_BR (1<<2)
#define COLLIDE_BL (1<<3)

#define FRAME_NEXT (void *)1

enum
{
	MOBILITY_NORMAL,
	MOBILITY_FLYING,
	MOBILITY_GHOST
};

typedef struct
{
	int xOffset;
	int yOffset;
	int tileNum;
	int shape;
	int size;
	int hflip;
	void * nextSprite;
} characterFrameDef;

typedef struct
{
	int frameDelay;
	u16 * gfxBlockSource;
	characterFrameDef * thisFrame;
	void * nextFrame;
} characterAnimDef;

struct CharacterStatusStruct
{
	characterFrameDef * currentFrame;
	characterAnimDef * currentAnim;
	characterAnimDef * animBase;
	SpriteList spriteList;
	SpriteGfxBlockId gfxBlock;
	int listSize;
	int frameCounter;
	int x;
	int y;
	int screenX;
	int screenY;
	int boundingBlockOffsetX;
	int boundingBlockOffsetY;
	int boundingBlockWidth;
	int boundingBlockHeight;
	int faded;
	int special;  			//special...for characters where one tile may be different on 
							//initialization....set the tile to TILE_SPECIAL in the frame definition
							//then set special to the value of the tile in the code....
	int mobility;
	int priority;
	BulletDef * lastBulletHitBy;


};

typedef struct CharacterStatusStruct CharacterStatus;


int getScreenX(int worldX);
int getScreenY(int worldY);
bool isOnScreen(int width, int height, int screenX, int screenY);

void loadAndSetPalette(CharacterStatus * status, u16 * palette);

void setCharacterFrame(CharacterStatus * status, characterFrameDef * frameDef);
void setCharacterAnimationForced(CharacterStatus * status, characterAnimDef * animDef);
bool setCharacterAnimation(CharacterStatus * status, characterAnimDef * animDef);
void updateCharacter(CharacterStatus * status);
void repositionCharacter(CharacterStatus * status);
void stopAnimation(CharacterStatus * status);
void setBoundingBlock(CharacterStatus * status, int w, int h, int x, int y);

//temporary bullet immunity for piercing bullets
void setCharacterBulletImmunityFinished(CharacterStatus * status);
void setCharacterBulletImmunity(CharacterStatus * status, BulletDef * bullet);
bool isCharacterImmuneToBullet(CharacterStatus * status, BulletDef * bullet);

int checkWallCollision(CharacterStatus * status);

bool charactersCollide(CharacterStatus * char1, CharacterStatus * char2);


#define getX1(status) ((status)->x + (status)->boundingBlockOffsetX)
#define getX2(status) (getX1(status) + (status)->boundingBlockWidth)
#define getY1(status) ((status)->y + (status)->boundingBlockOffsetY)
#define getY2(status) (getY1(status) + (status)->boundingBlockHeight)
#define centerX(status) ((getX1(status) + getX2(status)) >> 1)
#define centerY(status) ((getY1(status) + getY2(status)) >> 1)

#endif
