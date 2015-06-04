#include "item.h"
#include "items.h"
#include "level.h"
#include "mainChar.h"
#include "bullet.h"
#include "characters.h"
#include "enemies/itemGfx.h"
#include "darkness.h"
#include "sound.h"
#include "textData.h"
#include "enemies.h"
#include "shop.h"
#include "minimap.h"
#include "misc.h"
#include "platform.h"

//notice that we include ITEM.H, singular. yeah, it's tricky and messy.  basically:
//1.  item.h defines basic things having to do with items in general
//2.  enemies/item.c defines items as game-objects that appear, you can run into, etc
//3.  this items.c deals with USING items, and having them in inventory 

#define MAX_CALTROPS_ON_SCREEN 2
#define MAX_DYNAMITE_ON_SCREEN 1
#define DYNAMITE_TIME_TO_EXPLODE 60  //the time from when the dynamite is set until it explodes
#define DYNAMITE_EXPLODE_TIME 30  //total time of the explosion
#define CALTROP_TRAP_TIME 180
#define RING_BONUS 6


int Inventory[NUM_ITEMS];
int InvCurrItem;
int g_caltrops;
int g_dynamite;

bool itemBowUse();
bool itemHeldItemUse();
bool itemPotionUse();
int itemBowPickup();
int itemGoldPickup();
int itemGoldBagPickup();
int itemSmallBagPickup();
int itemArrowsPickup();
void itemLanternSwitchFrom();
void itemLanternSwitchTo();
void itemBootsSwitchFrom();
void itemBootsSwitchTo();
void itemAttackRingSwitchFrom();
void itemAttackRingSwitchTo();
bool itemCaltropsUse();
int itemCaltropsPickup();
int itemDynamitePickup();
bool itemDynamiteUse();
int itemMapDngn2Pickup();
int itemMapDngn3Pickup();
int itemMapDngn4Pickup();

#define QTY_PICKUP(x,y) {null, true, null, (y), (x), 0, null, null}
#define QTY_ONLY(x) QTY_PICKUP(null,x)
#define PICKUP_ONLY(x) {null, false, null, 0, (x), 0, null, null}
#define HOLD_ONLY(x,y) {itemHeldItemUse, false, null, 1, null, 0, (x), (y)}

#define ARROW_TILE 192
#define CALTROP_TILE 152
#define DYNAMITE_TILE 197

const Item itemDefs[] = {

	[ITEM_NONE]       = {null, false, null, 0, null},
	[ITEM_KEY_RED]    = QTY_ONLY(99),
	[ITEM_KEY_YELLOW] = QTY_ONLY(99),
	[ITEM_KEY_BLUE]   = QTY_ONLY(99),
	[ITEM_MEAT]       = PICKUP_ONLY(mainCharMeatPickup),
	[ITEM_ATTACK_UP]  = PICKUP_ONLY(mainCharAttackUp),
	[ITEM_LIFE_UP]    = PICKUP_ONLY(mainCharLifeUp),
	[ITEM_DEFENCE_UP] = PICKUP_ONLY(mainCharDefenceUp),
	[ITEM_BOW]        = {itemBowUse, false, ITEM_ARROWS, 1, itemBowPickup, 20},
	[ITEM_ARROWS]     = QTY_PICKUP(itemArrowsPickup, 255),
	[ITEM_LANTERN]    = HOLD_ONLY(itemLanternSwitchFrom, itemLanternSwitchTo),
	[ITEM_POTION]     = {itemPotionUse, true, null, 3, null, 60},
	[ITEM_BOOTS]      = HOLD_ONLY(itemBootsSwitchFrom, itemBootsSwitchTo),
	[ITEM_CALTROPS]   = {itemCaltropsUse, true, null, 255,itemCaltropsPickup, 1},
	[ITEM_GOLD]		  = PICKUP_ONLY(itemGoldPickup),
	[ITEM_ATTACK_RING]= HOLD_ONLY(itemAttackRingSwitchFrom, itemAttackRingSwitchTo),
	[ITEM_DYNAMITE]   = {itemDynamiteUse, true, null, 255, itemDynamitePickup, 20},
	[ITEM_GOLD_BAG]   = PICKUP_ONLY(itemGoldBagPickup),
	[ITEM_SMALL_BAG]   = PICKUP_ONLY(itemSmallBagPickup),
	[ITEM_MAP_DNGN_2]   = PICKUP_ONLY(itemMapDngn2Pickup),
	[ITEM_MAP_DNGN_3]   = PICKUP_ONLY(itemMapDngn3Pickup),
	[ITEM_MAP_DNGN_4]   = PICKUP_ONLY(itemMapDngn4Pickup),

};

