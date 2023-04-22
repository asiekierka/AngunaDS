#include <nds.h>
#include <maxmod9.h>

#include "platform.h"
#include "misc.h"
#include "sound.h"

#include "soundbank.h"
#include "soundbank_bin.h"

void playGameSfx(int effect)
{
	mmLoadEffect(effect);
	mmEffect(effect);
}

static int current_song = -1;

void playMusic(int song)
{
	if (song != current_song)
	{
		if (current_song >= 0)
		{
			mmStop();
			mmUnload(current_song);
		}
		mmLoad(song);
		mmStart(song, MM_PLAY_LOOP);
		current_song = song;
	}
}

void initSoundSystem()
{
	mmInitDefaultMem((mm_addr) soundbank_bin);
	mmSelectMode(MM_MODE_A);
}

void soundWorkerVBlank()
{
}

void soundWorker()
{
}

