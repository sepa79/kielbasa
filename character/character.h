#ifndef CHARACTER_H
#define CHARACTER_H

#include <translation/common.h>
#include <assets/mainGfx.h>
#include <tick/calendar.h>
#include <engine/gameState.h>

#define STAT_COUNT 3
#define SKILL_COUNT 4
enum CHAR_STATS {
    STAT_STR,
    STAT_AGI,
    STAT_INT,
};

enum CHAR_SKILLS {
    SKILL_ANIMALS,
    SKILL_FARMING,
    SKILL_COOKING,
    SKILL_TRADE,
    // SKILL_MELEE,
    // SKILL_RANGED,
};

#define CHARACTER_COUNT 4
#define CHARACTER_SLOTS 4
#define NO_CHARACTER 0xff
#define NO_SLOT 0xff
// priority goes 0 (none) 1 (highest) to 5 (lowest)
#define MAX_PRIO 5

struct Character {
    /* Pointer to text to display */
    char storyTextIdx;
    char nameIdx;
    char surnameIdx;
    /* How much energy char has, starts with 100, max 100, min 0 */
    unsigned int energy;
    /* Amount and time of regen, gained after eating */
    unsigned int regenAmount;
    char regenAmountMin;
    char regenTime;
    unsigned int bonusAmount;
    char bonusAmountMin;
    char bonusTime;
    /* CHARACTER_PORTRAITS index (0 based) */
    char * picture;

    char stat[STAT_COUNT];
    char skill[SKILL_COUNT];
    char prio[SKILL_COUNT];

    /* Populate when character is assigned to slot, used by UI handling */
    char slot;
    bool busy;
};

// Player character is the 1st one.
extern struct Character allCharacters[CHARACTER_COUNT];
// Each entry is an index to ALL_CHARACTERS arrays.
extern char characterSlots[CHARACTER_SLOTS];

//-----------------------------------------------------------------------------------------
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------
void initCharacterList();

//-----------------------------------------------------------------------------------------
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------

bool checkEnergyLevel(char charIdx, unsigned int amount);
void incEnergyLevel(char charIdx, unsigned int amount);
void decEnergyLevel(char charIdx, unsigned int amount);

#pragma compile("character.c")
#endif