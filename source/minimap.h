#ifndef _MINIMAP_H_
#define _MINIMAP_H_
#include "level.h"

#define MINIMAP_ARRAY_SIZE 35


const typedef enum 
{
	PIN_TREASURE_CAVE,
	PIN_SHOP_1,
	PIN_DNGN_4,
	PIN_HEALER_1,
	PIN_WESTERN_PASS_EXIT,
	PIN_WESTERN_PASS_ENTRANCE,
	PIN_HEALER_2,
	PIN_DNGN_1,
	PIN_SHOP_2,
	PIN_DNGN_5,
	PIN_DNGN_3,
	PIN_ROCKY_CAVE,
	PIN_SHOP_3,
	PIN_HEALER_3,
	PIN_OASIS_PATH_EXIT,
	PIN_SHOP_4,
	PIN_OASIS_PATH_ENTRANCE,
	PIN_DNGN_2,
} PinLocations;

void updateMiniMap();
bool hasMiniMap();
void drawMiniMap();
void updateDrawMiniMap();
void drawMiniMapOverworld(int xOrigin, int yOrigin);
void forceMiniMapUpdate();

void purchasedMap(int itemNumber);

void incrementMapPin();
void decrementMapPin();

/**
 * gets the raw data for saving the minimap array.
 * its length will be of MINIMAP_ARRAY_SIZE
 * use with caution!  the inernals of how it is saved
 * are NOT to be known by other modules....only 
 * the data itself, for saving/restoring/etc
 */
u32 * getMiniMapArray();

/**
 * clears the minimap visited list
 */
void clearMiniMap();

/**
 * makes you have been everywhere in the minimap.
 * useful for testing/debugging/cheating
 */
void miniMapBeenEverywhere();

#endif
