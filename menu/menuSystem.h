#ifndef MENU_STRUCTS_H
#define MENU_STRUCTS_H

#include <c64/types.h>
#include <engine/irqHandler.h>
#include <c64/keyboard.h>

// Menu regions use these - keep in sync with GFX banks.
#define DAY_GFX_BMP 0x9000
#define DAY_GFX_SCR 0x9000+0x0f00
#define DAY_GFX_COL 0x9000+0x0f00+0x01e0
#define NIGHT_GFX_BMP 0xa2c0
#define NIGHT_GFX_SCR 0xa2c0+0x0f00
#define NIGHT_GFX_COL 0xa2c0+0x0f00+0x01e0
#define MENU_SPRITE_SRC 0xb711
#define MENU_SPRITE_DST 0xef00

#define SCREEN_X_START 0
#define SCREEN_Y_START 12
#define BIG_SCREEN_Y_START 0
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 13
#define BIG_SCREEN_HEIGHT 25

enum UI_MAP {
    UI_U=1,
    UI_D=2,
    UI_L=4,
    UI_R=8,
    UI_F=16,
    UI_LF=32,
    UI_SELECT=64,
    UI_HIDE=128
};

/* Represents a single menu option */
struct MenuOption {
    /* Text to display */
    byte textIdx;
    /* Key that will call the option if pressed */
    byte key;
    // what screen type IRQs to use
    byte screenMode;
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
#define END_MENU_CHOICES {0x00, 0x00, 0x00, 0x00, nullptr, 0x00, 0x00, 0x00}
#define NO_ALTERNATE_MENU_BANK 0x00

// Loaders vector table used in each menu
struct Loaders {
    void (*loadMenuCode)(void);
    void (*loadMenuGfx)(void);
    void (*loadMenuSprites)();
    void (*showMenu)(void);
    __interrupt void (*showSprites)(void);
    void (*updateMenu)(void);
    void (*runMenuLoop)(void);
};

extern bool mnu_isGfxLoaded;
extern volatile char mnu_menuBank;

void loadMenu(byte bank);
void loadMenuGfx();
void loadFullKoalaToBMP2();
void loadMenuSprites();
void showMenu();
__interrupt void showSprites();
void updateMenu();
void updateMenuIfIn(char bank);
void runMenuLoop();

#pragma compile("menuSystem.c")

#endif
