#include <nds.h>
#include "bgDefines.h"
#include "input.h"

#define BLEND_BG_BEHIND(x) (1 << ((x) + 8))
#define NICE_BLEND_LEVELS (0x0509);

void vblank()
{
	//need to do anything during vblank?
}

void initDsIrq()
{

	//irqs are nice
	//irqInit();
	irqSet(IRQ_VBLANK, vblank);
}

void initDsVideo()
{
	//turn everything on
	powerOn(POWER_ALL_2D);

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

	REG_BLDCNT = BLEND_ALPHA | BLEND_BG_BEHIND(BG_MAIN_NUM);
	REG_BLDALPHA = NICE_BLEND_LEVELS;

	REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_BG_BEHIND(BG_MAIN_NUM);
	REG_BLDALPHA_SUB = NICE_BLEND_LEVELS;


}


void setDsGfxModeSplash()
{
	REG_BLDCNT = 0x440;
	REG_BLDALPHA = NICE_BLEND_LEVELS;
}

