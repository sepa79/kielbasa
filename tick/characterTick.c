#include <c64/types.h>
#include <c64/vic.h>

#include <character/character.h>

// =============================================================================
// Tick code
#pragma code ( ticksCode )
// dynamic data - in RAM
#pragma data ( data )
// =============================================================================
volatile char regenAmountMin;
volatile char bonusAmountMin;

void sleepTick(){
    for(byte charSlot = 0; charSlot < CHARACTER_SLOTS; charSlot++){
        if(characterSlots[charSlot] != NO_CHARACTER){
            char charIdx = characterSlots[charSlot];
            word rnd = rand();
            byte rnd0 = 0;
            // hungry ppl do not regen much
            if(allCharacters[charIdx].regenTime){
                rnd0 = (rnd & 31) + 30;
            } else {
                rnd0 = (rnd & 15) + 5;
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
                decEnergyLevel(characterSlots[charSlot], ENERGY_COST_MIA);
            }
        }
    }
}

static void _regenChar(char charIdx){
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

void regenTick(){
    for(byte charSlot = 0; charSlot < CHARACTER_SLOTS; charSlot++){
        if(characterSlots[charSlot] != NO_CHARACTER){
            _regenChar(characterSlots[charSlot]);
        }
    }
}

// used on map, will alter timers nad regen all chars apart from player
void regenTickMapHour(){
    for(byte charSlot = 1; charSlot < CHARACTER_SLOTS; charSlot++){
        if(characterSlots[charSlot] != NO_CHARACTER){
            _regenChar(characterSlots[charSlot]);
        }
    }
}

void regenTickMinute(){
    // only player is ticked
    char charIdx = 0;
    char regen = 0;
    if(allCharacters[charIdx].regenTime){
        regen += regenAmountMin;
    }
    if(allCharacters[charIdx].bonusTime){
        regen += bonusAmountMin;
    }
    incEnergyLevel(charIdx, regen);
    drawBattery(0);
    // vic.color_border++;
}
//-----------------------------------------------------------------------------------------
// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
