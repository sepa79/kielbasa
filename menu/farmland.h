#ifndef FARMLAND_MENU_H
#define FARMLAND_MENU_H
#include <menu/menuSystem.h>

extern const struct MenuOption FARMLAND_MENU[];

enum FieldSizes {
    FIELD_SMALL,
    FIELD_MEDIUM,
    FIELD_BIG,
    FIELD_HUGE,
};

#pragma compile("farmland.c")
#endif