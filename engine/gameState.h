#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <tick/calendar.h>
#include <tick/characterTick.h>
#include <tick/farmlandTick.h>
#include <tick/kitchenTick.h>
#include <tasks/taskManager.h>
#include <menu/villiageMap.h>
#include <character/character.h>

#define MAX_ENERGY 1000
#define ENERGY_COST_MIA 90
#define ENERGY_COST_BAKE_BREAD 100
#define ENERGY_COST_MULTIPLIER_FIELD_TASK 10

struct GameState {
    unsigned long cash;
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
