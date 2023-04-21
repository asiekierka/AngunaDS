#include "spriteMgr.h"
#include "misc.h"
#include "memcpy.h"
#include "characters.h"
#include "mainChar.h"
#include "hud.h"

#define NUM_SPECIAL_SPRITES 32
#define NUM_TOP_SPECIAL_SPRITES 3
//#define NUM_SPECIAL_SPRITES NUM_ITEMS
#define SPRITELIST_SIZE 128
#define WORDS_PER_SPRITE 2
#define SPRITELIST_WORDS (WORDS_PER_SPRITE * SPRITELIST_SIZE)

#define HALFWORDS_PER_TILE 16

#define HALFWORDS_PER_TILE_ROW HALFWORDS_PER_TILE >> 3

#define TILES_PER_ROW 32

#define SWAPPER_NUM_SLOTS 8
#define SWAPPER_SLOT_WIDTH 16
#define SWAPPER_SLOT_HEIGHT 4
#define SWAPPER_BLOCK_SIZE (SWAPPER_SLOT_WIDTH * SWAPPER_SLOT_HEIGHT * 64)
#define SWAPPER_TILES_PER_BLOCK (SWAPPER_SLOT_WIDTH * SWAPPER_SLOT_HEIGHT)

#define PAL_SWAPPER_NUM_SLOTS 16
#define PAL_SIZE 16

#define MAX_CHARACTERS

#define OWNERLIST_SIZE 64

#define NO_OWNER 99999

typedef struct {
	CharacterStatus * owner;

} SpriteMetaData;

struct CharacterStatusStruct * ownerList[OWNERLIST_SIZE];
int lastOwner;

Sprite spriteList[SPRITELIST_SIZE];
SpriteMetaData spriteDataList[SPRITELIST_SIZE];
Sprite spriteShadowBuffer[SPRITELIST_SIZE];

int lastSpriteUsed;
Sprite specialSprites[NUM_SPECIAL_SPRITES];
Sprite specialTopSprites[NUM_TOP_SPECIAL_SPRITES];
Sprite blankSprite;

extern CharacterStatus bulletOwner;

void addToOwnerList(CharacterStatus * ownerPtr);
void removeFromOwnerList(CharacterStatus * ownerPtr);

int getOwnerBasedPosition(int whichOwner) {

	if (ownerList[whichOwner] == null) {
		return NO_OWNER;
	}
	return (ownerList[whichOwner])->y * (ownerList[whichOwner]->priority << 2);

}

void enableSprites() {
	//enable sprites, and set to 2D mapping mode
	//why use 2d?  it's just easier for the graphics configuration,
	//I can deal with the slightly harder code
	REG_DISPCNT |= DISPLAY_SPR_ACTIVE | DISPLAY_SPR_2D;
	REG_DISPCNT_SUB |= DISPLAY_SPR_ACTIVE | DISPLAY_SPR_2D;
}

void disableSprites() {
	//turn off the sprites (why would you want to do this?  maybe title screen)
	REG_DISPCNT &= ~DISPLAY_SPR_ACTIVE;
	REG_DISPCNT_SUB &= ~DISPLAY_SPR_ACTIVE;

}

void disableMainSprites() {
	//turn off the sprites (why would you want to do this?  maybe title screen)
	REG_DISPCNT &= ~DISPLAY_SPR_ACTIVE;
}

void setSpritePalette(Sprite * sprite, PaletteId id) {
	sprite->attribute[2] &= ~ATTR2_PALETTE(15);
	sprite->attribute[2] |= ATTR2_PALETTE(id);
}

int getSpritePalette(Sprite * sprite) {
	return sprite->attribute[2] >> 12;
}

void setSpriteNotInUse(Sprite * sprite) {
	//makes the sprite invisible, as well as use for marking available sprites
	sprite->attribute[0] |= ATTR0_ROTSCALE_DOUBLE;
	setSpriteFaded(sprite, false);
	setSpriteStartTile(sprite, BLANK_TILE);
	setSpritePosition(sprite, -16,-16);
}

