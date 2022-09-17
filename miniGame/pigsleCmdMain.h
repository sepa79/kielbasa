#ifndef PIGSLE_CMD_MAIN_H
#define PIGSLE_CMD_MAIN_H

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

#define PIGSLE_CMD_ANIM_CROSSHAIR_LOADED_BANK 16
#define PIGSLE_CMD_ANIM_CROSSHAIR_EMPTY_BANK 17
#define PIGSLE_CMD_ANIM_EXPLOSION_BANK 18
#define PIGSLE_CMD_ANIM_PEST_DROP_BANK 38
#define PIGSLE_CMD_ANIM_B29_BANK 48

enum GameState {
    GS_READY,     // Getting ready
    GS_PLAYING,   // Playing the game
    GS_END        // Wait for restart
};

#pragma data ( pigsleCommandRAMData )

// Joystick and crosshair control
extern volatile int  CrossX;
extern volatile char CrossY;
extern volatile bool CrossP;
extern volatile char CrossDelay;


// Structure for an explosion
struct Explosion {
    int         x, y;         // Sprite position
    char        frame;        // animation frame
    char        delay;        // delay before advancing to next frame
    char        sprIdx;       // sprite index (0-7)
    char        sprBank;      // which animation to use, as starting bank
    Explosion * next;         // Next explosion in list
};

// this is used to show that we can fire, as well as holds next free explosion pointer
extern Explosion * efree;
// Display bitmap
// extern Bitmap sbm;

#define EXPLOSION_INITIAL_DELAY 40
#define EXPLOSION_ANIM_DELAY 2
#define EXPLOSION_ANIM_FRAMES 10
#define EXPLOSION_COUNT 3

// used to quickly set sprites in IRQs
extern volatile char explosionAnimX[EXPLOSION_COUNT];
extern volatile char explosionAnimY[EXPLOSION_COUNT];
extern volatile char explosionAnimBank[EXPLOSION_COUNT];
extern volatile char explosionsVisible;
extern volatile char explosionsOver255;


// Structure for a pests drop
struct PestDrop {
    int         x, y;         // Sprite position
    char        frame;        // animation frame
    char        delay;        // delay before advancing to next frame
    char        sprIdx;       // sprite index (0-7)
    char        sprBank;      // which animation to use, as starting bank
    PestDrop  * next;         // Next drop in list
};

#define DROP_INITIAL_DELAY 3
#define DROP_ANIM_DELAY 3
#define DROP_ANIM_FRAMES 10
#define DROP_COUNT 4
#define DROP_MAX_Y 200

extern volatile char pestDropAnimX[DROP_COUNT];
extern volatile char pestDropAnimY[DROP_COUNT];
extern volatile char pestDropAnimBank[DROP_COUNT];
extern volatile char pestDropsVisible;
extern volatile char pestDropsOver255;

#pragma code ( pigsleCommandRAMCode )
void pigsleScreenInit();
void pigsleSpriteLoader();
void pigsleCmdInit();

#pragma code ( pigsleCommandCode )
void gameState(GameState state);
void gameLoop();

#pragma compile("pigsleCmdMain.c")

#pragma code ( code )
#pragma data ( data )

#endif