#include <c64/types.h>
#include "character.h"

#include <assets/mainGfx.h>
#include <engine/irqSpriteController.h>

byte characterSlots[CHARACTER_SLOTS];

static void _setCharacterToSlot(byte charIdx, byte charSlot){
    characterSlots[charSlot] = charIdx;

    if(charIdx != NO_CHARACTER){
        setCharacterSlotPic(charSlot, allChars_picture[charIdx]);
        setCharacterSlotIcon(charSlot, SPR_TASK_MIA);
        drawBattery(charSlot, allChars_energy[charIdx]);
    }
}

void initCharacterList(){
    _setCharacterToSlot(0, 0);
    _setCharacterToSlot(NO_CHARACTER, 1);
    _setCharacterToSlot(NO_CHARACTER, 2);
    _setCharacterToSlot(NO_CHARACTER, 3);
}

// call only on populated slots
void incEnergyLevel(byte charSlot, byte amount){
    byte charIdx = characterSlots[charSlot];
    byte energy = allChars_energy[charIdx];
    energy += amount;

    if(energy > 100){
        energy = 100;
    }
    allChars_energy[charIdx] = energy;
    drawBattery(charSlot, energy);
}

// call only on populated slots
void decEnergyLevel(byte charSlot, byte amount){
    byte charIdx = characterSlots[charSlot];
    byte energy = allChars_energy[charIdx];
    energy -= amount;
    
    if(energy > 100){
        energy = 0;
    }

    allChars_energy[charIdx] = energy;
    drawBattery(charSlot, energy);
}

void sleepTick(){
    for(byte charSlot = 0; charSlot < CHARACTER_SLOTS; charSlot++){
        if(characterSlots[charSlot] != NO_CHARACTER){
            word rnd = rand();
            byte rnd0 = rnd & 15;
            incEnergyLevel(charSlot, rnd0);
        }
    }
}