void setSpriteInUse(Sprite * sprite) {
	//re-enable the sprite
	sprite->attribute[0] &= ~ATTR0_ROTSCALE_DOUBLE;
}

void initSprite(Sprite * result) {
	result->attribute[0] = ATTR0_COLOR_16 | ATTR0_SQUARE;
	result->attribute[1] = ATTR1_SIZE_16;
	result->attribute[2] = 0;

	setSpriteStartTile(result, BLANK_TILE);

	setSpriteInUse(result);
	//huh...why do we clear it and then set it?  I guess it works, 
	//so I'll leave it...
	result->attribute[2] &= ~ATTR2_PRIORITY(SPRITE_PRIORITY_NORMAL);
	result->attribute[2] |= ATTR2_PRIORITY(SPRITE_PRIORITY_NORMAL);
	setSpriteFaded(result, false);
	setSpriteWindowed(result, false);
}

void initSpriteList() {
	//set all sprites not in use
	int i;
	lastSpriteUsed = -1;
	for (i = 0; i < 128; i++) {
		setSpriteNotInUse(&(spriteList[i]));
		setSpriteFaded(&(spriteList[i]), false);
	}
	initSprite(&blankSprite);
	setSpriteNotInUse(&blankSprite);
	clearTopCustomSprites();
	clearCustomSprites();
}

bool spriteInUse(Sprite * sprite) {
	//find out if the sprite is in use
	return !(sprite->attribute[0] & ATTR0_ROTSCALE_DOUBLE);
}

Sprite * getSprite(Sprite * lastPointer) {
	return getSpriteWithOwner(lastPointer, null);
}

Sprite * getSpriteWithOwner(Sprite * lastPointer, CharacterStatus * owner) {
	//we pass in lastPointer here so we can be lazy about getting a sprite.
	//if we aren't sure if the character has one, just go ahead and call it again


	if (lastPointer != null) {
		return lastPointer;
	}

	//start searching from the last used, to save time
	int found = -1;
	int startLastUsed = lastSpriteUsed;
	bool looped = false;
	Sprite * result;
	while (found == -1) {
		//this nastiness is for wrapping around back to the beginning of the list
		//I'm not sure this is worth it, actually 
		lastSpriteUsed++;
		if (lastSpriteUsed == 128) {
			lastSpriteUsed = 0;
			looped = true;
		}
		if ((lastSpriteUsed > startLastUsed) && looped) {
			found = -2;
		}

		//if we find an unused sprite, grab it
		if (!(spriteInUse(&spriteList[lastSpriteUsed]))) {
			found = lastSpriteUsed;
		}
	}
	result = null;

	if (found >= 0) {
		result = &spriteList[found];
		spriteDataList[found].owner = owner;
		addToOwnerList(owner);
	} else {
		ASSERT(false, "ran out of sprites");
		return null;
	}

	//otherwise, go ahead and initialize the sprite
	//so we don't hand it out again it accidentally
	initSprite(result);

	return result;
}

//characters will often need multiple sprites.  
//This lets them get them all in a batch
void getMultipleSprites(SpriteList listOfSprites, int numToGet) {
	getMultipleSpritesWithOwner(listOfSprites, numToGet, null);

}

void getMultipleSpritesWithOwner(SpriteList listOfSprites, int numToGet,
		CharacterStatus * owner) {
	int ctr;

	for (ctr = 0; ctr < numToGet; ctr++) {
		listOfSprites[ctr] = getSpriteWithOwner(listOfSprites[ctr], owner);
	}

}

