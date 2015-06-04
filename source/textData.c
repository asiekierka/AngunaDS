#include "text.h"
#include "textData.h"
#include "item.h"

#define TSTR(x) (char **)&TextStrings[(x)]
#define LINES(x) (TextLine *)&TextLines[(x)]

const char * cutSceneText1_1[] = {
	"",
	"",
	"  Just your luck.  You have ",
	"  been captured by ",
	"  monsters.",
	"",
	"      Again.",
	null
};

const char * cutSceneText1_2[]= {
	"",
	"",
	"You had been on a mission",
	"to find and defeat the",
	"dragon, leader of the",
	"monsters.",
	"",
	"But you had to go and get",
	"yourself captured...",
	null
};

const char * cutSceneText1_3[]= {
	"",
	"",
	"Luckily, as usual, they left",
	"you with your sword in hand.",
	"",
	"Now all you need to do is",
	"kill off these ugly little",
	"guards and then get out of",
	"here, and continue your",
	"mission....",
	null
};

const char * TextDemoOver1[] = {
	"","",
	"Congratulations!  You ",
	"escaped from the dungeon...",
	"",
	"But there is a whole world",
	"outside to explore before",
	"you can find your way ",
	"back home...",
	null
};
const char * TextDemoOver2[] = {

	"","",
	"There is more to come later",
	"thanks for playing the demo!",
	"",
	"",
	" -gauauu ",
	"nathantolbert(at)gmail.com",
	null
};

const char * ItemText[] = {
	"",
	"Got Red Key",
	"Got Yellow Key",
	"Got Blue Key",
	"Got Food.  HP Restored.",
	"Attack Power Up!" ,
	"Max HP Up!" ,
	"Defense Up!",
	"Got Bow and Arrows" ,
	"Got Arrows" ,
	"Got Lantern",
	"Got Life Potion",
	"Got Winged Boots",
	"Got Traps",
	null,
	"Got Ring of Attack",
	"Got Dynamite",
	"Got 100 Gold",
	"Got 5 Gold",
	null,
	null,
	null,
};

const char * ItemNames[] = {
	[ITEM_KEY_RED] = "Red Key",
	[ITEM_KEY_YELLOW] = "Yellow Key",
	[ITEM_KEY_BLUE] = "Blue Key",
	[ITEM_MEAT] = "Food",
	[ITEM_ATTACK_UP] = "Attack Powerup",
	[ITEM_LIFE_UP] = "Life Powerup",
	[ITEM_DEFENCE_UP] = "Defense Powerup",
	[ITEM_ARROWS] = "Arrows", 
	[ITEM_POTION] = "Life Potion",
	[ITEM_CALTROPS] = "Traps",
	[ITEM_ATTACK_RING] = "Ring of Attack",
	[ITEM_DYNAMITE] = "Dynamite",
	[ITEM_MAP_DNGN_2] = "Map: Location of                     Desert Dungeon",
	[ITEM_MAP_DNGN_3] = "Map: Location of                     Tomb Dungeon",
	[ITEM_MAP_DNGN_4] = "Map: Location of                     Lake Dungeon",
};

const char * GameOver[] = {
	"","","","","","",
	"          Game Over",
	null
};

const char * SaveMenuText[] = {
	"Save and continue",
	"Save and quit",
	"Cancel",
	"Game Saved",
	"Cannot save with",
	"enemies still alive.",
	null
};

const char * EmptyEnemyDatabaseText[] = {
	"No enemies have been",
	"    defeated yet.",
	"",
	"",
	"",
	"Enemies will appear in",
	"the database after they",
	"have been defeated.",
	null
};

const char * HealerText[] = {
	"Stay here and rest",
	"for awhile!",
	null
};

const char * pinLocationsText[] = 
{
	[PIN_DNGN_1] = "Prison Dungeon",
	[PIN_DNGN_2] = "Desert Dungeon",
	[PIN_DNGN_3] = "Tomb Dungeon",
	[PIN_DNGN_4] = "Lake Dungeon",
	[PIN_DNGN_5] = "Final Dungeon",
	[PIN_TREASURE_CAVE] = "Treasure Cave",
	[PIN_HEALER_1] = "Healer",
	[PIN_HEALER_2] = "Healer",
	[PIN_HEALER_3] = "Healer",
	[PIN_SHOP_1] = "Item Shop",
	[PIN_SHOP_2] = "Item Shop",
	[PIN_SHOP_3] = "Item Shop",
	[PIN_SHOP_4] = "Item Shop",
	[PIN_WESTERN_PASS_ENTRANCE] = "Western Pass",
	[PIN_WESTERN_PASS_EXIT] = "Western Pass",
	[PIN_ROCKY_CAVE] = "Rock Cave",
	[PIN_OASIS_PATH_ENTRANCE] = "Eastern Pass",
	[PIN_OASIS_PATH_EXIT] = "Eastern Pass",

};

const char * creditsText[]= {
	" You have defeated the",
	" dragon,the leader of",
	" the monsters!",
	"",
	" You have also finally come",
	" back to the lands you ",
	" recognize, and can return",
	" home in peace.",
	"",
	"",
	"",
	"",
	"",
	"",
	"Programming and Game Design",
	"===========================",
	"      Nathan Tolbert",
	"         (gauauu)",
	"",
	"",
	"         Artwork",
	"    =================",
	"    Chris Hildenbrand",
	"      (Sprite Attack)",
	"",
	"",
	"    Additional Artwork",
	"    ==================",
	"    REFMAP (see readme)",
	"",
	"",
	"      Music and Sound",
	"   =====================",
	"   Jessie Tracer (Ekeet)",
	" Fred Scalliet (Magic Fred)",
	"       Nathan Tolbert",
	"",
	"",
	"  Audio Engine Programming",
	"  ========================",
	"      LibXM7 by Sverx",
	"",
	"",
	"   Code snippets, advice,",
	"  and general awesomeness",
	"  ========================",
	"     Jasper Vijn (Cearn)",
	"",
	"",
	"  DS Hardware donated by ",
	"  ====================== ",
	"         Tim Dudek",
	"         Electrobee"
	"",
	"",
	"          Testing",
	"         =========",
	"       12thAndSaturn,",
	"        Chris Lomaka,",
	"  	   Mukunda Johnson,",
	"  	    David Matthias,",
	"  	    Dietmar Pier",
	"",
	"    Plus all the wonderful",
	"    people that tested the",
	"    GBA version of Anguna",
	"",
	"",
	"",
	"",
	"",
	"   Thanks for playing!",
	null
};


