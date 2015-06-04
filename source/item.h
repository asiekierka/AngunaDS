#ifndef _ITEM_H_
#define _ITEM_H_

#include "enemies.h"


#define ITEM_MEAT_HEALTH 8

#define ITEM_RANDOM_MASK 0x100
#define ITEM_DROP_MASK (0xff | ITEM_RANDOM_MASK)


#define IS_RANDOM(x)	((x) & ITEM_RANDOM_MASK)
#define GET_ITEM(x)		((x) & 0xff)

//#define NUM_ITEMS 18
#define NUM_ITEMS 21

#define ITEM_RANDOM_1 		0x101	
#define ITEM_RANDOM_2 		0x102	
#define ITEM_RANDOM_3 		0x103	
#define ITEM_RANDOM(x)		(ITEM_RANDOM_MASK | (x))
#define ITEM_NONE 			0	
#define ITEM_KEY_RED 		1
#define ITEM_KEY_YELLOW 	2
#define ITEM_KEY_BLUE 		3
#define ITEM_MEAT			4
#define ITEM_ATTACK_UP		5
#define ITEM_LIFE_UP		6
#define ITEM_DEFENCE_UP		7
#define ITEM_BOW			8
#define ITEM_ARROWS			9
#define ITEM_LANTERN		10
#define ITEM_POTION			11
#define ITEM_BOOTS			12
#define ITEM_CALTROPS		13
#define ITEM_GOLD			14
#define ITEM_ATTACK_RING	15
#define ITEM_DYNAMITE		16
#define ITEM_GOLD_BAG		17
#define ITEM_SMALL_BAG		18
#define ITEM_MAP_DNGN_2		19
#define ITEM_MAP_DNGN_3		20
#define ITEM_MAP_DNGN_4		21

#define ITEM_SAVES		0x100
#define ITEM_REWARD		0x200
#define ITEM_SPECIAL	0x400

#define SAVE_STATE(x)	(SAVES | (x))

#define ITEM_SAVES_STATE(x) ((x) & ITEM_SAVES)
#define ITEM_SAVE_STATE(x) ((x) & 0xff)
#define ITEM_IS_KEY(x) ((x == ITEM_KEY_RED) || (x == ITEM_KEY_YELLOW) || (x == ITEM_KEY_BLUE))
extern const EnemyType enemy_item_def;

#endif