int itemCoolDown;

void resetBulletItems()
{
	g_caltrops = 0;
	g_dynamite = 0;
}

void itemCoolDownTick()
{
	if (itemCoolDown > 0)
	{
		itemCoolDown--;
	}
}

void resetItemCoolDown()
{
	itemCoolDown = 0;
}

bool isItemCooled()
{
	return (itemCoolDown == 0);
}

void pickupItem(int item)
{
	ItemPickupFunction pFunc = itemDefs[item].pickupFunction;
	if ((pFunc == null) || (pFunc()))
	{
		adjustInventory(item, 1);
	}
	
}

void clearInventory()
{
	int i;
	for (i = 0; i < NUM_ITEMS; i++)
	{
		Inventory[i] = 0;
	}
	InvCurrItem = ITEM_NONE;
}

bool useCurrentItem()
{

	// actually use the item.....
	bool used = false;

	ItemUseFunction function = itemDefs[InvCurrItem].useFunction;		
	if ((function != null) && (isItemCooled()))
	{
		if(function())	
		{
			itemCoolDown = itemDefs[InvCurrItem].coolDown;
			used = true;
		}
	}

	if (getCurrentItemQty() == 0)
	{
		selectNextItem();
	}
	
	return used;
}

void switchItemFrom(int item)
{
	const Item * def = &(itemDefs[item]);
	
	if (def->switchFrom != null)
	{
		def->switchFrom();
	}
	
}

void switchItemTo(int item)
{
	const Item * def = &(itemDefs[item]);
	
	if (def->switchTo != null)
	{
		def->switchTo();
	}
	
}

bool selectNextItem()
{
	if (InvCurrItem == ITEM_NONE)
	{
		InvCurrItem = NUM_ITEMS - 1;
	}
	int startItem = InvCurrItem;
	do
	{
		InvCurrItem++;
		if (InvCurrItem > NUM_ITEMS - 1)
		{
			InvCurrItem = 0;
		}

	}while
	  (((getCurrentItemQty() == 0) || (itemDefs[InvCurrItem].useFunction == null))
		    && (InvCurrItem != startItem));

	if ((getCurrentItemQty() == 0) || (itemDefs[InvCurrItem].useFunction == null))
	{
		InvCurrItem = ITEM_NONE;
	}

	if ((InvCurrItem == ITEM_NONE) || (InvCurrItem == startItem))
	{
		return false;
	}

	resetItemCoolDown();
	switchItemFrom(startItem);
	switchItemTo(InvCurrItem);
	return true;
	
}

bool selectPrevItem()
{
	if (InvCurrItem == ITEM_NONE)
	{
		InvCurrItem = 0;
	}
	int startItem = InvCurrItem;

	do
	{
		InvCurrItem--;
		if (InvCurrItem < 0)
		{
			InvCurrItem = NUM_ITEMS - 1;
		}

	}while
	  (((getCurrentItemQty() == 0) || (itemDefs[InvCurrItem].useFunction == null))
		    && (InvCurrItem != startItem));

	if (getCurrentItemQty() == 0)
	{
		InvCurrItem = ITEM_NONE;
	}

	if ((InvCurrItem == ITEM_NONE) || (InvCurrItem == startItem))
	{
		return false;
	}

	resetItemCoolDown();

	switchItemFrom(startItem);
	switchItemTo(InvCurrItem);
	
	return true;
}

int getCurrentItem()
{
	return InvCurrItem;
}

