#ifndef SHOP_INSIDE_H
#define SHOP_INSIDE_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

extern const struct MenuOption SHOP_INSIDE_MENU[];

#pragma compile("shopInside.c")
#endif