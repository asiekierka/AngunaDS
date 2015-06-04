//======================================================================
//
//	cutscene_gameStart, 256x160@8, 
//	+ palette 256 entries, not compressed
//	+ 419 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x20 
//	Total size: 512 + 26816 + 1280 = 28608
//
//	Time-stamp: 2005-12-28, 23:30:46
//	Exported by Cearn's Usenti v1.7.1
//	(comments, kudos, flames to "daytshen@hotmail.com")
//
//======================================================================

#ifndef __CUTSCENE_GAMESTART__
#define __CUTSCENE_GAMESTART__

#define cutscene_gameStartPalLen 512
extern const unsigned short cutscene_gameStartPal[256];

#define cutscene_gameStartTilesLen 26816
extern const unsigned short cutscene_gameStartTiles[13408];

#define cutscene_gameStartMapLen 1280
extern const unsigned short cutscene_gameStartMap[640];

#endif // __CUTSCENE_GAMESTART__

