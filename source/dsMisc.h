#ifndef DSMISC_H_
#define DSMISC_H_


void initDsIrq();
void dsFrameTick();
void initDsVideo();

/**
 * Sets to a standard gfx mode for the game (8-bit color, tiled backgrounds, sprites, etc)
 */
void setDsGfxModeStandard();

/**
 * Sets to a splash-screen gfx mode for the game
 */
void setDsGfxModeSplash();

#endif /*DSMISC_H_*/
