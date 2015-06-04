#include "platform.h"
#include "dsMisc.h"
#include "misc.h"

void initMisc()
{
	seedRandom();
	initDsVideo();
	initDsIrq();
}

void setGfxMode(GfxMode mode)
{
	if (mode == GFX_MODE_STANDARD)
	{
		setDsGfxModeStandard();
		/*
			SetMode(MODE_0 | OBJ_ENABLE | OBJ_MAP_2D);   //enable mode 0
			enable_bg(BG_MAIN);
			enable_bg(BG_TEXT);
			disable_bg(BG_MID);
			enable_bg(BG_FOREGROUND);
			REG_BLDMOD = BLEND_NORMAL | 
						BLEND_BG_BEHIND(BG_MAIN_NUM);
			REG_COLEV = 0x0509;             //faded sprites
			*/
	}
	if (mode == GFX_MODE_SPLASH)
	{
		setDsGfxModeSplash();
		/*
			SetMode(MODE_3 | OBJ_MAP_2D | BG_FOREGROUND);   //enable mode 0
			REG_BLDMOD = 0x440;				//turn on transparency for 
			REG_COLEV = 0x0509;             //faded sprites
			*/
	}
	if (mode == GFX_MODE_TEXTONLY)
	{
		//do we even need this mode?
//			SetMode(MODE_0 | OBJ_MAP_2D | BG_TEXT);  
	}
	
}

