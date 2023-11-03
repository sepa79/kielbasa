#include "BattleEngine.h"

static void sortActorsByInitiative(Actor** actors, int numElements) {
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
    // Initialize the currentTurnIndex field.
    battleEngine->currentTurnIndex = 0;

    // Copy the actors from the teamA and teamB arrays into the sortedActors array.
    for (int i = 0; i < numActorsA; i++) {
        battleEngine->sortedActors[i] = teamA[i];
        teamA[i]->inPlayerTeam = true;
    }

    for (int i = 0; i < numActorsB; i++) {
        battleEngine->sortedActors[numActorsA + i] = teamB[i];
        teamB[i]->inPlayerTeam = false;
    }

    // keep track of the teams, so we can later select targets and friends as needed
    battleEngine->teamA = teamA;
    battleEngine->teamB = teamB;
    battleEngine->numActorsA = numActorsA;
    battleEngine->numActorsB = numActorsB;
    battleEngine->numActors = numActorsA + numActorsB;

    // Sort the sortedActors array by initiative score using the bubble sort algorithm.
    sortActorsByInitiative(battleEngine->sortedActors, battleEngine->numActors);
}

static bool BattleEngine_isTeamDead(int numActors, Actor** team) {
    // Check if all players on the given team are dead.
    for (int i = 0; i < numActors; i++) {
        if (!Actor_isAlive(team[i])) {
        return true;
        }
    }

    // No players on the given team are dead.
    return false;
}

static bool BattleEngine_isBattleOver(BattleEngine* battleEngine) {
    // Check if either team has all players dead.
    return BattleEngine_isTeamDead(battleEngine->numActorsA, battleEngine->teamA) || BattleEngine_isTeamDead(battleEngine->numActorsB, battleEngine->teamB);
}

BattleStatus BattleEngine_mainLoop(BattleEngine* battleEngine) {
    // While the battle is not over, process the turn of the current actor.
    while (!BattleEngine_isBattleOver(battleEngine)) {
        // Get the current actor.
        Actor* currentActor = battleEngine->sortedActors[battleEngine->currentTurnIndex];

        // If the actor is player controlled and has no action selected, then return a status indicating that the function is waiting for input.
        if (currentActor->playerControlled && currentActor->selectedAction == ACTION_NONE) {
            return BATTLE_STATUS_WAITING_FOR_INPUT;
        } else
        // Call a routine in the actor that will return the action that the NPC should take - player will already have one set by now.
        if (!currentActor->playerControlled && currentActor->inPlayerTeam){
            Actor_selectAction(currentActor, battleEngine->teamA, battleEngine->teamB);
        } else {
            Actor_selectAction(currentActor, battleEngine->teamB, battleEngine->teamA);
        }

        // Perform the action.
        Actor_performAction(currentActor);

        // Increment the current turn index.
        battleEngine->currentTurnIndex++;
        if(battleEngine->currentTurnIndex == battleEngine->numActors){
            battleEngine->currentTurnIndex = 0;
        }

        return BATTLE_STATUS_WAITING_FOR_SCREEN_UPDATE;
    }

    // The battle is over.
    // Check which team won and return the appropriate status.
    if (BattleEngine_isTeamDead(battleEngine->numActorsA, battleEngine->teamA)) {
        return BATTLE_STATUS_TEAM_B_WIN;
    } else if (BattleEngine_isTeamDead(battleEngine->numActorsB, battleEngine->teamB)) {
        return BATTLE_STATUS_TEAM_A_WIN;
    } else {
        // Unknown battle state.
        return BATTLE_STATUS_UNKNOWN;
    }
}
