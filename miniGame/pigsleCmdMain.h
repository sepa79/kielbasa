#ifndef PIGSLE_CMD_MAIN_H
#define PIGSLE_CMD_MAIN_H

// #include <gfx/mcbitmap.h>

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

// Joystick and crosshair control
extern volatile int  CrossX;
extern volatile char CrossY;
extern volatile bool CrossP;
extern volatile char CrossDelay;
// this is used to show that we can fire, as well as holds next free explosion pointer
extern Explosion * efree;
// Display bitmap
// extern Bitmap sbm;

void pigsleScreenInit();
void pigsleSpriteLoader();
void game_state(GameState state);
void game_loop();

#pragma compile("pigsleCmdMain.c")

#endif