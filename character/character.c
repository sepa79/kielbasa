#include <c64/types.h>
#include "character.h"

#include <assets/mainGfx.h>
#include <engine/irqSpriteController.h>

byte characterSlots[4];

// __export const struct CharacterStruct ALL_CHARACTERS[] = {
//     // story | name | energy (always 100) | pic | slot
// // | int | str | cun | ani | frm | but | trd |  $$$ | &trt | trtTxt
//     {TXT_IDX_CHAR_1_STORY, TXT_IDX_CHAR_1_NAME, 100, 0, 0,
//      3,     3,    3,    3,    3,    4,    3,    2},
//     {TXT_IDX_CHAR_2_STORY, TXT_IDX_CHAR_2_NAME, 80, 1, 0,
//      2,     3,    2,    2,    6,    1,    1,    1},
//     {TXT_IDX_CHAR_3_STORY, TXT_IDX_CHAR_3_NAME, 50, 2, 0,
//      4,     2,    4,    1,    1,    1,    7,    2},
//     {TXT_IDX_CHAR_4_STORY, TXT_IDX_CHAR_4_NAME, 30, 3, 0,
//      3,     3,    4,    6,    2,    1,    1,    2},
// };

static void _setCharacterToSlot(byte charIdx, byte slot){
    characterSlots[slot] = charIdx;
    // allChars_slot[charIdx] = slot;

    setCharacterSlotPic(slot, allChars_picture[charIdx]);
    setCharacterSlotIcon(slot, SPR_TASK_MIA);
    drawBattery(slot, allChars_energy[charIdx]);
}

void initCharacterList(){
    _setCharacterToSlot(0, 0);
    _setCharacterToSlot(1, 1);
    _setCharacterToSlot(2, 2);
    _setCharacterToSlot(3, 3);
}

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
    for(byte charSlot = 0; charSlot < 4; charSlot++){
        if(characterSlots[charSlot] != 0xff){
            word rnd = rand();
            byte rnd0 = rnd & 15;
            incEnergyLevel(charSlot, rnd0);
        }
    }
}
