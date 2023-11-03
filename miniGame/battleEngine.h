#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H

#include <c64/charwin.h>
#include "actor.h"

#define MAX_ACTORS 6

enum BattleStatus {
  BATTLE_STATUS_WAITING_FOR_INPUT,
  BATTLE_STATUS_WAITING_FOR_SCREEN_UPDATE,
  BATTLE_STATUS_TEAM_A_WIN,
  BATTLE_STATUS_TEAM_B_WIN,
  BATTLE_STATUS_UNKNOWN,
};

struct BattleEngine {
    char numActors;
    Actor* sortedActors[MAX_ACTORS];
    Actor** teamA;
    Actor** teamB;
    char numActorsA;
    char numActorsB;
    char currentTurnIndex;
};

void BattleEngine_init(BattleEngine* battleEngine, Actor** teamA, Actor** teamB, char numActorsA, char numActorsB);
void BattleEngine_startBattle(BattleEngine* battleEngine);
bool BattleEngine_isBattleOver(BattleEngine* battleEngine);
BattleStatus BattleEngine_mainLoop(BattleEngine* battleEngine);

#pragma compile("battleEngine.c")

#endif
