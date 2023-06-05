#ifndef CHARACTER_H
#define CHARACTER_H

#include <translation/common.h>
#include <assets/mainGfx.h>
#include <tick/calendar.h>

#define STAT_COUNT 3
#define SKILL_COUNT 4
enum CHAR_STATS {
    STAT_INT,
    STAT_STR,
    STAT_CUN
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

struct CharacterStruct {
    /* Pointer to text to display, max 5 x 28, should have newlines */
    char storyTextIdx;
    char nameIdx;
    /* How much energy char has, starts with 100, max 100, min 0 */
    char energy;
    /* How much food char has, starts with 100, max 100, min 0 */
    char food;
    /* How much health char has, starts with 100, max 100, min 0 */
    char health;

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
extern __striped struct CharacterStruct allCharacters[CHARACTER_COUNT];
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

bool checkEnergyLevel(char charIdx, char amount);
void incEnergyLevel(char charIdx, char amount);
void decEnergyLevel(char charIdx, char amount);

#pragma compile("character.c")
#endif