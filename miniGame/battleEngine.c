#include "BattleEngine.h"

static void sortActorsByInitiative(BattleEngine* battleEngine, Actor** actors, char numActors) {
//   // Check if the input parameters are valid.
//   if (!battleEngine || !actors || numActors <= 0) {
//     return;
//   }

  // Use a selection sort algorithm to sort the actors in the BattleEngine's sorted actors array.
  for (char i = 0; i < numActors - 1; i++) {
    // Find the smallest element in the unsorted subarray.
    char smallestIndex = i;
    for (char j = i + 1; j < numActors; j++) {
      if (getInitiative(actors[j]) < getInitiative(actors[smallestIndex])) {
        smallestIndex = j;
      }
    }

    // Swap the smallest element with the current element in the BattleEngine's sorted actors array.
    Actor* temp = battleEngine->sortedActors[i];
    battleEngine->sortedActors[i] = battleEngine->sortedActors[smallestIndex];
    battleEngine->sortedActors[smallestIndex] = temp;
  }
}

void BattleEngine_init(BattleEngine* battleEngine, Actor** actors, char numActors) {
  battleEngine->numActors = numActors;

  // Sort the actors by initiative.
  sortActorsByInitiative(battleEngine, actors, numActors);
  
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
//   for (char i = 0; i < battleEngine->numActors; i++) {
//     if (Actor_isAlive(battleEngine->actors[i])) {
//       return false;
//     }
//   }
  return true;
}

Actor* BattleEngine_getWinner(const BattleEngine* battleEngine) {
//   for (char i = 0; i < battleEngine->numActors; i++) {
//     if (Actor_isAlive(battleEngine->actors[i])) {
//       return battleEngine->actors[i];
//     }
//   }
  return nullptr;
}
