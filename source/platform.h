#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <nds.h>

//redefine these to get nds-specific stuff out of the rest of the files
#define SQUARE ATTR0_SQUARE
#define WIDE ATTR0_WIDE
#define TALL ATTR0_TALL
#define SIZE_8 ATTR1_SIZE_8
#define SIZE_16 ATTR1_SIZE_16
#define SIZE_32 ATTR1_SIZE_32
#define SIZE_64 ATTR1_SIZE_64

#define GBA_SCREEN_WIDTH 240
#define GBA_SCREEN_HEIGHT 160

#define SCREEN_BLOCK_WIDTH (SCREEN_WIDTH >> 3)
#define SCREEN_BLOCK_HEIGHT (SCREEN_HEIGHT >> 3)

#define GBA_SCREEN_BLOCK_WIDTH (GBA_SCREEN_WIDTH >> 3)
#define GBA_SCREEN_BLOCK_HEIGHT (GBA_SCREEN_HEIGHT >> 3)

#define IN_EWRAM

//#define ASSERT(x,y) while ((!(x))){setStatusMessage(y); while(1){}}
#define ASSERT(x,y)
#define TRACE()

typedef enum{
	GFX_MODE_STANDARD,
	GFX_MODE_SPLASH,
	GFX_MODE_TEXTONLY,
} GfxMode;
 
typedef SpriteEntry Sprite;

/**
 * One frame tick
 */
void frameTick();


void setGfxMode(GfxMode mode);

void initMisc();

#endif /*PLATFORM_H_*/