//and release them all in a batch
void releaseMultipleSprites(SpriteList spriteList, int numToRelease) {
	int ctr;
	if ((numToRelease > 50) || (numToRelease < 0))
	{
		ASSERT(false, "bad num released");
	}
	if (spriteList == null)
	{
		ASSERT(false, "null sprite list");
	}
	for (ctr = 0; ctr < numToRelease; ctr++) {
		spriteList[ctr] = releaseSprite(spriteList[ctr]);
	}
}

//release one sprite if no longer in use
Sprite * releaseSprite(Sprite * toRelease) {
	if (toRelease == null)
	{
		ASSERT(false, "trying to release null sprites");
	}
	setSpriteNotInUse(toRelease);
	return null;
}

//sets the graphic tile for a sprite
void setSpriteStartTile(Sprite * sprite, int tileNum) {
	//clear the start tile, keep upper bits
	sprite->attribute[2] &= 0xFC00;

	//set the new start tile
	sprite->attribute[2] |= (tileNum & 0x3FF);

}

void setSpritePriority(Sprite * sprite, int level) {
	level &= 0x03;

	//clear the last priority
	sprite->attribute[2] &= 0xF3FF;

	//set the new priority
	sprite->attribute[2] |= ATTR2_PRIORITY(level);

}

void setSpriteShape(Sprite * sprite, int shape) {
	//clear the shape
	sprite->attribute[0] &= 0x3FFF;

	//set the shape (assuming it uses a defined constant, so is already bit-shifted)
	sprite->attribute[0] |= shape;

}

void setSpriteSize(Sprite * sprite, int size) {
	//clear the size
	sprite->attribute[1] &= 0x3FFF;

	//set the size (assuming uses a defined constant, so is already bit-shifted)
	sprite->attribute[1] |= size;
}

/**
 * sets the sprite position
 */
void setSpritePosition(Sprite * sprite, int x, int y) {
	//clear the X position
	sprite->attribute[1] &= ~0x01FF;
	//and set it again (mask by 01FF to keep in size, and allow proper wrapping)
	sprite->attribute[1] |= (x & 0x01FF);

	//do the same for Y position
	sprite->attribute[0] &= ~0x00FF;
	sprite->attribute[0] |= (y & 0x00FF);

}

void setSpriteVFlip(Sprite * sprite, bool flip) {
	//either set or clear the vertical flip bit
	if (flip) {
		sprite->attribute[1] |= ATTR1_FLIP_Y;
	} else {
		sprite->attribute[1] &= ~ATTR1_FLIP_Y;
	}

}

void setSpriteWindowed(Sprite * sprite, bool windowed) {
	if (windowed) {
		sprite->attribute[0] |= ATTR0_TYPE_WINDOWED;
	} else {
		sprite->attribute[0] &= ~ATTR0_TYPE_WINDOWED;
	}

}

void setSpriteFaded(Sprite * sprite, bool faded) {
	if (faded) {
		sprite->attribute[0] |= ATTR0_TYPE_BLENDED;
	} else {
		sprite->attribute[0] &= ~ATTR0_TYPE_BLENDED;
	}
}

void setSpriteFlip(Sprite * sprite, bool flip) {
	if (flip) {
		sprite->attribute[1] |= ATTR1_FLIP_X;
	} else {
		sprite->attribute[1] &= ~ATTR1_FLIP_X;
	}

}

void setSpriteBitDepth(Sprite * sprite, int BitDepth) {
	if (BitDepth == BIT_DEPTH_16) {
		sprite->attribute[0] &= ATTR0_COLOR_256;
		sprite->attribute[0] |= ATTR0_COLOR_16;
	} else {
		sprite->attribute[0] &= ATTR0_COLOR_16;
		sprite->attribute[0] |= ATTR0_COLOR_256;
	}

}

void loadSpriteGfx16Color(u16 * spriteGfx, int startTile, int numTiles) {
	loadSpriteGfx(spriteGfx, startTile>>1, numTiles);

}

