#ifndef MENU_STRUCTS_H
#define MENU_STRUCTS_H

#include <c64/types.h>

// Menu regions use these - keep in sync with GFX banks.
#define DAY_GFX_BMP 0x9000
#define DAY_GFX_SCR 0x9000+0x0f00
#define DAY_GFX_COL 0x9000+0x0f00+0x01e0
#define NIGHT_GFX_BMP 0xa400
#define NIGHT_GFX_SCR 0xa400+0x0f00
#define NIGHT_GFX_COL 0xa400+0x0f00+0x01e0
#define MENU_SPRITE_SRC 0xb800
#define MENU_SPRITE_DST 0xef00

#define SCREEN_X_START 0
#define SCREEN_Y_START 12
#define BIG_SCREEN_Y_START 0
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 12
#define BIG_SCREEN_HEIGHT 24

#define UI_U 1
#define UI_R 2
#define UI_D 4
#define UI_L 8
#define UI_LR 16
#define UI_UD 32
#define UI_HIDE 64

/* Represents a single menu option */
struct MenuOption {
    /* Text to display */
    byte textIdx;
    /* Key that will call the option if pressed */
    byte key;
    // ui mode - should it be bound to joy direction (U/D/L/R), or part of a group to navigate through (UD/LR)
    byte uiMode;
    /* Jump Pointer to code that handles the option */
    void (*jmpPtr)(void);
    /* Bank to copy code from, 00 - don't copy */
    byte bank; 
    /*  These are charwin coordinates, not screen ones - charwin is set by menu drawing function in jmpPtr */
    /* X position of text on screen*/
    byte x;
    /* Y position of text on screen*/
    byte y;
};

/* #define END_MENU_CHOICES {0x00, 0x00, 0x00, nullptr, 0x00, 0x00, 0x00} */
#define END_MENU_CHOICES {0x00, 0x00, 0x00, nullptr, 0x00, 0x00, 0x00}
#define NO_ALTERNATE_MENU_BANK 0x00

// Loaders vector table used in each menu
struct Loaders {
    void (*loadMenuCode)(void);
    void (*loadMenuGfx)(bool isDay);
    void (*loadMenuSprites)();
    void (*showMenu)(void);
    __interrupt void (*showSprites)(void);
    void (*updateMenu)(void);
};

void loadMenu(byte bank);
void loadMenuGfx(bool isDay);
void loadMenuSprites();
void showMenu();
__interrupt void showSprites();
void updateMenu();

#pragma compile("menuSystem.c")

#endif
