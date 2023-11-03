#include "actor.h"
#include <c64/vic.h>

int Actor_getInitiative(Actor* actor) {
    return actor->dexterity;
}

bool Actor_isAlive(const Actor* actor) {
    return actor->energy > 0;
}

void Actor_Attack(Actor* attacker) {
    // Default attack implementation
    Actor* defender = attacker->selectedTarget;
    defender->energy -= attacker->strength;
}

void Actor_Defend(Actor* actor) {
  // Default defend implementation
}

// default action selection logic for Actor
void Actor_selectAction(Actor* actor, Actor** friends, Actor** enemies) {
    // If the actor is player controlled, then return.
    if (actor->playerControlled) {
        return;
    }

    // Otherwise, NPC will simply attack the first enemy.
    actor->selectedAction = ACTION_ATTACK;
    actor->selectedTarget = enemies[0];
}

void Actor_performAction(Actor* currentActor) {
    switch (currentActor->selectedAction) {
        case ACTION_ATTACK:
            // Attack the target.
            currentActor->Attack(currentActor);
            break;
        case ACTION_DEFEND:
            // Defend.
            currentActor->Defend(currentActor);
            break;
        default:
            // Unknown action.
            //   printf("Unknown action: %s\n", action.name);
            break;
    }
    // reset action
    currentActor->selectedAction = ACTION_NONE;
}
