#include "Actor.h"

int Actor_getInitiative(Actor* actor) {
  return actor->dexterity;
}

bool Actor_isAlive(const Actor* actor) {
  return actor->energy > 0;
}

void Actor_Attack(Actor* attacker, Actor* defender) {
  // Default attack implementation

  defender->energy -= attacker->strength;
}

void Actor_Defend(Actor* actor) {
  // Default defend implementation
}