int getItemQty(int item)
{
	return Inventory[item];
}

int getCurrentItemQty()
{
	return getItemQty(getCurrentItem());
}

int getItemUsableQty(int item)
{
	if (itemDefs[item].hasCount)
	{
		return getItemQty(item);
	}

	if (itemDefs[item].associatedCountItem != null)
	{
		return getItemUsableQty(itemDefs[item].associatedCountItem);
	}

	return ITEM_NO_COUNT;
	
}

void adjustInventory(int item, int qty)
{
	Inventory[item] += qty;
	Inventory[item] = max(Inventory[item], 0);
	Inventory[item] = min(Inventory[item], itemDefs[item].max);

	//if they don't have an item selected, cycle through and see if they can select 
	//this new item
	if (getCurrentItem() == ITEM_NONE)
	{
		selectNextItem();
	}
}

bool canHoldMore(int item)
{
	int max = itemDefs[item].max;
	if (max == 0)
	{
		return true;
	}


	return (Inventory[item] < max);
}

void getAllItems()
{
	//for cheating/testing
	int i;
	for (i = 0; i < NUM_ITEMS; i++)
	{
		if (itemDefs[i].hasCount)
		{
			adjustInventory(i, 255);
		}
		else
		{
			adjustInventory(i, 1);
		}
	}
	
}

int getNumSubscreenItems()
{
	int i;
	int iCtr = 0;
	for(i = 0; i < NUM_ITEMS; i++)
	{
		if (getItemQty(i) > 0)
		{
			Item * item = getItemDef(i);
			if (item->useFunction != null)
			{
				iCtr++;
			}
		}
	}
	
	return iCtr;	
}

int subscreenToItemIndex(int index)
{
	//poor man's mod, but we're dealing with small multiples
	//so it should be faster than a general case mod
	int numSubItems = getNumSubscreenItems();
	
	if (numSubItems < 1)
	{
		return -1;
	}
	
	while (index >= numSubItems)
	{
		index -= numSubItems;
	}	
	
	int i;
	int iCtr = 0;
	for(i = 0; i < NUM_ITEMS; i++)
	{
		if (getItemQty(i) > 0)
		{
			Item * item = getItemDef(i);
			if (item->useFunction != null)
			{
				if (iCtr == index)
				{
					return i;
				}
				iCtr++;
			}
		}
	}
	
	return -1;
}

int itemToSubscreenIndex(int index)
{
	int i;
	int iCtr = 0;
	for(i = 0; i < NUM_ITEMS; i++)
	{
		if (getItemQty(i) > 0)
		{
			Item * item = getItemDef(i);
			if (item->useFunction != null)
			{
				if (index == i)
				{
					return iCtr;
				}
				iCtr++;
			}
			else
			{
				if (index == i)
				{
					return -1;
				}
			}
		}
	}
	
	return -1;	
}

int itemArrowsPickup()
{
	adjustInventory(ITEM_ARROWS, 5);
	return 0;
}

int itemGoldPickup()
{
	addMainCharGold(1);
	return 0;
}

int itemGoldBagPickup()
{
	addMainCharGold(200);
	return 0;
}

int itemSmallBagPickup()
{
	addMainCharGold(5);
	return 0;
}

int itemBowPickup()
{
	//the bow gives you +10 arrows on pickup
	adjustInventory(ITEM_ARROWS, 10);
	return 1;
}

