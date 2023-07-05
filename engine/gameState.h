#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <tick/calendar.h>
#include <tick/characterTick.h>
#include <tick/farmlandTick.h>
#include <tick/kitchenTick.h>
#include <tasks/taskManager.h>
#include <menu/villiageMap.h>
#include <character/character.h>
#include <character/items.h>

#define MAX_ENERGY 1000
#define ENERGY_COST_MIA 90
#define ENERGY_COST_BAKE_BREAD 100
#define ENERGY_COST_MULTIPLIER_FIELD_TASK 10

// timer used on map for time ticks
#define MAP_TICK_DELAY 60;

struct GameState {
    unsigned long cash;
    unsigned long pension;
    // percentage shown as byte
    signed int inflation;
    // percentage shown as byte
    unsigned long bills;
    char billsPercent;
    struct Calendar calendar;
    struct Kitchen kitchen;
    struct Farm farm;
    struct VilliageMap vMap;

    // character data kept in allCharacters[], see gameState.c
    // tasks won't fit here, save separately
};

extern struct GameState GS;

void initGame();
void loadGame();
void saveGame();

#pragma compile("gameState.c")

#endif
