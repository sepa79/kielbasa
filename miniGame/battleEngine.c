#include "BattleEngine.h"

void sortActorsByInitiative(Actor** actors, int numElements) {
  for (int i = 0; i < numElements - 1; i++) {
    for (int j = 0; j < numElements - i - 1; j++) {
      if (Actor_getInitiative(actors[j]) < Actor_getInitiative(actors[j + 1])) {
        Actor* temp = actors[j];
        actors[j] = actors[j + 1];
        actors[j + 1] = temp;
      }
    }
  }
}

void BattleEngine_init(BattleEngine* battleEngine, Actor** teamA, Actor** teamB, char numActorsA, char numActorsB) {
  // Calculate the total number of actors in the battle.
  char numActors = numActorsA + numActorsB;

  // Copy the actors from the teamA and teamB arrays into the sortedActors array.
  for (int i = 0; i < numActorsA; i++) {
    battleEngine->sortedActors[i] = teamA[i];
  }

  for (int i = 0; i < numActorsB; i++) {
    battleEngine->sortedActors[numActorsA + i] = teamB[i];
  }

  // Sort the sortedActors array by initiative score using the bubble sort algorithm.
  sortActorsByInitiative(battleEngine->sortedActors, numActors);

  battleEngine->numActors = numActors;
}

void BattleEngine_startBattle(BattleEngine* battleEngine) {
  char currentActorIndex = 0;
  while (!BattleEngine_isBattleOver(battleEngine)) {
    Actor* currentActor = battleEngine->sortedActors[currentActorIndex];

    // Take the current actor's turn.

    currentActorIndex++;
    if (currentActorIndex >= battleEngine->numActors) {
      currentActorIndex = 0;
    }
  }
}

bool BattleEngine_isBattleOver(const BattleEngine* battleEngine) {
  // Check if teamA has all players dead.
  bool isTeamADead = true;
  for (int i = 0; i < battleEngine->numActorsA; i++) {
    if (!Actor_isAlive(battleEngine->teamA[i])) {
      isTeamADead = false;
      break;
    }
  }

  // Check if teamB has all players dead.
  bool isTeamBDead = true;
  for (int i = 0; i < battleEngine->numActorsB; i++) {
    if (!Actor_isAlive(battleEngine->teamB[i])) {
      isTeamBDead = false;
      break;
    }
  }

  // Return true if one of the teams has all players dead, false otherwise.
  return isTeamADead || isTeamBDead;
}

Actor* BattleEngine_getWinner(const BattleEngine* battleEngine) {
//   for (char i = 0; i < battleEngine->numActors; i++) {
//     if (Actor_isAlive(battleEngine->actors[i])) {
//       return battleEngine->actors[i];
//     }
//   }
  return nullptr;
}
