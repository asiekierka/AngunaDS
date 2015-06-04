
#include <nds.h>

#include "platform.h"
#include "misc.h"
#include "sound.h"
#include "audio/audio.h"
#include "audio/audioDefs.h"


const void * getSoundEffect(int effect)
{

	switch(effect)
	{
		case SFX_TAKE_HIT:
			return REFSFX(ugh);
		case SFX_ENEMY_HIT:
			return REFSFX(thunk);
		case SFX_TAKE_ITEM:
			return REFSFX(getItemGood);
		case SFX_DOOR:
		case SFX_BADCHOICE:
			return REFSFX(door);
		case SFX_BITE:
			return REFSFX(bite);
		case SFX_SWORD:
		case SFX_ARROW:
			return REFSFX(swingSword);
		case SFX_ENEMY_SPIT:
			return REFSFX(spit);
		case SFX_CHANGE_ITEM:
			return REFSFX(changeItem);
		case SFX_SHOW_SUBSCREEN:
		case SFX_HIDE_SUBSCREEN:
			return REFSFX(pageFlip);
		case SFX_POTION:
			return REFSFX(potion);
		case SFX_SWITCH:
			return REFSFX(changeItem);
		case SFX_ARMOR:
			return REFSFX(armor);
		case SFX_DYNAMITE:
			return REFSFX(dynamite);
	}
	return null;
}

const u32 getSoundEffectLength(effect)
{
	switch(effect)
	{
		case SFX_TAKE_HIT:
			return REFSFX_SIZE(ugh);
		case SFX_ENEMY_HIT:
			return REFSFX_SIZE(thunk);
		case SFX_TAKE_ITEM:
			return REFSFX_SIZE(getItemGood);
		case SFX_DOOR:
		case SFX_BADCHOICE:
			return REFSFX_SIZE(door);
		case SFX_BITE:
			return REFSFX_SIZE(bite);
		case SFX_SWORD:
		case SFX_ARROW:
			return REFSFX_SIZE(swingSword);
		case SFX_ENEMY_SPIT:
			return REFSFX_SIZE(spit);
		case SFX_CHANGE_ITEM:
			return REFSFX_SIZE(changeItem);
		case SFX_SHOW_SUBSCREEN:
		case SFX_HIDE_SUBSCREEN:
			return REFSFX_SIZE(pageFlip);
		case SFX_POTION:
			return REFSFX_SIZE(potion);
		case SFX_SWITCH:
			return REFSFX_SIZE(changeItem);
		case SFX_ARMOR:
			return REFSFX_SIZE(armor);
		case SFX_DYNAMITE:
			return REFSFX_SIZE(dynamite);
	}
	return 0;
}

//this is a hack because my sounds aren't all saved at the same frequency
int getSoundEffectFrequency(int effect)
{
	switch (effect)
	{
		case SFX_TAKE_HIT:
		case SFX_ENEMY_HIT:
		case SFX_SWORD:
		case SFX_ARROW:
			return 22050;
	}
	return 11025;
}


void playGameSfx(int effect)
{
	TransferSoundData sound;
	sound.data = getSoundEffect(effect);
	sound.len = getSoundEffectLength(effect);
	sound.rate = getSoundEffectFrequency(effect);
	sound.vol = 127;
	sound.pan = 64;
	sound.format = 2;
	if (sound.data != null) 
	{
		playSound(&sound);
	}

}

void playMusic(int song)
{
	playMikModSong(song);
}

void initSoundSystem()
{
	initMikMod();
	
}

void soundWorkerVBlank()
{
}

void soundWorker()
{
}

