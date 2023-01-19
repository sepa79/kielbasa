#ifndef KITCHEN_TICK_H
#define KITCHEN_TICK_H

//-----------------------------------------------------------------------------------------
#pragma code ( ticksCode )
#pragma data ( data )
//-----------------------------------------------------------------------------------------

#define FOOD_ITEMS_COUNT 4
#define FOOD_SHOP_BREAD 0
#define FOOD_HOME_BREAD 1
#define FOOD_CANNED_MEAT 2
#define FOOD_SOUSAGE 3

struct FOOD_ITEM {
    char * nameIdx;
    char food;
    char energy;
    char health;
    char weight;
    char price;
};

extern char kit_storage[FOOD_ITEMS_COUNT];

//-----------------------------------------------------------------------------------------
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------

void initKitchen();

//-----------------------------------------------------------------------------------------
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
#pragma compile("kitchenTick.c")

#endif