#ifndef _SPRITE_MGR_H_
#define _SPRITE_MGR_H_

#include <nds.h>
#include "platform.h"

struct CharacterStatusStruct;

#define BLANK_TILE 100

#define BIT_DEPTH_256 1
#define BIT_DEPTH_16 2

#define SPRITE_PRIORITY_NORMAL 2
#define SPRITE_PRIORITY_HIGH 1
#define SPRITE_PRIORITY_BG 3
#define SPRITE_PRIORITY_UI 0

typedef Sprite ** SpriteList;

//functions for working on global settings, 
//operating on the global sprite list
void enableSprites();
void disableSprites();
void disableMainSprites();

void debugSpritesInUse();

void initSpriteList();

struct CharacterStatusStruct;

Sprite * getSprite(Sprite * lastPointer);
Sprite * releaseSprite(Sprite * toRelease);
void getMultipleSprites(SpriteList listOfSprites, int numToGet);
void getMultipleSpritesWithOwner(SpriteList listOfSprites, int numToGet, struct CharacterStatusStruct * owner);
Sprite * getSpriteWithOwner(Sprite * lastPointer, struct CharacterStatusStruct * owner);
void releaseMultipleSprites(SpriteList spriteList, int numToRelease);

void loadSpriteGfx(u16 * spriteGfx, int startTile, int numTiles);
void loadSpritePalette(u16 * palette);

void loadSubSpriteGfx(u16 * spriteGfx, int startTile, int numTiles);
void loadSubSpritePaletteLine(u16 * palette, int palLine);

void loadOneSpriteBlock16(u16 * gfxSource, int destTile);
void updateSpriteDisplay();
void clearCustomSprites();
void clearTopCustomSprites();


typedef int SpriteGfxBlockId;
//functions for operating on sprite tile memory swapper
SpriteGfxBlockId loadSpriteGfxBlock(u16 * spriteGfx);    //loads a new gfxBlock
void forceLoadSpriteGfxBlock(u16 * spriteGfx, SpriteGfxBlockId id);  //forcefully loads a gfx block into a specified block
void setMaxLockedGfxBlock(SpriteGfxBlockId id);			//sets locked blocks, for main character, etc
void timeTickForSpriteGfxBlocks();						//time tick for increasing age of blocks
void initSpriteGfxSwapper();
u16 * getOriginOfSpriteGfxBlock(SpriteGfxBlockId id);
SpriteGfxBlockId findSpriteGfxBlock(u16 * spriteGfx);
int spriteGfxBlockFirstTileNum(SpriteGfxBlockId id);

//now for the palette swapper
typedef int PaletteId;
//functions for operating on sprite tile memory swapper
PaletteId loadPalette(u16 * palette);    //loads a new palette
void forceLoadPalette(u16 * palette, PaletteId id);  //forcefully loads a palette block into a specified block
void setMaxLockedPalette(PaletteId id);			//sets locked blocks, for main character, etc
void timeTickForPalette();						//time tick for increasing age of blocks
void initPaletteSwapper();
u16 * getOriginOfPalette(PaletteId id);
PaletteId findPalette(u16 * palette);

//functions to act on an invidividual sprite
void setSpriteStartTile(Sprite * sprite, int tileNum);
void setSpriteShape(Sprite * sprite, int shape);
void setSpritePosition(Sprite * sprite, int x, int y);
void setSpriteFlip(Sprite * sprite, bool flip);
void setSpriteVFlip(Sprite * sprite, bool flip);
void setSpritePriority(Sprite * sprite, int level);
void setSpriteSize(Sprite * sprite, int size);
void setSpriteFaded(Sprite * sprite, bool faded);
void setSpritePalette(Sprite * sprite, PaletteId);
int getSpritePalette(Sprite * sprite);
void setSpriteWindowed(Sprite * sprite, bool windowed);

void setSpriteBitDepth(Sprite * sprite, int BitDepth);


//for special custom sprite work, (subscreens, animations, etc)
//use these instead of the bulky sprite manager
Sprite * specialSprite(int num);
Sprite * specialTopSprite(int num);
void updateSpriteDisplayUsingCustomSprites();
void updateTopSpriteDisplayUsingCustomSprites();
void initSprite(Sprite * result);
void sortSpriteList();
void addToOwnerList(struct CharacterStatusStruct * ownerPtr);
void removeFromOwnerList(struct CharacterStatusStruct * ownerPtr);
void setSpriteNotInUse(Sprite * sprite);

void resetGfxStats();

//typedef u32*(*HudSpecialSpriteFunction)();
/**
 * This function should return a pointer to a sprite that will be used in the hud
 * It should either be null, or return null, if there is no need
 * to draw a special hud sprite.
 */
//void setHudSpecialSpriteFunction(HudSpecialSpriteFunction hudSpecialSpriteFunc);



#endif
