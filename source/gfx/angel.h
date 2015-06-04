//======================================================================
//
//	angel, 256x32@4, 
//	+ palette 16 entries, not compressed
//	+ 128 tiles not compressed
//	Total size: 32 + 4096 = 4128
//
//	Time-stamp: 2006-03-27, 17:54:59
//	Exported by Cearn's Usenti v1.7.1
//	(comments, kudos, flames to "daytshen@hotmail.com")
//
//======================================================================

#ifndef __ANGEL__
#define __ANGEL__

#define angelPalLen 32
extern const unsigned short angelPal[16];

#define angelTilesLen 4096
extern const unsigned short angelTiles[2048];


const characterAnimDef * AngelAnim;
#endif // __ANGEL__

