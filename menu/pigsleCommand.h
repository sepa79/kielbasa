#ifndef PIGSLE_COMMAND_MENU_H
#define PIGSLE_COMMAND_MENU_H

#define ANIM_EXPLOSION_DELAY 20
#define ANIM_DELAY 2
#define EXPLOSION_COUNT 3

// Structure for a explosion
struct Explosion{
    int         x, y;    // Center of circle
    char        f;       // frame
    char        d;       // delay
    char        s;       // sprite index
    Explosion * next;    // Next explosion in list
};


enum GameState
{
    GS_READY,     // Getting ready
    GS_PLAYING,   // Playing the game
    GS_END        // Wait for restart
};

// Joystick and crosshair control
extern volatile int  CrossX;
extern volatile char CrossY;
extern volatile bool CrossP
extern volatile char CrossDelay;

extern const struct MenuOption PIGSLE_COMMAND_MENU[];

#pragma compile("pigsleCommand.c")
#endif
