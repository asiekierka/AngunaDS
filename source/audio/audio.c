#include <nds.h>
//#include <mikmod9.h>

#include "../sound.h"
#include "ipc.h"
#include "libxm/include/libxm7.h"

extern const u8 overworld_bin_end[];
extern const u8 overworld_bin[];
extern const u32 overworld_bin_size;

extern const u8 dungeon_bin_end[];
extern const u8 dungeon_bin[];
extern const u32 dungeon_bin_size;

extern const u8 fred_bin_end[];
extern const u8 fred_bin[];
extern const u32 fred_bin_size;

/*
MODULE* dungeonMod = 0;
MODULE* overworldMod = 0;
MODULE* fredMod = 0;
*/

XM7_ModuleManager_Type dungeonMM;
XM7_ModuleManager_Type fredMM;
XM7_ModuleManager_Type overworldMM;


// called by the drivers in mikmod library
void MikMod9_SendCommand(u32 command)
{
	while (REG_IPC_FIFO_CR & IPC_FIFO_SEND_FULL);
	REG_IPC_FIFO_TX = IPC_PLAY_SONG;
	while (REG_IPC_FIFO_CR & IPC_FIFO_SEND_FULL);
	REG_IPC_FIFO_TX = command;
}

void audioTimerInterrupt()
{
	// player tick
	//MikMod_Update();
	
	// the bpm can change in the middle of the song
	//TIMER0_DATA = TIMER_FREQ_256(md_bpm * 50 / 125);
}

void initMikMod()
{
	REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR;

	XM7_LoadXM(&dungeonMM, (XM7_XMModuleHeader_Type*)dungeon_bin);
	XM7_LoadXM(&fredMM, (XM7_XMModuleHeader_Type*)fred_bin);
	XM7_LoadXM(&overworldMM, (XM7_XMModuleHeader_Type*)overworld_bin);

	//MikMod_RegisterDriver(&drv_nds_hw);

	//MikMod_RegisterLoader(&load_xm);
	//MikMod_Init("");
	//dungeonMod = Player_LoadMemory(dungeon_bin, dungeon_bin_size, 64, 0);
	//overworldMod = Player_LoadMemory(overworld_bin, overworld_bin_size, 64, 0);
}


int lastSong = -1;

void playMikModSong(int song)
{
	if (lastSong == song)
	{
		return;
	}
	lastSong = song;

	if (song == MUSIC_OVERWORLD)
	{
		MikMod9_SendCommand((u32)&overworldMM);
	}
	if (song == MUSIC_DUNGEON)
	{
		MikMod9_SendCommand((u32)&dungeonMM);
	}
	if (song == MUSIC_MAINTHEME)
	{
		MikMod9_SendCommand((u32)&fredMM);
	}
	//make sure mikMod is init'd 
	//if (dungeonMod == 0)
	//{
		//initMikMod();
	//}

	
	/*

	MODULE * mm = dungeonMod;
	if ((song == MUSIC_OVERWORLD))
	{
		mm = overworldMod;
	}

	if (mm)
	{
		mm->wrap = true;
		mm->loop = true;
		Player_Start(mm);
		TIMER0_CR = 0;
		irqSet(IRQ_TIMER0, audioTimerInterrupt);
		irqEnable(IRQ_TIMER0);
		TIMER0_DATA = TIMER_FREQ_256(md_bpm * 50 / 125);
		TIMER0_CR = TIMER_DIV_256 | TIMER_IRQ_REQ | TIMER_ENABLE;
	}
	else
	{
			consoleDemoInit();
			printf("Could not load module, reason: \n%s\n",
			MikMod_strerror(MikMod_errno));

		while(1){
		}
	}
	*/
}
