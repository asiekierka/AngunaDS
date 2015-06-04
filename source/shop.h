#ifndef _SHOP_H_
#define _SHOP_H_

#include "misc.h"

typedef struct
{

	int item;
	int price;
	int saveIfLimited;
	int saveOnBuy;

} ShopItemDef;

typedef struct
{

	int numItems;
	ShopItemDef ** shopItems;

} ShopDef;


void doShop(int shopId);
void doShopByDef(ShopDef * shop);

bool didBuyDynamite();
bool didBuyTraps();


#endif
