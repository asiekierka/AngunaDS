#include <nds.h>
#include <stdlib.h>

//#include <mikmod7.h>
#include "../../libxm/include/libxm7.h"
#include "../../ipc.h"

bool xm7isPlaying = false;

void playSong(XM7_ModuleManager_Type* song)
{
	if (xm7isPlaying)
	{
		XM7_StopModule();
	}
	XM7_PlayModule(song);
	xm7isPlaying = true;
}

bool playSongReady = false;

void FIFOHandler()
{
	while(!(REG_IPC_FIFO_CR & IPC_FIFO_RECV_EMPTY))
	{
		u32 command = REG_IPC_FIFO_RX;
		if (playSongReady)
		{
			playSongReady = false;
			playSong((XM7_ModuleManager_Type *)command);
		}
		else if (command == IPC_PLAY_SONG)
		{
			playSongReady = true;
		}
		
		//if(command >= 1<<28)
			//MikMod7_ProcessCommand(command);
			
		// process your own fifo messages here
	}
}


/*
//---------------------------------------------------------------------------------
void startSound(int sampleRate, const void* data, u32 bytes, u8 channel, u8 vol,  u8 pan, u8 format) {
//---------------------------------------------------------------------------------
	SCHANNEL_TIMER(channel)  = SOUND_FREQ(sampleRate);
	SCHANNEL_SOURCE(channel) = (u32)data;
	SCHANNEL_LENGTH(channel) = bytes >> 2 ;
	SCHANNEL_CR(channel)     = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(vol) | SOUND_PAN(pan) | (format==1?SOUND_8BIT:SOUND_16BIT);
}
*/


//---------------------------------------------------------------------------------
s32 getFreeSoundChannel() {
//---------------------------------------------------------------------------------
	int i;
	for (i=8; i<16; i++) {
		if ( (SCHANNEL_CR(i) & SCHANNEL_ENABLE) == 0 ) return i;
	}
	return -1;
}


int vcount;
touchPosition first,tempPos;


void sleep()
{
		// Save the current interrupt sate.
		u32 ie_save = REG_IE;
		// Turn the speaker down.
		swiChangeSoundBias(0,0x400);
		// Save current power state.
		int power = readPowerManagement(PM_CONTROL_REG);
		// Set sleep LED.
		writePowerManagement(PM_CONTROL_REG, PM_LED_CONTROL(1));
		// Register for the lid interrupt.
		REG_IE = IRQ_LID;

		// Power down till we get our interrupt.
		swiSleep(); //waits for PM (lid open) interrupt

		REG_IF = ~0;
		// Restore the interrupt state.
		REG_IE = ie_save;
		// Restore power state.
		writePowerManagement(PM_CONTROL_REG, power);
		 
		// Turn the speaker up.
		swiChangeSoundBias(1,0x400);
}

void checkLid()
{
	int but = REG_KEYXY;
	// Check if the lid has been closed.
	if(but & BIT(7))
	{
		sleep();
	}
}

//---------------------------------------------------------------------------------
void VcountHandler() {
//---------------------------------------------------------------------------------
	static int lastbut = -1;

	checkLid();
	
	uint16 but=0, x=0, y=0, xpx=0, ypx=0, z1=0, z2=0;

	but = REG_KEYXY;

	if (!( (but ^ lastbut) & (1<<6))) {
 
		tempPos = touchReadXY();

		x = tempPos.x;
		y = tempPos.y;
		xpx = tempPos.px;
		ypx = tempPos.py;
		z1 = tempPos.z1;
		z2 = tempPos.z2;
		
	} else {
		lastbut = but;
		but |= (1 <<6);
	}

	if ( vcount == 80 ) {
		first = tempPos;
	} else {
		if (	abs( xpx - first.px) > 10 || abs( ypx - first.py) > 10 ||
				(but & ( 1<<6)) ) {

			but |= (1 <<6);
			lastbut = but;

		} else { 	
			IPC->mailBusy = 1;
			IPC->touchX			= x;
			IPC->touchY			= y;
			IPC->touchXpx		= xpx;
			IPC->touchYpx		= ypx;
			IPC->touchZ1		= z1;
			IPC->touchZ2		= z2;
			IPC->mailBusy = 0;
		}
	}
	IPC->buttons		= but;
	vcount ^= (80 ^ 130);
	SetYtrigger(vcount);

}

//---------------------------------------------------------------------------------
void VblankHandler(void) {
//---------------------------------------------------------------------------------
	u32 i;

	//sound code  :)
	TransferSound *snd = IPC->soundData;
	IPC->soundData = 0;

	if (0 != snd) {

		for (i=0; i<snd->count; i++) {
			s32 chan = getFreeSoundChannel();

			if (chan >= 0) {
				startSound(snd->data[i].rate, snd->data[i].data, snd->data[i].len, chan, snd->data[i].vol, snd->data[i].pan, snd->data[i].format);
			}
		}
	}
}

void Vblankhandler(void)
{
}

//---------------------------------------------------------------------------------
int main(int argc, char ** argv) {
//---------------------------------------------------------------------------------
    // init fifo
	REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR;

	// Reset the clock if needed
	rtcReset();

	XM7_Initialize();

	//enable sound
	powerON(POWER_SOUND);
	SOUND_CR = SOUND_ENABLE | SOUND_VOL(0x7F);
	IPC->soundData = 0;

	irqInit();
	irqSet(IRQ_VBLANK, VblankHandler);
	irqEnable(IRQ_VBLANK);
	SetYtrigger(80);
	vcount = 80;
	irqSet(IRQ_VCOUNT, VcountHandler);
	irqEnable(IRQ_VCOUNT);
	
	// Keep the ARM7 idle
	while (1)
	{

		FIFOHandler();
		swiWaitForVBlank();
	}
}


