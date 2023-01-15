#include <c64/types.h>
#include <c64/vic.h>

#include <tick/kitchenTick.h>

// =============================================================================
// Tick code
// =============================================================================
// ticks code
#pragma code ( ticksCode )
// dynamic data - in RAM
#pragma data ( data )

char kit_storage[FOOD_ITEMS_COUNT] = {0};

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )

void initKitchen(){
    kit_storage[FOOD_SHOP_BREAD]  = 0;
    kit_storage[FOOD_HOME_BREAD]  = 100;
    kit_storage[FOOD_CANNED_MEAT] = 255;
    kit_storage[FOOD_SOUSAGE]     = 0;
}
