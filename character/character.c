#include "character.h"

#include <assets/mainGfx.h>
#include <engine/irqSpriteController.h>

__striped struct CharacterStruct allCharacters[CHARACTER_COUNT];
char characterSlots[CHARACTER_SLOTS];

static void _setCharacterToSlot(char charIdx, char charSlot){
    characterSlots[charSlot] = charIdx;

    if(charIdx != NO_CHARACTER){
        allCharacters[charIdx].slot = charSlot;
        setCharacterSlotPic(charIdx);
        setCharacterSlotIcon(charIdx, SPR_TASK_MIA);
        drawBattery(charIdx);
    }
    // TODO: Add slot cleanup
}

void initCharacterList(){
    static const struct CharacterStruct initialChars[CHARACTER_COUNT] = {
        {TXT_IDX_CHAR_1_STORY, TXT_IDX_CHAR_1_NAME, 100, 100, 100, SPR_PORTRAIT01, {3,3,3}, {3,3,4,3}, {1,2,3,4}, NO_SLOT, false},
        {TXT_IDX_CHAR_2_STORY, TXT_IDX_CHAR_2_NAME, 100, 100, 100, SPR_PORTRAIT02, {2,3,2}, {2,6,1,1}, {3,3,3,3}, NO_SLOT, false},
        {TXT_IDX_CHAR_3_STORY, TXT_IDX_CHAR_3_NAME, 100, 100, 100, SPR_PORTRAIT03, {4,3,4}, {1,1,1,7}, {3,3,3,3}, NO_SLOT, false},
        {TXT_IDX_CHAR_4_STORY, TXT_IDX_CHAR_4_NAME, 100, 100, 100, SPR_PORTRAIT04, {3,3,4}, {6,2,1,1}, {3,3,3,3}, NO_SLOT, false}
    };
    for(char i=0; i<CHARACTER_COUNT; i++){
        allCharacters[i] = initialChars[i];
    }

    _setCharacterToSlot(0, 0);
    _setCharacterToSlot(NO_CHARACTER, 1);
    _setCharacterToSlot(NO_CHARACTER, 2);
    _setCharacterToSlot(NO_CHARACTER, 3);
}

// checks if given charIdx (from allCharSlots) has at least 'amount' energy
bool checkEnergyLevel(char charIdx, char amount){
    if(allCharacters[charIdx].energy >= amount){
        return true;
    }
    return false;
}

// call only on populated slots
void incEnergyLevel(char charIdx, char amount){
    char energy = allCharacters[charIdx].energy;
    energy += amount;

    if(energy > 100){
        energy = 100;
    }
    allCharacters[charIdx].energy = energy;
    drawBattery(charIdx);
}

// call only on populated slots
void decEnergyLevel(char charIdx, char amount){
    char energy = allCharacters[charIdx].energy;
    energy -= amount;

    if(energy > 100){
        energy = 0;
    }

    allCharacters[charIdx].energy = energy;
    drawBattery(charIdx);
}
