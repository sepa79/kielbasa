#include <c64/types.h>
#include "character.h"

#include <assets/mainGfx.h>
#include <engine/irqSpriteController.h>

struct CharacterStruct allCharacters[CHARACTER_COUNT];
struct CharacterStruct * characterSlots[CHARACTER_SLOTS];

static void _setCharacterToSlot(CharacterStruct * charPtr, byte charSlot){
    characterSlots[charSlot] = charPtr;

    if(charPtr != NO_CHARACTER){
        charPtr->slot = charSlot;
        setCharacterSlotPic(charPtr);
        setCharacterSlotIcon(charPtr, SPR_TASK_MIA);
        drawBattery(charPtr);
    }
    // TODO: Add slot cleanup
}

void initCharacterList(){
    static const struct CharacterStruct ch0 = {TXT_IDX_CHAR_1_STORY, TXT_IDX_CHAR_1_NAME, 100, 100, 100, SPR_PORTRAIT01, {3,3,3}, {3,3,4,3}, {1,2,3,4}, NO_SLOT, false};
    static const struct CharacterStruct ch1 = {TXT_IDX_CHAR_2_STORY, TXT_IDX_CHAR_2_NAME, 100, 100, 100, SPR_PORTRAIT02, {2,3,2}, {2,6,1,1}, {3,3,3,3}, NO_SLOT, false};
    static const struct CharacterStruct ch2 = {TXT_IDX_CHAR_3_STORY, TXT_IDX_CHAR_3_NAME, 100, 100, 100, SPR_PORTRAIT03, {4,3,4}, {1,1,1,7}, {3,3,3,3}, NO_SLOT, false};
    static const struct CharacterStruct ch3 = {TXT_IDX_CHAR_4_STORY, TXT_IDX_CHAR_4_NAME, 100, 100, 100, SPR_PORTRAIT04, {3,3,4}, {6,2,1,1}, {3,3,3,3}, NO_SLOT, false};
    allCharacters[0] = ch0;
    allCharacters[1] = ch1;
    allCharacters[2] = ch2;
    allCharacters[3] = ch3;

    _setCharacterToSlot(&(allCharacters[0]), 0);
    _setCharacterToSlot(NO_CHARACTER, 1);
    _setCharacterToSlot(NO_CHARACTER, 2);
    _setCharacterToSlot(NO_CHARACTER, 3);
}

// checks if given charIdx (from allCharSlots) has at least 'amount' energy
bool checkEnergyLevel(struct CharacterStruct * charPtr, byte amount){
    if(charPtr->energy >= amount){
        return true;
    }
    return false;
}

// call only on populated slots
void incEnergyLevel(struct CharacterStruct * charPtr, byte amount){
    byte energy = charPtr->energy;
    energy += amount;

    if(energy > 100){
        energy = 100;
    }
    charPtr->energy = energy;
    drawBattery(charPtr);
}

// call only on populated slots
void decEnergyLevel(struct CharacterStruct * charPtr, byte amount){
    byte energy = charPtr->energy;
    energy -= amount;

    if(energy > 100){
        energy = 0;
    }

    charPtr->energy = energy;
    drawBattery(charPtr);
}
