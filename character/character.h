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
static const byte allChars_storyTxtIdx[CHARACTER_COUNT] =
    {TXT_IDX_CHAR_1_STORY, TXT_IDX_CHAR_2_STORY, TXT_IDX_CHAR_3_STORY, TXT_IDX_CHAR_4_STORY};
static const byte allChars_nameIdx[CHARACTER_COUNT]     =
    {TXT_IDX_CHAR_1_NAME, TXT_IDX_CHAR_2_NAME, TXT_IDX_CHAR_3_NAME, TXT_IDX_CHAR_4_NAME};
static byte allChars_energy[CHARACTER_COUNT]            = {100,  80,  50,  30};
static const char* allChars_picture[CHARACTER_COUNT]     = { SPR_TASK_PORTRAIT01, SPR_TASK_PORTRAIT02, SPR_TASK_PORTRAIT03, SPR_TASK_PORTRAIT04};
static const byte allChars_statInt[CHARACTER_COUNT]     = {  3,   2,   4,   3};
static const byte allChars_statStr[CHARACTER_COUNT]     = {  3,   3,   2,   3};
static const byte allChars_statCun[CHARACTER_COUNT]     = {  3,   2,   4,   4};
static const byte allChars_skillAni[CHARACTER_COUNT]    = {  3,   2,   1,   6};
static const byte allChars_skillFrm[CHARACTER_COUNT]    = {  3,   6,   1,   2};
static const byte allChars_skillBth[CHARACTER_COUNT]    = {  4,   1,   1,   1};
static const byte allChars_skillTrd[CHARACTER_COUNT]    = {  3,   1,   7,   1};
static byte allChars_prioAni[CHARACTER_COUNT]           = {  3,   3,   3,   3};
static byte allChars_prioFrm[CHARACTER_COUNT]           = {  3,   3,   3,   3};
static byte allChars_prioBth[CHARACTER_COUNT]           = {  3,   3,   3,   3};
static byte allChars_prioTrd[CHARACTER_COUNT]           = {  3,   3,   3,   3};
static const byte allChars_salary[CHARACTER_COUNT]      = {  0,   1,   2,   2};
static bool allChars_busy[CHARACTER_COUNT]              = {false, false, false, false};

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