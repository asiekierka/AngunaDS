#ifndef _ITEMS_H_
#define _ITEMS_H_

#include "item.h"

#define ITEM_FIRST_USABLE ITEM_BOW
#define ITEM_LAST_USABLE ITEM_BOW

#define ITEM_NO_COUNT -1

bool useCurrentItem();
bool selectNextItem();
bool selectPrevItem();
int getCurrentItem();
int getCurrentItemQty();
int getItemQty(int item);
int getItemUsableQty(int item);
void clearInventory();
void adjustInventory(int item, int qty);
void getAllItems();
void pickupItem(int item);
void itemCoolDownTick();
int getNumSubscreenItems();
int subscreenToItemIndex(int index);
int itemToSubscreenIndex(int index);
char * getItemName(int itemId);
bool canHoldMore(int item);
void resetBulletItems();


typedef bool(*ItemUseFunction)();
typedef int(*ItemPickupFunction)();
typedef void(*ItemSwitchFunction)();

typedef struct 
{
	ItemUseFunction useFunction;
	bool hasCount;
	int associatedCountItem;
	int max;
	ItemPickupFunction pickupFunction;
	int coolDown;
	ItemSwitchFunction switchFrom;
	ItemSwitchFunction switchTo;
	
} Item;

Item * getItemDef(int itemId);

#endif
