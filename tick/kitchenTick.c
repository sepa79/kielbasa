#include <c64/types.h>
#include <c64/vic.h>
#include <string.h>

#include <tick/kitchenTick.h>
#include <tick/farmlandTick.h>
#include <tasks/kitchenTask.h>
#include <assets/mainGfx.h>
#include <character/character.h>

// =============================================================================
// Tick code
// =============================================================================
// ticks code
#pragma code ( ticksCode )
// dynamic data - in RAM
#pragma data ( data )

__striped const struct FoodItem foodItems[FOOD_ITEMS_COUNT] = {
    // FOOD_SHOP_BREAD
    {60, 5, false, 0, 10},
    // FOOD_HOME_BREAD
    {60, 8, false, 0, 15},
    // FOOD_CANNED_MEAT
    {120, 4, true, 0, 30},
    // FOOD_SOUSAGE
    {180, 4, true, 0, 40},
    // bonus foods
    // FOOD_POTATOES
    {60, 4, false, 0, 5},
    // FOOD_CORN
    {120, 2, false, 0, 5}
};

void kitchenTick(){
    if(GS.kitchen.bakeBreadDaily) {
        addBakeBreadTask();
    }
}

// food is only dealt with for main chracter, rest 'eats' on their own
static bool _eatBread(){
    char foodItem = FOOD_SHOP_BREAD;
    if(GS.kitchen.preferHomeFood && GS.kitchen.storage[FOOD_HOME_BREAD]){
        foodItem = FOOD_HOME_BREAD;
    }
    // if there is no shop bread and its preferred, try for homemade
    if(!GS.kitchen.preferHomeFood && !GS.kitchen.storage[FOOD_SHOP_BREAD]){
        foodItem = FOOD_HOME_BREAD;
    }

    if(GS.kitchen.storage[foodItem]){
        GS.kitchen.storage[foodItem]--;
        allCharacters[0].regenAmount = foodItems[foodItem].regenAmount;
        allCharacters[0].regenTime   = foodItems[foodItem].regenTime;
        return false;
    }
    return true;
}

static bool _eatMeat(){
    char foodItem = FOOD_CANNED_MEAT;
    if(GS.kitchen.preferHomeFood && GS.kitchen.storage[FOOD_SOUSAGE]){
        foodItem = FOOD_SOUSAGE;
    }
    // if there is no shop meat and its preferred, try for homemade
    if(!GS.kitchen.preferHomeFood && !GS.kitchen.storage[FOOD_CANNED_MEAT]){
        foodItem = FOOD_SOUSAGE;
    }

    if(GS.kitchen.storage[foodItem]){
        GS.kitchen.storage[foodItem]--;
        allCharacters[0].regenAmount += foodItems[foodItem].regenAmount;
        allCharacters[0].regenTime   = allCharacters[0].regenTime   < foodItems[foodItem].regenTime   ? foodItems[foodItem].regenTime   : allCharacters[0].regenTime;
        return false;
    }
    return true;
}

static bool _eatVeggies(){
    char foodItem = FOOD_POTATOES;
    char farmItem = PLANT_POTATO;
    if(GS.kitchen.preferCorn && GS.farm.storage[PLANT_CORN]){
        foodItem = FOOD_CORN;
        farmItem = PLANT_CORN;
    }
    // if there are no potatoes and they are preferred, try for corn
    if(!GS.kitchen.preferCorn && !GS.farm.storage[PLANT_POTATO]){
        foodItem = FOOD_CORN;
        farmItem = PLANT_CORN;
    }

    if(GS.farm.storage[farmItem]){
        GS.farm.storage[farmItem]--;
        allCharacters[0].bonusAmount = foodItems[foodItem].regenAmount;
        allCharacters[0].bonusTime   = foodItems[foodItem].regenTime;
        return false;
    }
    return true;
}

