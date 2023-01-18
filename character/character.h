#ifndef CHARACTER_H
#define CHARACTER_H
#include <c64/types.h>

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
#define NO_CHARACTER nullptr
#define NO_SLOT 0xff
// priority goes 0 (none) 1 (highest) to 5 (lowest)
#define MAX_PRIO 5

struct CharacterStruct {
    /* Pointer to text to display, max 5 x 28, should have newlines */
    byte storyTextIdx;
    byte nameIdx;
    /* How much energy char has, starts with 100, max 100, min 0 */
    byte energy;
    /* How much food char has, starts with 100, max 100, min 0 */
    byte food;
    /* How much health char has, starts with 100, max 100, min 0 */
    byte health;

    /* CHARACTER_PORTRAITS index (0 based) */
    const char * picture;

    byte stat[STAT_COUNT];
    byte skill[SKILL_COUNT];
    byte prio[SKILL_COUNT];

    /* Populate when character is assigned to slot, used by UI handling */
    byte slot;
    bool busy;
};

// Each entry is an index to ALL_CHARACTERS arrays.
// Player character is the 1st one.
extern CharacterStruct allCharacters[CHARACTER_COUNT];
extern CharacterStruct * characterSlots[CHARACTER_SLOTS];

//-----------------------------------------------------------------------------------------
#pragma code ( gameInitRAMCode )
#pragma data ( gameInitData )
//-----------------------------------------------------------------------------------------
void initCharacterList();

//-----------------------------------------------------------------------------------------
#pragma code ( code )
#pragma data ( data )
//-----------------------------------------------------------------------------------------

bool checkEnergyLevel(struct CharacterStruct * charPtr, byte amount);
void incEnergyLevel(struct CharacterStruct * charPtr, byte amount);
void decEnergyLevel(struct CharacterStruct * charPtr, byte amount);

#pragma compile("character.c")
#endif