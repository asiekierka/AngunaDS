//======================================================================
//
//	cutscene_dungeonWindow, 256x160@8, 
//	+ palette 256 entries, not compressed
//	+ 406 tiles (t|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x20 
//	Total size: 512 + 25984 + 1280 = 27776
//
//	Time-stamp: 2006-03-10, 12:56:38
//	Exported by Cearn's Usenti v1.7.1
//	(comments, kudos, flames to "daytshen@hotmail.com")
//
//======================================================================

#ifndef __CUTSCENE_DUNGEONWINDOW__
#define __CUTSCENE_DUNGEONWINDOW__

#define cutscene_dungeonWindowPalLen 512
extern const unsigned short cutscene_dungeonWindowPal[256];

#define cutscene_dungeonWindowTilesLen 25984
extern const unsigned short cutscene_dungeonWindowTiles[12992];

#define cutscene_dungeonWindowMapLen 1280
extern const unsigned short cutscene_dungeonWindowMap[640];

#endif // __CUTSCENE_DUNGEONWINDOW__