static void _mealTick(MEAL_TYPE mealType){
    setCharacterSlotIcon(0, SPR_TASK_EAT);
    setCharacterSlotIcon(1, SPR_TASK_DRINK);
    setCharacterSlotIcon(2, SPR_TASK_EAT);
    setCharacterSlotIcon(3, SPR_TASK_EAT);
    // feed non-player chars
    for(byte charSlot = 1; charSlot < CHARACTER_SLOTS; charSlot++){
        if(characterSlots[charSlot] != NO_CHARACTER){
            char charIdx = characterSlots[charSlot];
            allCharacters[charIdx].regenAmount = (rand() & 15) + 120;
            allCharacters[charIdx].regenTime   = (rand() & 7) + 5;
            allCharacters[charIdx].bonusAmount = (rand() & 7) + 2;
            allCharacters[charIdx].bonusTime   = (rand() & 15) + 30;
        }
    }

    // deal with the player
    bool noMeat  = false;
    bool noVeg   = false;
    // bread is the base, always eat it
    bool noBread = _eatBread();
    // if there was no bread, auto-upgrade meal
    if(noBread && mealType == MEAL_TYPE_LIGHT){
        mealType++;
    }
    if(mealType > MEAL_TYPE_LIGHT){
        // eat some meat, too
        noMeat = _eatMeat();
    }
    // no bread, no meats? well, go for veg - don't starve!
    if(noBread && noMeat && mealType == MEAL_TYPE_MEDIUM){
        mealType++;
    }
    if(mealType > MEAL_TYPE_MEDIUM){
        // eat veggies on top
        noVeg = _eatVeggies();
    }
    
    if(noBread || noMeat || noVeg){
        char str[12*3+1];
        char row2[13] = s"            ";
        char row3[13] = s"            ";
        bool needRow3 = true;
        if(noBread && noMeat){
            sprintf(row2, "%s %s", TXT[TXT_IDX_KITCHEN_MISSING_BREAD], TXT[TXT_IDX_KITCHEN_MISSING_MEAT]);
        } else if (noBread) {
            sprintf(row2, "%s", TXT[TXT_IDX_KITCHEN_MISSING_BREAD]);
        } else if (noMeat) {
            sprintf(row2, "%s", TXT[TXT_IDX_KITCHEN_MISSING_MEAT]);
        } else if (noVeg){
            sprintf(row2, "%s", TXT[TXT_IDX_KITCHEN_MISSING_VEG]);
            needRow3 = false;
        }
        if(needRow3 && noVeg){
            sprintf(row3, "%s", TXT[TXT_IDX_KITCHEN_MISSING_VEG]);
        }
        sprintf(str, "%s%12s%12s", TXT[TXT_IDX_KITCHEN_MISSING], row2, row3);
        updateStatusBarError(str);
    }
    // debug
    // char str[12*3+1];
    // sprintf(str, "%3d   %3d   %3d   %3d   ", allCharacters[0].regenAmount, allCharacters[0].regenTime, allCharacters[0].bonusAmount, allCharacters[0].bonusTime);
    // updateStatusBar(str);
    updateMenu();
}


void breakfastTick(){
    _mealTick(GS.kitchen.breakfastType);
}

void supperTick(){
    _mealTick(GS.kitchen.supperType);
}

//-----------------------------------------------------------------------------------------
// In Init bank
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------

void initKitchen(Kitchen * kit){
    kit->storage[FOOD_SHOP_BREAD]  = 1;
    kit->storage[FOOD_HOME_BREAD]  = 0;
    kit->storage[FOOD_CANNED_MEAT] = 2;
    kit->storage[FOOD_SOUSAGE]     = 0;
    kit->maxStorage = 4;
    kit->bakeBreadDaily = false;
    kit->breakfastType = MEAL_TYPE_LIGHT;
    kit->supperType = MEAL_TYPE_LIGHT;
    kit->preferHomeFood = false;
    kit->preferCorn = false;
}

//-----------------------------------------------------------------------------------------
// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
