#include <c64/types.h>

#include <translation/common.h>
#include <character/character.h>

// =============================================================================
// Tick code
// =============================================================================
// ticks code
#pragma code ( ticksCode )
// dynamic data - in RAM
#pragma data ( data )

void sleepTick(){
    for(byte charSlot = 0; charSlot < CHARACTER_SLOTS; charSlot++){
        if(characterSlots[charSlot] != NO_CHARACTER){
            word rnd = rand();
            byte rnd0 = (rnd & 15) + 5;
            incEnergyLevel(charSlot, rnd0);
        }
    }
}

void miaTick(){
    for(byte charSlot = 0; charSlot < CHARACTER_SLOTS; charSlot++){
        if(characterSlots[charSlot] != NO_CHARACTER){
            // check if character is not nusy (mia)
            byte charIdx = characterSlots[charSlot];
            if(allChars_busy[charIdx] == false){
                word rnd = rand();
                byte rnd0 = (rnd & 7) + 3;
                decEnergyLevel(charSlot, rnd0);
            }
        }
    }
}

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
