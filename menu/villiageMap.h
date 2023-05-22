#ifndef VILLIAGE_MAP_MENU_H
#define VILLIAGE_MAP_MENU_H

extern const struct MenuOption VILLIAGE_MAP_MENU[];
// these will need to be defined in a more global space, as they will be used with all maps probably
extern char vMapX;
extern char vMapY;
extern char vMapLocation;

#pragma compile("villiageMap.c")
#endif
