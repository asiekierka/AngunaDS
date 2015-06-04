#ifndef _DOOR_H_
#define _DOOR_H_

#include "item.h"

#define DOOR_HORIZONTAL 1
#define DOOR_VERTICAL_L 2
#define DOOR_VERTICAL_R 3
#define DOOR_VERTICAL 2



#define DOOR_RED 		0x04
#define DOOR_YELLOW 	0x08
#define DOOR_BLUE	 	0x0C


#define TEST_HAS_NO_LOCK  4
#define SET_NO_LOCK_TILE 74	

#define DOOR_APPEARS_BEFORE   0x1000
#define DOOR_APPEARS_AFTER    0x2000
#define DOOR_HIDDEN 		  0x4000
#define DOOR_INVIS			  0x2000

#define DOOR_FAKE	 (1<<0xA)

#define DOOR_APPEARS_LATE(x)  ((x) & (0x3000))

#define DOOR_DIRECTION(x) ((x) & 3)
#define DOOR_IS_HIDDEN(x)     ((x) & (DOOR_HIDDEN))
#define DOOR_IF_HIDDEN(x)     (DOOR_IS_HIDDEN(x)?3:0)
#define DOOR_IF_INVIS(x)	  (((x) & (DOOR_INVIS))?6:0)
#define DOOR_FRAME(x) (DOOR_DIRECTION(x) + DOOR_IF_HIDDEN(x) + DOOR_IF_INVIS(x))
#define IS_VERTICAL(x) (((x)==DOOR_VERTICAL_L)||((x)==DOOR_VERTICAL_R))
#define DOOR_KEY(x)	(((x) & DOOR_BLUE) >> 2)

#define DOOR_COLOR_TO_TILE(x)	((DOOR_KEY(x) * 6) + 4)

//to use a switch to open a door, the switch 
//MUST be immediately before the door in the
//level's enemy list.
//Then put SWTICH_DOOR(1) in the 
//switch's special parameter.  The number(1)
//indicates how many of the next doors are 
//affected by the switch
#define SWITCH_DOOR(x) (x) 
#define SWITCH_NUM_DOORS(x) ((x->special) & 7)

#define SWITCH_TOGGLE(x) (x)
#define SWITCH_GET_TOGGLE(x) (((x)->itemDrop) & 31)

#define DOOR_TOGGLE(x) ((x) << 5)
#define DOOR_GET_TOGGLE(x) ((((x)->itemDrop)>>5) & 31)
#define DOOR_OPEN_ON_TOGGLE(x) ((((x)->itemDrop)>>0xA) & 1)
#define DOOR_TOGGLE_FLIP (1 << 0xA)



#endif
