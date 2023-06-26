#ifndef KITCHEN_TICK_H
#define KITCHEN_TICK_H

//-----------------------------------------------------------------------------------------
#pragma code ( ticksCode )
#pragma data ( data )
//-----------------------------------------------------------------------------------------

#define FOOD_ITEMS_COUNT 6
enum FOOD_ITEMS {
    FOOD_SHOP_BREAD,
    FOOD_HOME_BREAD,
    FOOD_CANNED_MEAT,
    FOOD_SOUSAGE,
    FOOD_POTATOES,
    FOOD_CORN,
};

struct FoodItem {
    // char * nameIdx;
    char regenAmount;
    char regenTime;
    bool isHealing;
    char weight;
    char price;
};

extern __striped const struct FoodItem foodItems[FOOD_ITEMS_COUNT];

enum MEAL_TYPE {
    MEAL_TYPE_LIGHT = 0,
    MEAL_TYPE_MEDIUM,
    MEAL_TYPE_FULL,
};

struct Kitchen {
    char storage[FOOD_ITEMS_COUNT];
// maybe later some fridge upgrades?
    char maxStorage;
    bool bakeBreadDaily;
    MEAL_TYPE breakfastType;
    MEAL_TYPE supperType;
    bool preferHomeFood;
    bool preferCorn;
};


//-----------------------------------------------------------------------------------------
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------

void initKitchen(Kitchen * kit);
//-----------------------------------------------------------------------------------------
#pragma code ( ticksCode )
#pragma data ( data )
//-----------------------------------------------------------------------------------------

void kitchenTick();
void breakfastTick();
void supperTick();

//-----------------------------------------------------------------------------------------
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
#pragma compile("kitchenTick.c")

#endif