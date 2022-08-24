#ifndef SMOKE_HOUSE_H
#define SMOKE_HOUSE_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

extern const struct MenuOption SMOKE_HOUSE_MENU[];

#pragma compile("smokeHouse.c")
#endif