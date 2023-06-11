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
    kit->maxStorage = 10;
}
//-----------------------------------------------------------------------------------------
// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
