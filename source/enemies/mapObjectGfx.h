//======================================================================
//
//	map_objectGfx, 256x32@4, 
//	+ palette 16 entries, not compressed
//	+ 128 tiles not compressed
//	Total size: 32 + 4096 = 4128
//
//	Time-stamp: 2006-02-20, 14:49:47
//	Exported by Cearn's Usenti v1.7.1
//	(comments, kudos, flames to "daytshen@hotmail.com")
//
//======================================================================

#ifndef __MAP_OBJECTGFX__
#define __MAP_OBJECTGFX__

#define map_objectGfxPalLen 32
extern const unsigned short map_objectGfxPal[16];
extern const unsigned short map_objectBushPal[16];
extern const unsigned short map_objectDoorPal[16];
extern const unsigned short map_objectCactusPal[16];

#define map_objectGfxTilesLen 4096
extern const unsigned short map_objectGfxTiles[2048];

#endif // __MAP_OBJECTGFX__

