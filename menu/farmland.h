#ifndef FARMLAND_MENU_H
#define FARMLAND_MENU_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

extern const struct MenuOption FARMLAND_MENU[];

#pragma compile("farmland.c")
#endif