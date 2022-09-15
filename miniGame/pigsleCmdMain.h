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


#define ANIM_EXPLOSION_DELAY 40
#define ANIM_DELAY 2
#define ANIM_FRAMES 10
#define EXPLOSION_COUNT 3
#define EXPLOSION_X_OFFSET 0
#define EXPLOSION_Y_OFFSET 0

#define PIGSLE_CMD_ANIM_CROSSHAIR_LOADED_BANK 16
#define PIGSLE_CMD_ANIM_CROSSHAIR_EMPTY_BANK 17
#define PIGSLE_CMD_ANIM_EXPLOSION_BANK 18
#define PIGSLE_CMD_ANIM_B29_BANK 38

// Structure for an explosion
struct Explosion {
    int         x, y;         // Center of circle
    char        frame;        // frame
    char        delay;        // delay
    char        sprIdx;       // sprite index
    char        sprBank;      // which explosion to use - or 'bank offset'
    Explosion * next;         // Next explosion in list
};

// Structure for a pests drop
struct PestDrop {
    int         x, y;         // Center of circle
    char        frame;        // frame
    char        delay;        // delay
    char        sprIdx;       // sprite index
    char        sprBank;      // which explosion to use - or 'bank offset'
    Explosion * next;         // Next explosion in list
};

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
// this is used to show that we can fire, as well as holds next free explosion pointer
extern Explosion * efree;
// Display bitmap
// extern Bitmap sbm;

// used to quickly set sprites in IRQs
extern volatile char  explosionAnimX[EXPLOSION_COUNT];
extern volatile char explosionAnimY[EXPLOSION_COUNT];
extern volatile char explosionAnimBank[EXPLOSION_COUNT];
extern volatile char visibleExplosions;
extern volatile char explosionsOver255;

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