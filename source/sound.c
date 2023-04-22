#include <nds.h>
#include <maxmod9.h>

#include "platform.h"
#include "misc.h"
#include "sound.h"

#include "soundbank.h"

static comutex_t mmMutex;

int playGameSfxThread(void *arg)
{
	int effect = (u32) arg;
	if (!comutex_try_acquire(&mmMutex)) return 0;
	mmLoadEffect(effect);
	mmEffect(effect);
	comutex_release(&mmMutex);
	return 0;
}

void playGameSfx(int effect)
{
	cothread_create(playGameSfxThread, (void*)effect, 0, COTHREAD_DETACHED);
	cothread_yield();
}

static int current_song = -1;

int playMusicThread(void *arg)
{
	int song = (u32) arg;
	comutex_acquire(&mmMutex);
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
	comutex_release(&mmMutex);
	return 0;
}

void playMusic(int song)
{
	cothread_create(playMusicThread, (void*)song, 0, COTHREAD_DETACHED);
	cothread_yield();
}

void initSoundSystem()
{
	mmInitDefault("nitro:/soundbank.bin");
	//mmInitDefaultMem((mm_addr) soundbank_bin);
	mmSelectMode(MM_MODE_A);
	comutex_init(&mmMutex);
}
