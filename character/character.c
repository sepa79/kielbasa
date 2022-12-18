#include <c64/types.h>
#include "character.h"

#include <assets/mainGfx.h>
#include <engine/irqSpriteController.h>

#define NO_SLOT 0xff
const byte allChars_storyTxtIdx[CHARACTER_COUNT]           = {TXT_IDX_CHAR_1_STORY, TXT_IDX_CHAR_2_STORY, TXT_IDX_CHAR_3_STORY, TXT_IDX_CHAR_4_STORY};
const byte allChars_nameIdx[CHARACTER_COUNT]               = {TXT_IDX_CHAR_1_NAME, TXT_IDX_CHAR_2_NAME, TXT_IDX_CHAR_3_NAME, TXT_IDX_CHAR_4_NAME};
byte allChars_energy[CHARACTER_COUNT]                      = {100,  100,  100,  100};
const char* allChars_picture[CHARACTER_COUNT]              = { SPR_PORTRAIT01, SPR_PORTRAIT02, SPR_PORTRAIT03, SPR_PORTRAIT04};
const byte allChars_stats[CHARACTER_COUNT][3]              = { {3,3,3}, {2,3,2}, {4,2,4}, {3,3,4}};
const byte allChars_skills[CHARACTER_COUNT][SKILL_COUNT]   = { {3,3,4,3}, {2,6,1,1}, {1,1,1,7}, {6,2,1,1}};
byte allChars_prios[CHARACTER_COUNT][SKILL_COUNT]          = { {1,2,3,4}, {3,3,3,3}, {3,3,3,3}, {3,3,3,3}};
const byte allChars_salary[CHARACTER_COUNT]                = { 0, 1, 2, 2};
bool allChars_busy[CHARACTER_COUNT]                        = {false, false, false, false};
byte allChars_slot[CHARACTER_COUNT]                        = {NO_SLOT, NO_SLOT, NO_SLOT, NO_SLOT};

byte characterSlots[CHARACTER_SLOTS];

static void _setCharacterToSlot(byte charIdx, byte charSlot){
    characterSlots[charSlot] = charIdx;

    if(charIdx != NO_CHARACTER){
        allChars_slot[charIdx] = charSlot;
        setCharacterSlotPic(charSlot, allChars_picture[charIdx]);
        setCharacterSlotIcon(charSlot, SPR_TASK_MIA);
        drawBattery(charSlot, allChars_energy[charIdx]);
    }
}

void initCharacterList(){
    _setCharacterToSlot(0, 0);
    _setCharacterToSlot(1, 1);
    _setCharacterToSlot(2, 2);
    _setCharacterToSlot(3, 3);
    // _setCharacterToSlot(NO_CHARACTER, 3);
}

// checks if given charIdx (from allCharSlots) has at least 'amount' energy
bool checkEnergyLevel(byte charIdx, byte amount){
    byte energy = allChars_energy[charIdx];
    if(energy >= amount){
        return true;
    }
    return false;
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
