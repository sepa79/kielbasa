#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <tick/calendar.h>
#include <tick/characterTick.h>
#include <tick/farmlandTick.h>
#include <tick/kitchenTick.h>
#include <tasks/taskManager.h>
#include <menu/villiageMap.h>
#include <character/character.h>

struct GameState {
    long cash;
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
