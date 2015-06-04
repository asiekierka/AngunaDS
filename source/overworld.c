#include "bgDefines.h"
#include "level.h"
#include "overworld.h"
//#include "gfx/levelGfx/overworld.h"
#include "bg.h"

//tons of magic numbers and nastiness.  Could put them in a config file to 
//autogenerate this source, but it's a lot of work for a one-off solution.
//So here is the collision checking for the overworld graphics, in all it's nasty 
//glory....wow, it's getting uglier by the minute.  BAD DESIGN!  No Biscuit!
//(otherwise, could hold a whole data file for it, but that's a lot of data also)

#define TILE(x) ((x)*4)
#define CHIP(x,z) (TILE(x)+(z))
#define O_BLANK (TILE(1))
#define O_NORMAL (TILE(46))
#define O_TRUNKS (TILE(54))
#define O_WATER (TILE(121))
#define O_OVERLAY (TILE(178))
#define R_NORMAL (TILE(42))

extern const u16 * overworld_Tiles[];
extern const u16 * overworld2_Tiles[];
extern const u16 * overworldGraveyard_Tiles[];
extern const u16 * cave_Tiles[];


bool isNormalOverworldTiles(LevelData * currentLevel)
{
	if ((void *)currentLevel->tileGfx == (void*)overworld_Tiles) 
	{
		return true;
	}
	return false;
}

bool isCaveTiles(LevelData * currentLevel)
{
	if ((void *)currentLevel->tileGfx == (void*)cave_Tiles) 
	{
		return true;
	}
	return false;
}

bool isGraveyardTiles(LevelData * currentLevel)
{
	if ((void *)currentLevel->tileGfx == (void*)overworldGraveyard_Tiles) 
	{
		return true;
	}
	return false;
}

bool isOverworld2Tiles(LevelData * currentLevel)
{
	if ((void *)currentLevel->tileGfx == (void*)overworld2_Tiles) 
	{
		return true;
	}
	//overworldGraveyard uses (almost) the same tiles, and palette order as overworld2, so 
	//let's cheat and return true if we're using it as well....
	if ((void *)currentLevel->tileGfx == (void*)overworldGraveyard_Tiles) 
	{
		return true;
	}
	return false;
}

bool isNormalOverworldBridge(int tile)
{		
	if ((tile >= 348) && (tile <= 407))
	{
		int tmp = tile - 336;
		tmp >>= 2;
		switch (tmp)
		{
			case 3:
			case 4:
			case 5:
			case 9:
			case 10:
			case 11:
			case 15:
			case 16:
			case 17:
				return true;
		}

	}
	return false;

}

u16 applyNormalOverworldPalette(int tile)
{
	if (tile >= 288)
	{
		if (isNormalOverworldBridge(tile))
		{
			return tile | BGPAL(4);
		}

		if (tile >= 480)
		{
			if (tile >= 720)
			{
				return tile | BGPAL(2);
			}
			return tile | BGPAL(3);
		}

		return tile | BGPAL(1);
	}
	return tile | BGPAL(0);
}

u16 applyRockyOverworldPalette(int tile)
{

	if ((tile > R_NORMAL) && (tile < R_NORMAL + 48))
	{
		return tile | BGPAL(4);
	}
	if ((tile > R_NORMAL + 60) && (tile < R_NORMAL + 72))
	{
		return tile | BGPAL(4);
	}
				
	if (tile >= 288)
	{

		if (tile >= 480)
		{
			return tile | BGPAL(2);
		}
		return tile | BGPAL(0);

	}
	return tile | BGPAL(1);
}

u16 applyOverworld2Palette(int tile)
{
	if (isNormalOverworldBridge(tile))
	{
		return tile | BGPAL(4);
	}
	if (tile >= 288)
	{
		if (tile >= 480)
		{
			return tile | BGPAL(2);
		}

		return tile | BGPAL(1);
	}
	return tile | BGPAL(0);
}

u16 applyCavePalette(int tile)
{
	if (tile < 192)
	{
		return tile | BGPAL(1);
	}
	else
	{
		return tile | BGPAL(0);
	}
}


u16 applyOverworldPalette(LevelData * level, int tile)
{
	if (isNormalOverworldTiles(level)) 
	{
		return applyNormalOverworldPalette(tile);
	}
	if (isCaveTiles(level)) 
	{
		return applyCavePalette(tile);
	}
	if (isOverworld2Tiles(level))
	{
		return applyOverworld2Palette(tile);
	}
	return applyRockyOverworldPalette(tile);

}


bool isCaveWallCollision(LevelData * currentLevel, int x, int y, int mobility)
{
	int tile = getTileAtWorldCoord(x,y);
	if ((tile < 4) && (mobility == MOBILITY_NORMAL))
	{
		return true;
	}
	if (tile < 120)
	{
		return false;
	}
	if ((tile < 192) && (mobility == MOBILITY_NORMAL))
	{
		return true;
	}
	if (tile < 336)
	{
		return false;
	}

	return true;
}

