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


#define ANIM_EXPLOSION_DELAY 20
#define ANIM_DELAY 2
#define ANIM_FRAMES 10
#define EXPLOSION_COUNT 3
#define EXPLOSION_X_OFFSET 14
#define EXPLOSION_Y_OFFSET 40

#define PIGSLE_CMD_ANIM_CROSSHAIR_LOADED_BANK 16
#define PIGSLE_CMD_ANIM_CROSSHAIR_EMPTY_BANK 17
#define PIGSLE_CMD_ANIM_EXPLOSION_BANK 18


// Structure for a explosion
struct Explosion {
    int         x, y;         // Center of circle
    char        frame;        // frame
    char        delay;        // delay
    char        sprIdx;       // sprite index
    char        sprIdxOffset; // which explosion ot use - or 'bank offset'
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

#pragma code ( pigsleCommandRAMCode )
void pigsleScreenInit();
void pigsleSpriteLoader();

#pragma code ( pigsleCommandCode )
void game_state(GameState state);
void game_loop();

#pragma compile("pigsleCmdMain.c")

#pragma code ( code )
#pragma data ( data )

#endif