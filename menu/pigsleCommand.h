#ifndef PIGSLE_COMMAND_MENU_H
#define PIGSLE_COMMAND_MENU_H

#include <engine/easyFlashBanks.h>

#define PIGSLE_CMD_ANIM_CROSSHAIR_LOADED_BANK 16
#define PIGSLE_CMD_ANIM_CROSSHAIR_EMPTY_BANK 17
#define PIGSLE_CMD_ANIM_EXPLOSION_BANK 18
#define PIGSLE_CMD_ANIM_PEST_DROP_BANK 38
#define PIGSLE_CMD_ANIM_B29_BANK PIGSLE_CMD_ANIM_PEST_DROP_BANK + DROP_ANIM_FRAMES*2

#define EXPLOSION_INITIAL_DELAY 40
#define EXPLOSION_ANIM_DELAY 2
#define EXPLOSION_ANIM_FRAMES 10
#define EXPLOSION_COUNT 3

#define DROP_INITIAL_DELAY 8

#define DROP_ANIM_1_DELAY 5
#define DROP_ANIM_1_ANIMA 1
#define DROP_ANIM_1_ANIMB 7

#define DROP_ANIM_2_DELAY 3
#define DROP_ANIM_2_ANIMA 1
#define DROP_ANIM_2_ANIMB 8

#define DROP_ANIM_3_DELAY 4
#define DROP_ANIM_3_ANIMA 2
#define DROP_ANIM_3_ANIMB 8

#define DROP_ANIM_FRAMES 20
#define DROP_ANIM_REPEAT 5
#define DROP_COUNT 4
#define DROP_MAX_Y 200
#define HIT_RANGE 7

// State of the bomb plane
struct DropRun {
    bool inProgress;
    int x;
    char dropsRemaining;
    char nextDropDelay;
};


enum GameState {
    GS_READY,     // Getting ready
    GS_PLAYING,   // Playing the game
    GS_END        // Wait for restart
};

enum PestDropType {
    PD_SIMPLE,
    PD_STRONG,
    PD_SINUS
};

// Structure for a pests drop
struct PestDrop {
    int         x;            // Sprite position
    char        y;            // Sprite position
    char        frame;        // animation frame
    char        delay;        // delay before advancing to next frame
    char        delayR;       // delay before advancing to next frame
    char        sprIdx;       // sprite index (0-7)
    char        sprBank;      // which animation to use, as starting bank
    char        type;         // one of a few drop types, determines animation style
    signed char animA;        // used with the above
    char        animB;        // used with the above
    signed char animAR;       // used with the above
    char        animBR;       // used with the above
    PestDrop  * next;         // Next drop in list
};

// Structure for an explosion
struct Explosion {
    int         x;            // Sprite position
    char        y;            // Sprite position
    char        frame;        // animation frame
    char        delay;        // delay before advancing to next frame
    char        sprIdx;       // sprite index (0-7)
    char        sprBank;      // which animation to use, as starting bank
    Explosion * next;         // Next explosion in list
};

//------------------------------------------------------------------------------------
// Regions
//------------------------------------------------------------------------------------
#pragma section( pigsleCommandLoaderData, 0 )
#pragma section( pigsleCommandCode, 0 )
#pragma section( pigsleCommandConsts, 0 )
#pragma section( pigsleCommandRAMCode, 0 )
#pragma section( pigsleCommandRAMData, 0 )
#pragma region( regionPigsleCommandCrt, 0x8000, 0xafff, , MENU_BANK_PIGSLE_COMMAND_1, { pigsleCommandLoaderData, pigsleCommandCode, pigsleCommandConsts } )
#pragma region( regionPigsleCommandRam, 0xb000, 0xbfff, , MENU_BANK_PIGSLE_COMMAND_1, { pigsleCommandRAMCode, pigsleCommandRAMData }, 0x7000 )

#pragma section( pigsleCommandGfx1, 0 )
#pragma section( pigsleCommandGfx1Loaders, 0 )
#pragma region( regionPigsleCommandG1, 0x8000, 0xbfff, , MENU_BANK_PIGSLE_COMMAND_GFX_1, { pigsleCommandGfx1, pigsleCommandGfx1Loaders } )
//------------------------------------------------------------------------------------

#pragma data ( pigsleCommandRAMData )
#pragma data ( pigsleCommandRAMData )

// Joystick and crosshair control
extern volatile int  CrossX;
extern volatile char CrossY;
extern volatile bool CrossP;
extern volatile char CrossDelay;

// used to quickly set sprites in IRQs
extern volatile char explosionAnimX[EXPLOSION_COUNT];
extern volatile char explosionAnimY[EXPLOSION_COUNT];
extern volatile char explosionAnimBank[EXPLOSION_COUNT];
extern volatile char explosionsVisible;
extern volatile char explosionsOver255;

// this is used to show that we can fire, as well as holds next free explosion pointer
extern Explosion * efree;

extern volatile char pestDropAnimX[DROP_COUNT];
extern volatile char pestDropAnimY[DROP_COUNT];
extern volatile char pestDropAnimBank[DROP_COUNT];
extern volatile char pestDropsVisible;
extern volatile char pestDropsOver255;

// used in IRQs
extern volatile DropRun TheB29Plane;

// Switching code generation back to shared section
#pragma code ( code )
#pragma data ( data )
extern const struct MenuOption PIGSLE_COMMAND_MENU[];

#pragma compile("pigsleCommand.c")
#endif