//we also do graveyard collision here, since they are almost the same thing
bool isOverworld2WallCollision(LevelData * currentLevel, int x, int y, int mobility)
{
	int tile = getTileAtWorldCoord(x,y);
	if (tile < O_BLANK)
	{
		//empty is like water
		tile = 100;
	}
	if (isGraveyardTiles(currentLevel))
	{
		switch (tile)
		{
			case CHIP(42,0):
			case CHIP(42,1):
			case CHIP(42,2):
			case CHIP(42,3):
			case CHIP(43,0):
			case CHIP(43,1):
			case CHIP(43,2):
			case CHIP(43,3):
			case CHIP(44,0):
			case CHIP(44,1):
			case CHIP(44,2):
			case CHIP(44,3):
			case CHIP(45,0):
			case CHIP(45,2):
			case CHIP(46,1):
			case CHIP(46,3):
			case CHIP(51,0):
			case CHIP(51,2):
			case CHIP(52,1):
			case CHIP(52,3):
				return true;

		}
		if (tile < 216)
		{
			return false;
		}
	}
	else
	{
		if (tile < O_NORMAL)
		{
			//normal land
			return false;
		}
	}


	if (isNormalOverworldBridge(tile))
	{
		switch (tile)
		{
			case 348:
			case 349:
			case 352:
			case 353:
				return true;
		}
		if (tile > 396)
		{
			return true;
		}
		return false;	
	}
	if (tile < O_WATER)
	{
		switch(tile)
		{
			case CHIP(74,0):
			case CHIP(76,1):
			case CHIP(80,2):
				return false;
			case CHIP(82,3):
				return (mobility <= MOBILITY_NORMAL);
		}

		if (mobility > MOBILITY_NORMAL)
		{
			return false;
		}
		return true;
	}
	if (tile < (O_WATER + 48))
	{
		return false;
	}
	return true;
}

extern const ZoneData zone_cave;

bool isOutdoorWallCollision(LevelData * currentLevel, int x, int y, int mobility)
{
	if (isCaveTiles(currentLevel))
	{
		return isCaveWallCollision(currentLevel, x, y, mobility);
	}
	if (isOverworld2Tiles(currentLevel))
	{
		return isOverworld2WallCollision(currentLevel, x, y, mobility);
	}

	int tile = getTileAtWorldCoord(x,y);
	if (isNormalOverworldTiles(currentLevel)) 
	{


		if (tile < O_BLANK)
		{
			//empty is like water
			tile = 100;
		}
		if (tile < O_NORMAL)
		{
			//normal land
			return false;
		}
		if (tile < O_TRUNKS)
		{
			switch(tile)
			{
				case CHIP(48,1):
					return true;
				case CHIP(48,3): 
					return true;
				case CHIP(49,0):
					return true;
				case CHIP(49,2):
					return true;
				case CHIP(50,1):
					return true;
				case CHIP(50,3): 
					return true;
				case CHIP(51,0):
					return true;
				case CHIP(51,2):
					return true;
				case CHIP(52,1):
					return true;
				case CHIP(52,3): 
					return true;
				case CHIP(53,0):
					return true;
				case CHIP(53,2):
					return true;
				default:
					return false;
		
			}
			//screwy tree trunks
		}
		if (tile < O_WATER)
		{
			if (isNormalOverworldBridge(tile))
			{
				switch (tile)
				{
					case 348:
					case 349:
					case 352:
					case 353:
						return true;
				}
				if (tile > 396)
				{
					return true;
				}
				return false;	
			}
			//water
			switch(tile)
			{
				case CHIP(74,0):
				case CHIP(76,1):
				case CHIP(80,2):
					return false;
				case CHIP(82,3):
					return (mobility <= MOBILITY_NORMAL);
			}

			if (mobility > MOBILITY_NORMAL)
			{
				return false;
			}
			return true;
		}
		if (tile < O_OVERLAY)
		{
			//overlay trees
			return false;
		}
		//cave stuff
		int original = tile >> 2;
		switch(original)
		{
			case (181):
			case (182):
			case (183):
			//case (186):
			//case (192):
			//case (198):
			case (219):
			case (220):
				return false;
		}
		if (currentLevel->zone == &zone_cave)
		{
			switch(original)
			{
				case (186):
				case (192):
				case (198):
					return false;
			}
		}
		return true;
	}
	else
	{
		//if it's the rocky tiles....
		if (tile > R_NORMAL) 
		{
			if (tile < O_WATER)
			{
				if (tile < R_NORMAL + 48)
				{
					switch (tile){
						case (R_NORMAL + 12):
						case (R_NORMAL + 13):
						case (R_NORMAL + 16):
						case (R_NORMAL + 17):
						case (R_NORMAL + 20):
						case (R_NORMAL + 21):
							return true;
					}
					return false;
				}


				if (mobility > MOBILITY_NORMAL)
				{
					return false;
				}
				return true;
			}
			//cave stuff
			int original = tile >> 2;
			switch(original)
			{
				case (181):
				case (182):
				case (183):
				//case (186):
				//case (192):
				//case (198):
				case (219):
				case (220):
				case (163):
				case (164):
					return false;
			}
			return true;
		}
		return false;


	}

	//return isDungeonWallCollision(x, y, mobility);

}

