#include <c64/types.h>

#include <character/character.h>

// =============================================================================
// Tick code
#pragma code ( ticksCode )
// dynamic data - in RAM
#pragma data ( data )
// =============================================================================

void sleepTick(){
    for(byte charSlot = 0; charSlot < CHARACTER_SLOTS; charSlot++){
        if(characterSlots[charSlot] != NO_CHARACTER){
            char charIdx = characterSlots[charSlot];
            word rnd = rand();
            byte rnd0 = 0;
            // hungry ppl do not regen much
            if(allCharacters[charIdx].regenTime){
                rnd0 = (rnd & 7) + 5;
            } else {
                rnd0 = (rnd & 3) + 1;
            }
            incEnergyLevel(characterSlots[charSlot], rnd0);
        }
    }
}

void miaTick(){
    for(byte charSlot = 0; charSlot < CHARACTER_SLOTS; charSlot++){
        if(characterSlots[charSlot] != NO_CHARACTER){
            // check if character is not busy
            if(allCharacters[characterSlots[charSlot]].busy == false){
                word rnd = rand();
                byte rnd0 = (rnd & 3) + 1;
                decEnergyLevel(characterSlots[charSlot], rnd0);
            }
        }
    }
}

void regenTick(){
    for(byte charSlot = 0; charSlot < CHARACTER_SLOTS; charSlot++){
        if(characterSlots[charSlot] != NO_CHARACTER){
            char charIdx = characterSlots[charSlot];
            char regen = 0;
            if(allCharacters[charIdx].regenTime){
                allCharacters[charIdx].regenTime--;
                regen += allCharacters[charIdx].regenAmount;
                // clean up if we reduced timer to 0
                if(!allCharacters[charIdx].regenTime){
                    allCharacters[charIdx].regenAmount = 0;
                }
            }
            if(allCharacters[charIdx].bonusTime){
                allCharacters[charIdx].bonusTime--;
                regen += allCharacters[charIdx].bonusAmount;
                // clean up if we reduced timer to 0
                if(!allCharacters[charIdx].bonusTime){
                    allCharacters[charIdx].bonusAmount = 0;
                }
            }
            incEnergyLevel(charIdx, regen);
        }
    }
}
//-----------------------------------------------------------------------------------------
// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
