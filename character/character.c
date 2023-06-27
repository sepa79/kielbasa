#include "character.h"

#include <assets/mainGfx.h>
#include <engine/irqSpriteController.h>

char characterSlots[CHARACTER_SLOTS];

static void _setCharacterToSlot(char charSlot, char charIdx){
    characterSlots[charSlot] = charIdx;

    if(charIdx != NO_CHARACTER){
        allCharacters[charIdx].slot = charSlot;
        setCharacterSlotPic(charIdx);
        setCharacterSlotIcon(charIdx, SPR_TASK_MIA);
        drawBattery(charIdx);
    }
    // TODO: Add slot cleanup
}

// checks if given charIdx (from allCharSlots) has at least 'amount' energy
bool checkEnergyLevel(char charIdx, unsigned int amount){
    if(allCharacters[charIdx].energy >= amount){
        return true;
    }
    return false;
}

// call only on populated slots
void incEnergyLevel(char charIdx, unsigned int amount){
    unsigned int energy = allCharacters[charIdx].energy;
    energy += amount;

    if(energy > MAX_ENERGY){
        energy = MAX_ENERGY;
    }
    allCharacters[charIdx].energy = energy;
    // drawBattery(charIdx);
}

// call only on populated slots
void decEnergyLevel(char charIdx, unsigned int amount){
    unsigned int energy = allCharacters[charIdx].energy;

    if(energy < amount){
        energy = 0;
    } else {
        energy -= amount;
    }

    allCharacters[charIdx].energy = energy;
    // drawBattery(charIdx);
}

//-----------------------------------------------------------------------------------------
// In Init bank
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------

void initCharacterList(){
    static const struct Character initialChars[CHARACTER_COUNT] = {
        {TXT_IDX_CHAR_1_STORY, TXT_IDX_CHAR_1_NAME, 800, 100, 60, 8, 0, 0, SPR_PORTRAIT01, {3,3,3}, {3,13,5,30}, {1,2,3,4}, NO_SLOT, false},
        {TXT_IDX_CHAR_2_STORY, TXT_IDX_CHAR_2_NAME, 100, 100, 0, 0, 0, 0, SPR_PORTRAIT02, {2,3,2}, {42,65,80,57}, {2,3,1,0}, NO_SLOT, false},
        {TXT_IDX_CHAR_3_STORY, TXT_IDX_CHAR_3_NAME, 100, 100, 0, 0, 0, 0, SPR_PORTRAIT03, {4,3,4}, {14,18,12,47}, {0,0,0,0}, NO_SLOT, false},
        {TXT_IDX_CHAR_4_STORY, TXT_IDX_CHAR_4_NAME, 500, 100, 0, 0, 0, 0, SPR_PORTRAIT04, {6,1,7}, {67,73,54,36}, {0,0,2,0}, NO_SLOT, false}
    };

    memcpy(allCharacters, initialChars, sizeof(initialChars));

    _setCharacterToSlot(0, 0);
    _setCharacterToSlot(1, NO_CHARACTER);
    _setCharacterToSlot(2, NO_CHARACTER);
    _setCharacterToSlot(3, 3);
}

//-----------------------------------------------------------------------------------------
// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------