void loadSpriteGfx(u16 * spriteGfx, int startTile, int numTiles) {
	//if using 256 (8bb) color bitmaps, each bitmap is 64 bytes.  
	//Since we are copying 2 pixels at a time, we use u16's, 
	//there are 32 u16's per tile
	startTile = startTile * 32;

	numTiles = numTiles * HALFWORDS_PER_TILE;

	u16 * spriteGfxData= SPRITE_GFX;
	spriteGfxData = spriteGfxData + startTile;

	copyMem16(spriteGfxData, spriteGfx, numTiles);

}

void loadSubSpriteGfx(u16 * spriteGfx, int startTile, int numTiles) {
	startTile = startTile * 32;

	numTiles = numTiles * HALFWORDS_PER_TILE;

	u16 * spriteGfxData= SPRITE_GFX_SUB;
	spriteGfxData = spriteGfxData + startTile;
	copyMem16(spriteGfxData, spriteGfx, numTiles);
}

void loadSubSpritePaletteLine(u16 * palette, int palLine) {
	u16 * paletteMem = SPRITE_PALETTE_SUB;
	paletteMem  += (palLine * 16);
	copyMem16(paletteMem , palette, PAL_SIZE);
}

void loadOneSpriteBlock16(u16 * gfxSource, int destTile) {
	loadSpriteGfx16Color(gfxSource, destTile, 2);
	loadSpriteGfx16Color(gfxSource + (HALFWORDS_PER_TILE * 32), destTile + 32,
			2);

}

void loadSpritePalette(u16 * palette) {
	int loop;
	u16 * spritePaletteData= SPRITE_PALETTE;
	//this should be replaced by a faster memory copy....
	for (loop = 0; loop < 256; loop++) {
		spritePaletteData[loop] = palette[loop];
	}
}

void copySprite(Sprite * destSprite, Sprite * srcSprite)
{
	u32* dest = (u32*) destSprite;
	u32* src = (u32*) srcSprite;

	dest[0] = src[0];
	dest[1] = src[1];
}


void sortSpriteList() {

	int i;
	int max = 0;
	int positions[OWNERLIST_SIZE];

	//first, get an array of the positions, to sort by
	//(so we don't have to look them up each time, and
	// so we know what the largest one we need to sort)
	for (i = 0; i < OWNERLIST_SIZE; i++) {
		int tmpPos= getOwnerBasedPosition(i);

		positions[i] = tmpPos;

		if (tmpPos != NO_OWNER) {
			max = i;
		}

	}

	//sort the suckers
	shellSortReverse(PUN(void **)ownerList, positions, max + 1);

	//loop through each character, and 
	int shadow = 0;
	for (i = 0; i <= max; i ++) {
		//copy this character's sprites to shadow
		CharacterStatus * chr = ownerList[i];

		if (chr != null) {

			int numSprites = chr->listSize;
			int k;
			for (k = 0; k < numSprites; k++) {
				//spriteShadowBuffer[shadow] = *(chr->spriteList[k]);
				
				if (chr->spriteList[k] == null)
				{
					copySprite(&(spriteShadowBuffer[shadow]), &blankSprite);
				}
				else
				{
					copySprite(&(spriteShadowBuffer[shadow]), chr->spriteList[k]);
				}
				shadow++;
			}
		}

	}

	for (shadow = shadow; shadow < SPRITELIST_SIZE; shadow++) {
		//spriteShadowBuffer[shadow] = blankSprite;
		copySprite(&(spriteShadowBuffer[shadow]), &blankSprite);
	}

	return;

}

