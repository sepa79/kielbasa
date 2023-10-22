#include "Actor.h"

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
    Action action = ACTION_ATTACK;
    actor->selectedAction = action;
      actor->selectedTarget = enemies[0];
}
