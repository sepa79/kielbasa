#ifndef MAIN_MENU_H
#define MAIN_MENU_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

extern const struct MenuOption MAIN_MENU[];

#pragma compile("mainMenu.c")
#endif