bool itemBowUse()
{
	if (getItemQty(ITEM_ARROWS) < 1)
	{
		return false;
	}
	if (isMCAttacking())
	{
		return false;
	}
	
	playGameSfx(SFX_ARROW);
	

	adjustInventory(ITEM_ARROWS, -1);

	BulletDef bullet;

	bullet.ttl = 999;
	bullet.xSpeed = mcXFacing() * 3;
	bullet.ySpeed = mcYFacing() * 3;
	bullet.xFrameskip = 1;
	bullet.yFrameskip = 1;
	bullet.team = TEAM_HERO;
	bullet.pierce = false;
	bullet.power = getMainCharPower();
	CharacterStatus * mcStatus = getMainCharStatus();
	bullet.sourceX = centerX(mcStatus);
	bullet.sourceY = centerY(mcStatus);
	bullet.x = mcStatus->x;
	bullet.y = mcStatus->y - 4;
	bullet.height = 8;
	bullet.width = 8;
	bullet.sprite = null;

	if (abs(bullet.xSpeed) > 0)
	{
		bullet.tile = ARROW_TILE;
	}
	else
	{
		bullet.tile = ARROW_TILE + 2;
	}


	BulletDef * actualBullet = newBullet(&bullet);
	if (actualBullet != null)
	{
		Sprite * sprite = actualBullet->sprite;
		if (bullet.xSpeed > 0)
		{
			setSpriteFlip(sprite, true);
		}
		if (bullet.ySpeed > 0)
		{
			setSpriteVFlip(sprite, true);
		}

		if (abs(bullet.xSpeed) > 0)
		{
			setSpriteShape(sprite, WIDE);
		}
		else
		{
			setSpriteShape(sprite, TALL);
		}

		setSpritePalette(sprite, findPalette((u16*)itemsPal));
	}

	return true;

}

Item * getItemDef(int itemId)
{
	return (Item*)&(itemDefs[itemId]);
}

bool itemHeldItemUse()
{
	return true;
}

void itemLanternSwitchFrom()
{
	if (isRoomDark())
	{
		activateDarkMode();
	}
}

void itemLanternSwitchTo()
{
	if (isRoomDark())
	{
		deactivateDarkMode();
	}
}

bool itemPotionUse()
{
	if (getItemQty(ITEM_POTION) < 1)
	{
		return false;
	}
	if (isMCAttacking())
	{
		return false;
	}
	
	adjustInventory(ITEM_POTION, -1);
	
	playGameSfx(SFX_POTION);
	mcAddHP(99999);
	return true;
}

void itemBootsSwitchFrom()
{
	CharacterStatus * mc = getMainCharStatus();
	mc->mobility = MOBILITY_NORMAL;
}

void itemBootsSwitchTo()
{
	CharacterStatus * mc = getMainCharStatus();
	mc->mobility = MOBILITY_FLYING;
}

void itemAttackRingSwitchFrom()
{
	setAttackBonus(0);
}

void itemAttackRingSwitchTo()
{
	setAttackBonus(RING_BONUS);
}

void itemCaltropsDead(int why, void * target)
{
	g_caltrops--;
	g_caltrops = max(0,g_caltrops);

	if ((why == BULLET_HIT) && (target != null))
	{
		Enemy * enemyHit = (Enemy *)target;
		enemyHit->trapped = max(enemyHit->trapped, CALTROP_TRAP_TIME);
	}
}

bool itemCaltropsUse()
{
	if (getItemQty(ITEM_CALTROPS) < 1)
	{
		return false;
	}
	if (isMCAttacking())
	{
		return false;
	}
	if (g_caltrops >= MAX_CALTROPS_ON_SCREEN)
	{
		return false;
	}
	
	adjustInventory(ITEM_CALTROPS, -1);

	BulletDef bullet;

	bullet.ttl = 999999;
	bullet.xSpeed = 0;
	bullet.ySpeed = 0;
	bullet.xFrameskip = 1;
	bullet.yFrameskip = 1;
	bullet.team = TEAM_HERO;
	bullet.pierce = false;
	bullet.power = getMainCharPower() >> 1;
	CharacterStatus * mcStatus = getMainCharStatus();
	bullet.sourceX = centerX(mcStatus) - 7;
	bullet.sourceY = centerY(mcStatus) - 7;
	bullet.x = mcStatus->x;
	bullet.y = mcStatus->y - 4;
	bullet.height = 14;
	bullet.width = 14;
	bullet.sprite = null;

	bullet.tile = CALTROP_TILE;


	BulletDef * actualBullet = newBullet(&bullet);
	
	if (actualBullet != null)
	{
		actualBullet->deadFunction = itemCaltropsDead;

		Sprite * sprite = actualBullet->sprite;

		setSpritePalette(sprite, findPalette((u16*)itemsPal));
		setSpriteSize(sprite, SIZE_16);
		setSpriteShape(sprite, SQUARE);
		
		g_caltrops++;
	}

	return true;

	
}