//this copies the spriteList info into actual sprite data,
//we do this so sprites don't get updates in the middle of a frame,
//creating tearing or other weird effects.  This function should only
//be called during vblank
void updateSpriteDisplay() {
	//int loop; 
	u32* temp;
	temp = PUN(u32*)spriteShadowBuffer;
	//temp = (u32*)spriteList; 
	u32 * OAMmem = (u32*)OAM;

	int minusVal = 0;

	u32 * hudSprite = (u32*)getHudItemSprite();
	//	if (hudSpecialSpriteFunctionCallback != null) {
	//		u32 * hudSprite = hudSpecialSpriteFunctionCallback();
	if (hudSprite != null) {
			OAMmem[0] = hudSprite[0];
			OAMmem[1] = hudSprite[1];
			OAMmem += WORDS_PER_SPRITE;
			minusVal += 2;
	}
	//	}

#ifdef GOLD_HUD
	u32 * goldSprite = (u32*)getHudGoldSprite();
	OAMmem[0] = goldSprite[0];
	OAMmem[1] = goldSprite[1];
	OAMmem += WORDS_PER_SPRITE;
	minusVal += 2;
#endif

	copyMem32((void *)OAMmem, (void*)temp, (SPRITELIST_WORDS - minusVal));

	updateSpriteDisplayUsingCustomSprites();
}

void clearTopCustomSprites()
{
	int i;
	for (i = 0; i < NUM_TOP_SPECIAL_SPRITES; i++) {
		Sprite * sprite = specialTopSprite(i);
		initSprite(sprite);
		setSpriteNotInUse(sprite);
	}
}

void clearCustomSprites() {
	int i;
	for (i = 0; i < NUM_SPECIAL_SPRITES; i++) {
		Sprite * sprite = specialSprite(i);
		initSprite(sprite);
		setSpriteNotInUse(sprite);
	}
}

void updateTopSpriteDisplayUsingCustomSprites() {
	int loop;
	u32 * temp;
	temp = PUN(u32*)specialTopSprites;
	u32 * OAMmem = (u32*)OAM;
	for (loop = 0; loop < NUM_TOP_SPECIAL_SPRITES * WORDS_PER_SPRITE; loop++) {
		OAMmem[loop] = temp[loop];
	}
	//fill up the rest with the actual sprites.....
	temp = PUN(u32*)spriteShadowBuffer;
	OAMmem = (u32*) OAM;
	OAMmem += (SPRITELIST_WORDS >> 1);
	copyMem32((void *)OAMmem, (void*) temp, SPRITELIST_WORDS >> 1);

}

void updateSpriteDisplayUsingCustomSprites() {
	int loop;
	u32 * temp;
	temp = PUN(u32*)specialSprites;
	u32 * OAMmem = (u32*)OAM_SUB;
	for (loop = 0; loop < NUM_SPECIAL_SPRITES * WORDS_PER_SPRITE; loop++) {
		OAMmem[loop] = temp[loop];
	}
	//fill up the rest with the actual sprites.....
	temp = PUN(u32*)spriteShadowBuffer;
	OAMmem = (u32*) OAM;
	OAMmem += (SPRITELIST_WORDS >> 1);
	copyMem32((void *)OAMmem, (void*) temp, SPRITELIST_WORDS >> 1);

}

Sprite * specialTopSprite(int num) {
	return (Sprite *)&specialTopSprites[num];
}

Sprite * specialSprite(int num) {
	return (Sprite *)&specialSprites[num];
}

/////////////////////////////
//   Sprite Gfx Swapper
//////////////////////////////
//  This is used for swapping in sprite tile data on the fly.
//  Sprite video mem will be divided into "blocks", which will be swapped in and out
//  depending on which was the last used.
//  Also, any number of blocks, in the first block positions, can be "locked" so they are not
//  swapped out.
////////////////////////////


typedef struct {
	u16 * origin;
	int age;
} SpriteGfxBlockEntry; //struct for holding the cache data for each inMem-block

typedef SpriteGfxBlockEntry PaletteBlockEntry;

SpriteGfxBlockId spriteGfxUnlocked; //index of the lowest unlocked index
SpriteGfxBlockEntry spriteGfxSwapper[SWAPPER_NUM_SLOTS]; //list of cache data

PaletteId paletteUnlocked;
PaletteBlockEntry paletteSwapper[PAL_SWAPPER_NUM_SLOTS];

