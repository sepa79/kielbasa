#ifndef VILLIAGE_MAP_MENU_H
#define VILLIAGE_MAP_MENU_H

// extern const struct MenuOption VILLIAGE_MAP_MENU[];
// these will need to be defined in a more global space, as they will be used with all maps probably

enum WalkDir {
    WALK_UP,
    WALK_DOWN,
    WALK_LEFT,
    WALK_RIGHT,
    WALK_NONE
};

enum MoveCost {
    MCOST_IMPASSABLE,
    MCOST_MAINROAD,
    MCOST_ROAD,
    MCOST_DIRTROAD,
    MCOST_GRASS,
    MCOST_MUD,
};

extern const char MOVE_COST[];

enum Location {
    LOCATION_NOTHING,
    LOCATION_FARM_HOUSE,
    LOCATION_FARM_BARN,
    LOCATION_FARM_PIGPEN,
    LOCATION_FARM_EMPTY,
    LOCATION_FARM_GARAGE,
    LOCATION_FARM_BIG_FIELD,
    LOCATION_FARM_SMALL_FIELD_1,
    LOCATION_FARM_SMALL_FIELD_2,
    LOCATION_FARM_MEDIUM_FIELD,
    LOCATION_SHOP,
    LOCATION_CHURCH,
    LOCATION_HERMAN,
    LOCATION_SULEJUKOWA,
    LOCATION_HOUSE_1,
    LOCATION_HOUSE_2,
    LOCATION_MAIN_ROAD,
    LOCATION_VILLIAGE_ROAD,
    LOCATION_DIRT_ROAD,
    LOCATION_WELL,
    LOCATION_TOILET,
    LOCATION_FISHING,
    LOCATION_RESET = 0xff
};

// defined in GameState
struct VilliageMap {
    char x;
    char y;
    Location location;
    WalkDir direction;
};

void villiageMapInit(void);

#pragma compile("villiageMap.c")
#endif
