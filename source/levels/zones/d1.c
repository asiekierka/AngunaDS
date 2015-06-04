#include "../../level.h"
#include "../../sound.h"

const u8 zone_d1_map[] = {
 71, 76, 77, 80, 77,  0,  0,  0,  0,  0,
 59, 48, 32, 75, 47,  0,  0,  0,  0,  0,
 50, 38, 50, 53, 23,  0,  0,  0,  0,  0,
 50, 17, 43, 61, 51,  0,  0,  0,  0,  0,
  0,  0,  0,  0, 80,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,};



const ZoneData zone_d1 = {
	(u8 *) zone_d1_map,
	1,
	4,
	2,
	MUSIC_DUNGEON
};