//////
//private functions for operating on sprite tile memory swapper:
/////

//find the video memory location referred to by a block id
u16 * getVidMemFromBlockId(SpriteGfxBlockId id) {
	u16 * spriteGfxData= SPRITE_GFX;
	spriteGfxData += (SWAPPER_BLOCK_SIZE >> 1) * id;
	return spriteGfxData;
}

//find the video memory location referred to by a block id
u16 * getVidMemFromPaletteId(PaletteId id) {
	u16 * spritePaletteDat= SPRITE_PALETTE;
	spritePaletteDat += (id * 16);
	return spritePaletteDat;
}

//refresh a block's age
void setSpriteGfxBlockUsedNow(SpriteGfxBlockId spriteBlock) {
	spriteGfxSwapper[spriteBlock].age = 0;
}

void setPaletteUsedNow(PaletteId palette) {
	paletteSwapper[palette].age = 0;
}

//get the oldest block, to swap out
SpriteGfxBlockId findOldestSpriteGfxBlock() {
	int i;
	int oldestAge = 0;
	int oldestFound = -1;
	for (i = spriteGfxUnlocked; i < SWAPPER_NUM_SLOTS; i++) {
		//if we find one that's not assigned, use it immediately
		if (spriteGfxSwapper[i].origin == null) {
			return i;
		}

		//else, if it is the oldest so far, use it
		if (spriteGfxSwapper[i].age >= oldestAge) {
			oldestAge = spriteGfxSwapper[i].age;
			oldestFound = i;
		}
	}

	return oldestFound;

}

PaletteId findOldestPalette() {
	int i;
	int oldestAge = 0;
	int oldestFound = -1;
	for (i = paletteUnlocked; i < PAL_SWAPPER_NUM_SLOTS; i++) {
		//if we find one that's not assigned, use it immediately
		if (paletteSwapper[i].origin == null) {
			return i;
		}

		//else, if it is the oldest so far, use it
		if (paletteSwapper[i].age >= oldestAge) {
			oldestAge = paletteSwapper[i].age;
			oldestFound = i;
		}
	}

	return oldestFound;

}

//////
//public functions for operating on sprite tile memory swapper:
//////

int spriteGfxBlockFirstTileNum(SpriteGfxBlockId id) {
	return id * SWAPPER_TILES_PER_BLOCK * 2;
}

//loads a new gfx block into vid mem
SpriteGfxBlockId loadSpriteGfxBlock(u16 * spriteGfx) {
	int existing = findSpriteGfxBlock(spriteGfx);
	if (existing > -1) {
		return existing;
	}
	int oldest = findOldestSpriteGfxBlock();
	forceLoadSpriteGfxBlock(spriteGfx, oldest);

	return oldest;
}

PaletteId loadPalette(u16 * palette) {
	int existing = findPalette(palette);
	if (existing > -1) {
		return existing;
	}
	int oldest = findOldestPalette();
	forceLoadPalette(palette, oldest);

	return oldest;
}

//forcefully loads a gfx block into a specified block
void forceLoadSpriteGfxBlock(u16 * spriteGfx, SpriteGfxBlockId id) {
	setSpriteGfxBlockUsedNow(id);
	spriteGfxSwapper[id].origin = spriteGfx;
	copyMem16(getVidMemFromBlockId(id), spriteGfx, SWAPPER_BLOCK_SIZE >> 1);
}

//forcefully loads a palette into a specified block
void forceLoadPalette(u16 * palette, PaletteId id) {
	setPaletteUsedNow(id);
	paletteSwapper[id].origin = palette;
	copyMem16(getVidMemFromPaletteId(id), palette, PAL_SIZE);
}

void initSpriteGfxSwapper() {
	spriteGfxUnlocked = 0;
	int i;
	for (i = 0; i < SWAPPER_NUM_SLOTS; i++) {
		spriteGfxSwapper[i].origin = null;
		spriteGfxSwapper[i].age = 0;
	}
	initPaletteSwapper();

}

