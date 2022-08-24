#ifndef SHOP_OUTSIDE_H
#define SHOP_OUTSIDE_H
// Switching code generation to shared section
#pragma code ( mainCode )
#pragma data ( mainData )

extern const struct MenuOption SHOP_OUTSIDE_MENU[];

#pragma compile("shopOutside.c")
#endif