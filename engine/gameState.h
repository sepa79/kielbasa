#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <tick/calendar.h>
#include <tick/characterTick.h>
#include <tick/farmlandTick.h>
#include <tick/kitchenTick.h>
#include <character/character.h>

struct GameState {
    struct Calendar calendar;
    struct Kitchen kitchen;
};

extern struct GameState GS;

void initGame();
void loadGame();
void saveGame();

#pragma compile("gameState.c")

#endif