void initPaletteSwapper() {
	paletteUnlocked = 0;
	int i;
	for (i = 0; i < PAL_SWAPPER_NUM_SLOTS; i++) {
		paletteSwapper[i].origin = null;
		paletteSwapper[i].age = 0;
	}

}

void setMaxLockedGfxBlock(SpriteGfxBlockId id)
//sets locked blocks, for main character, etc
{
	spriteGfxUnlocked = id + 1;
}

void setMaxLockedPalette(PaletteId id)
//sets locked palettes, for main character, etc
{
	paletteUnlocked = id + 1;
}

//this should be called once per frame
void timeTickForSpriteGfxBlocks() {
	int i;
	for (i = spriteGfxUnlocked; i < SWAPPER_NUM_SLOTS; i++) {
		spriteGfxSwapper[i].age++;
	}
}

//this should be called once per frame
void timeTickForPalette() {
	int i;
	for (i = paletteUnlocked; i < PAL_SWAPPER_NUM_SLOTS; i++) {
		paletteSwapper[i].age++;
	}
}

//return the original memory location for a gfx block
u16 * getOriginOfSpriteGfxBlock(SpriteGfxBlockId id) {
	return spriteGfxSwapper[id].origin;
}

//return the original memory location for a palette
u16 * getOriginOfPalette(PaletteId id) {
	return paletteSwapper[id].origin;
}

//find the gfx block pointing to this address
SpriteGfxBlockId findSpriteGfxBlock(u16 * spriteGfx) {
	int i;
	for (i = 0; i < SWAPPER_NUM_SLOTS; i++) {
		if (spriteGfxSwapper[i].origin == spriteGfx) //compare pointers, not values
		{
			setSpriteGfxBlockUsedNow(i);
			return i;
		}
	}
	return -1;
}

//find the gfx block pointing to this address
PaletteId findPalette(u16 * palette) {
	int i;
	for (i = 0; i < PAL_SWAPPER_NUM_SLOTS; i++) {
		if (paletteSwapper[i].origin == palette) //compare pointers, not values
		{
			setPaletteUsedNow(i);
			return i;
		}
	}
	return -1;
}

void addToOwnerList(CharacterStatus * ownerPtr) {
	if ((ownerPtr == null)) {
		return;
	}

	CharacterStatus * owner = ownerPtr;

	int i=0;
	while ((i <= lastOwner) && (ownerList[i] != owner)) {
		i++;
	}

	if (i > lastOwner) {
		lastOwner++;
	}

	if (lastOwner >= OWNERLIST_SIZE) {
		lastOwner = OWNERLIST_SIZE - 1;
		return;
	}

	ownerList[i] = owner;

}

//actually, I'm not sure we ever want to do this....
//we don't actually create any new character objects
//so it's ok to have all character objects on the owner list
//...and if we remove a static character (like bulletOwner
// or mapObjectOwner), then the other bullets/objects won't get
// sorted and displayed
void removeFromOwnerList(CharacterStatus * ownerPtr) {
	if (ownerPtr == null) {
		return;
	}

	CharacterStatus * owner = ownerPtr;

	int i = 0;
	bool done = false;
	while ((i <= lastOwner) && (!done)) {
		if (ownerList[i] == owner) {
			ownerList[i] = null;
			if (i == lastOwner) {
				lastOwner--;
			}
			done = true;
		}
		i++;
	}

}

void clearOwnerList() {
	int i;
	for (i = 0; i < OWNERLIST_SIZE; i++) {
		ownerList[i] = null;
	}
	lastOwner = 0;

	ensureMCOnOwnerList();

}

void resetGfxStats() {
	clearOwnerList();
	timeTickForSpriteGfxBlocks();
	timeTickForPalette();
}