int itemCaltropsPickup()
{
	adjustInventory(ITEM_CALTROPS, 5);
	return 0;
}

int itemDynamitePickup()
{
	int num = 3;
	if (didBuyDynamite())
	{
		num = 1;
	}
	adjustInventory(ITEM_DYNAMITE, num);
	return 0;
}

void itemDynamiteDead(int why, void * target)
{
	g_dynamite--;
	g_dynamite = max(0,g_dynamite);

	/*
	//this shouldn't be called, since dynamite has pierce....
	if ((why == BULLET_HIT) && (target != null))
	{
		Enemy * enemyHit = (Enemy *)target;
		enemyHit->enemyType->updateFunction(enemyHit, ENEMY_DYNAMITED);
	}
	*/
}

void itemDynamiteUpdate(BulletDef * bullet)
{
	if (bullet->ttl == DYNAMITE_EXPLODE_TIME)
	{
		//explode the dynamite here....
		bullet->team = TEAM_HERO;
		//update to first frame of explosion
		playGameSfx(SFX_DYNAMITE);
		bullet->tile = DYNAMITE_TILE + 2;
		setSpriteStartTile(bullet->sprite, bullet->tile );
		bullet->spriteOffsetX = 12;
		bullet->spriteOffsetY = 12;
		bullet->x -= 12;
		bullet->y -= 12;
		bullet->width += 24;
		bullet->height += 24;
	}

	if (bullet->ttl < DYNAMITE_EXPLODE_TIME)  {
		if ((bullet->ttl & 0x07) == 1)
		{
			bullet->tile += 2;
			setSpriteStartTile(bullet->sprite, bullet->tile);
		}
	}


}


bool itemDynamiteUse()
{
	if (getItemQty(ITEM_DYNAMITE) < 1)
	{
		return false;
	}
	if (isMCAttacking())
	{
		return false;
	}
	if (g_dynamite >= MAX_DYNAMITE_ON_SCREEN)
	{
		return false;
	}
	
	adjustInventory(ITEM_DYNAMITE, -1);

	BulletDef bullet;

	bullet.ttl = DYNAMITE_TIME_TO_EXPLODE + DYNAMITE_EXPLODE_TIME;
	bullet.xSpeed = 0;
	bullet.ySpeed = 0;
	bullet.xFrameskip = 1;
	bullet.yFrameskip = 1;
	bullet.team = TEAM_NONE;
	bullet.pierce = true;
	bullet.power = getMainCharPower() << 2;
	CharacterStatus * mcStatus = getMainCharStatus();
	bullet.sourceX = centerX(mcStatus) - 7;
	bullet.sourceY = centerY(mcStatus) - 7;
	bullet.x = mcStatus->x;
	bullet.y = mcStatus->y - 4;
	bullet.height = 14;
	bullet.width = 14;
	bullet.sprite = null;

	bullet.tile = DYNAMITE_TILE;


	BulletDef * actualBullet = newBullet(&bullet);
	
	actualBullet->deadFunction = itemDynamiteDead;
	actualBullet->updateFunction = itemDynamiteUpdate;

	Sprite * sprite = actualBullet->sprite;

	setSpritePalette(sprite, findPalette((u16*)itemsPal));
	setSpriteSize(sprite, SIZE_16);
	setSpriteShape(sprite, SQUARE);
	
	g_dynamite++;

	return true;

	
}

char * getItemName(int itemId)
{
	return (char*)ItemNames[itemId];
}

int itemMapDngn2Pickup()
{
	purchasedMap(ITEM_MAP_DNGN_2);
	return 0;
}

int itemMapDngn3Pickup()
{
	purchasedMap(ITEM_MAP_DNGN_3);
	return 0;
}

int itemMapDngn4Pickup()
{
	purchasedMap(ITEM_MAP_DNGN_4);
	return 0;
}

