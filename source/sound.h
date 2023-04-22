#ifndef _SOUND_H_
#define _SOUND_H_

#include "soundbank.h"

#define SFX_UNDEFINED	-1

// Synonyms

#define SFX_TAKE_HIT		SFX_UGH
#define SFX_ENEMY_HIT 		SFX_THUNK
#define SFX_TAKE_ITEM		SFX_GETITEMGOOD
#define SFX_SWORD		SFX_SWINGSWORD
#define SFX_ENEMY_SPIT		SFX_SPIT
#define SFX_CHANGE_ITEM		SFX_CHANGEITEM
#define SFX_SHOW_SUBSCREEN	SFX_PAGEFLIP
#define SFX_HIDE_SUBSCREEN	SFX_PAGEFLIP
#define SFX_SWITCH	    	SFX_CHANGEITEM
#define SFX_BADCHOICE   	SFX_DOOR
#define SFX_ARROW		SFX_SWINGSWORD

#define MUSIC_MAINTHEME MOD_OVERWORLD
#define MUSIC_OVERWORLD MOD_OVERWORLD
#define MUSIC_DUNGEON MOD_DUNGEON

void playMusic(int song);
void playGameSfx(int effect);
void initSoundSystem();

#endif
