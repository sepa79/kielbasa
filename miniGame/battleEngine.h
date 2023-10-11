#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H

#include <c64/charwin.h>
#include "actor.h"

#define MAX_ACTORS 6

struct BattleEngine {
  char numActors;
  Actor* sortedActors[MAX_ACTORS];
  Actor* teamA[MAX_ACTORS/2];
  Actor* teamB[MAX_ACTORS/2];
  char numActorsA;
  char numActorsB;
};

void BattleEngine_init(BattleEngine* battleEngine, Actor** teamA, Actor** teamB, char numActorsA, char numActorsB);
void BattleEngine_startBattle(BattleEngine* battleEngine);
bool BattleEngine_isBattleOver(const BattleEngine* battleEngine);
Actor* BattleEngine_getWinner(const BattleEngine* battleEngine);
void displaySortedActors(BattleEngine* battleEngine, CharWin charWin, int x, int y);

#pragma compile("battleEngine.c")

#endif
