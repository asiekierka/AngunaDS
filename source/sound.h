#ifndef _SOUND_H_
#define _SOUND_H_

//#include "audio/samples.h"

#define SFX_UNDEFINED	-1

//until we get the sound engine working...
#define SFX_TAKE_HIT 	1
#define SFX_ENEMY_HIT 	2
#define SFX_TAKE_ITEM	3
#define SFX_DOOR		4
#define SFX_BITE		5
#define SFX_SWORD		6
#define SFX_ENEMY_SPIT	7
#define SFX_CHANGE_ITEM	8
#define SFX_SHOW_SUBSCREEN	9
#define SFX_HIDE_SUBSCREEN	10
#define SFX_POTION	    11
#define SFX_ARROW	    12
#define SFX_SWITCH	    13
#define SFX_BADCHOICE   14
#define SFX_ARMOR   	15
#define SFX_DYNAMITE   	16

/*
#define SFX_TAKE_HIT 	SAMPLE_UGH4
#define SFX_ENEMY_HIT 	SAMPLE_THUNK
#define SFX_TAKE_ITEM	SAMPLE_GETITEMGOOD
#define SFX_DOOR		SAMPLE_DOOR21
#define SFX_BITE		SAMPLE_BITE
#define SFX_SWORD		SAMPLE_SWINGSWORD2
#define SFX_ENEMY_SPIT	SAMPLE_SPIT
#define SFX_CHANGE_ITEM	SAMPLE_CHANGEITEM
#define SFX_SHOW_SUBSCREEN	SAMPLE_PAGEFLIP
#define SFX_HIDE_SUBSCREEN	SAMPLE_PAGEFLIP
#define SFX_POTION	    SAMPLE_POTION
#define SFX_ARROW	    SAMPLE_SWINGSWORD2
#define SFX_SWITCH	    SAMPLE_CHANGEITEM
#define SFX_BADCHOICE   SAMPLE_DOOR21
#define SFX_ARMOR   	SAMPLE_ARMOR
*/


//#define MUSIC_MAINTHEME (void*)fred
//#define MUSIC_OVERWORLD (void*)keetoverworld
//#define MUSIC_DUNGEON (void*)theme
#define MUSIC_MAINTHEME 0
#define MUSIC_OVERWORLD 1
#define MUSIC_DUNGEON 2

void playMusic(int song);

void initSoundSystem();
void soundWorker();
void soundWorkerVBlank();

void playGameSfx(int effect);

#endif
