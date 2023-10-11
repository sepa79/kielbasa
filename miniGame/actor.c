#include "Actor.h"

int getInitiative(Actor* actor) {
  return actor->dexterity;
}

void Actor_Attack(Actor* attacker, Actor* defender) {
  // Default attack implementation

  defender->energy -= attacker->strength;
}

void Actor_Defend(Actor* actor) {
  // Default defend implementation
}
