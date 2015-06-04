#include <nds.h>
#include "bgDefines.h"

#define BLEND_BG_BEHIND(x) (1 << ((x) + 8))
#define NICE_BLEND_LEVELS (0x0509);

void vblank()
{
	//need to do anything during vblank?
}

void initDsIrq()
{

	//irqs are nice
	irqInit();
	irqSet(IRQ_VBLANK, vblank);
}

void initDsVideo()
{
	//turn everything on
	powerON(POWER_ALL_2D);

    vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000,
                     VRAM_B_MAIN_BG,
                     VRAM_C_SUB_BG,
                     VRAM_D_LCD);

    vramSetBankE(VRAM_E_MAIN_SPRITE);
    vramSetBankI(VRAM_I_SUB_SPRITE);

}

void dsFrameTick()
{
	swiWaitForVBlank();
	scanKeys();
}

void setDsGfxModeStandard()
{
	//set the video mode
	videoSetMode(MODE_0_2D | DISPLAY_SPR_ACTIVE | //turn on sprites
	DISPLAY_BG0_ACTIVE | //turn on background 0
	DISPLAY_SPR_2D | //this is used when in tile mode
	DISPLAY_SPR_1D_BMP //and this in bitmap mode
	);
	
	videoSetModeSub(MODE_0_2D | DISPLAY_SPR_ACTIVE | //turn on sprites
	DISPLAY_BG0_ACTIVE | //turn on background 0
	DISPLAY_SPR_2D | //this is used when in tile mode
	DISPLAY_SPR_1D_BMP //and this in bitmap mode
	);

	BLEND_CR = BLEND_ALPHA | BLEND_BG_BEHIND(BG_MAIN_NUM);
	BLEND_AB = NICE_BLEND_LEVELS;

	SUB_BLEND_CR = BLEND_ALPHA | BLEND_BG_BEHIND(BG_MAIN_NUM);
	SUB_BLEND_AB = NICE_BLEND_LEVELS;


}


void setDsGfxModeSplash()
{
	BLEND_CR = 0x440;
	BLEND_AB = NICE_BLEND_LEVELS;
}

void lidSleep()
{
   __asm(".arm"); 
   __asm("mcr p15,0,r0,c7,c0,4");
   __asm("mov r0, r0");
   __asm("BX lr");
}

// when reading keys
void checkLidSleep()
{
	if (isKeyDown( KEY_LID)) {
	   /* hinge is closed */
	   /* set only key irq for waking up */
	   unsigned long oldIE = REG_IE;
	   REG_IE = IRQ_KEYS;
	   *(volatile unsigned short *)0x04000132 = BIT(14) | 255; /* any of the inner keys might irq */
	   /* power off everything not needed */
	   powerOFF(POWER_LCD);
	   /* set system into sleep */
	   swiWaitForIRQ();
	   /* wait a bit until returning power */
	   while (REG_VCOUNT!=0);
	   while (REG_VCOUNT==0);
	   while (REG_VCOUNT!=0);
	   /* power on again */
	   powerON(POWER_LCD);
	   /* set up old irqs again */
	   REG_IE = oldIE;
	} 
}
