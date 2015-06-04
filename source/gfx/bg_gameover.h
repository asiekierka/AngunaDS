//======================================================================
//
//	bg_gameover, 256x176@8, 
//	+ palette 256 entries, not compressed
//	+ 437 tiles (t reduced) not compressed
//	+ regular map (flat), not compressed, 32x22 
//	Total size: 512 + 27968 + 1408 = 29888
//
//	Time-stamp: 2006-03-28, 13:17:41
//	Exported by Cearn's Usenti v1.7.1
//	(comments, kudos, flames to "daytshen@hotmail.com")
//
//======================================================================

#ifndef __BG_GAMEOVER__
#define __BG_GAMEOVER__

#define bg_gameoverPalLen 512
extern const unsigned short bg_gameoverPal[256];

#define bg_gameoverTilesLen 27968
extern const unsigned short bg_gameoverTiles[13984];

#define bg_gameoverMapLen 1408
extern const unsigned short bg_gameoverMap[704];

#endif // __BG_GAMEOVER__

