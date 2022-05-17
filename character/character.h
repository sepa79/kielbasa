#ifndef CHARACTER_H
#define CHARACTER_H
#include <c64/types.h>

#include <translation/common.h>
#include <assets/mainGfx.h>

// struct CharacterStruct {
//     /* Pointer to text to display, max 5 x 28, should have newlines */
//     byte storyTextIdx;
//     byte nameIdx;
//     /* How much energy byte has, starts with 100, max 100, min 0 */
//     byte energy;

//     /* CHARACTER_PORTRAITS index (0 based) */
//     byte picture;

//     /* Which character slot this byte is using (if hired) */
//     byte slot;

//     // 1 - 5
//     byte statIntelligence;
//     byte statStrength;
//     byte statCunning;
    
//     // 1 - 10
//     byte skillAnimal;
//     byte skillFarm;
//     byte skillButcher;
//     byte skillTrade;

//     // x 10k, per day
//     byte salary;

//     /* Function to handle trait */
//     // unsigned int trait;
//     // byte * traitText;
// };
#define CHARACTER_COUNT 4
#define SKILL_COUNT 4
// priority goes 0 (none) 1 (highest) to 5 (lowest)
#define MAX_PRIO 5

static const byte allChars_storyTxtIdx[CHARACTER_COUNT]           = {TXT_IDX_CHAR_1_STORY, TXT_IDX_CHAR_2_STORY, TXT_IDX_CHAR_3_STORY, TXT_IDX_CHAR_4_STORY};
static const byte allChars_nameIdx[CHARACTER_COUNT]               = {TXT_IDX_CHAR_1_NAME, TXT_IDX_CHAR_2_NAME, TXT_IDX_CHAR_3_NAME, TXT_IDX_CHAR_4_NAME};
static byte allChars_energy[CHARACTER_COUNT]                      = {100,  80,  50,  30};
static const char* allChars_picture[CHARACTER_COUNT]              = { SPR_PORTRAIT01, SPR_PORTRAIT02, SPR_PORTRAIT03, SPR_PORTRAIT04};
static const byte allChars_stats[CHARACTER_COUNT][3]              = { {3,3,3}, {2,3,2}, {4,2,4}, {3,3,4}};
static const byte allChars_skills[CHARACTER_COUNT][SKILL_COUNT]   = { {3,3,4,3}, {2,6,1,1}, {1,1,1,7}, {6,2,1,1}};
static byte allChars_prios[CHARACTER_COUNT][SKILL_COUNT]          = { {1,2,3,4}, {3,3,3,3}, {3,3,3,3}, {3,3,3,3}};
static const byte allChars_salary[CHARACTER_COUNT]                = { 0, 1, 2, 2};
static bool allChars_busy[CHARACTER_COUNT]                        = {false, false, false, false};

#define STAT_INT 0
#define STAT_STR 1
#define STAT_CUN 2

#define SKILL_BREEDING 0
#define SKILL_FARMING 1
#define SKILL_BUTCHERY 2
#define SKILL_TRADE 3

// static const byte slot[CHARACTER_COUNT]

// Each entry is an index to ALL_CHARACTERS arrays.
// Player character is the 1st one.
#define CHARACTER_SLOTS 4
#define NO_CHARACTER 255
extern byte characterSlots[CHARACTER_SLOTS];

void initCharacterList();
void incEnergyLevel(byte charSlot, byte amount);
void decEnergyLevel(byte charSlot, byte amount);
void sleepTick();

#pragma compile("character.c")
#endif