#include "Actor.h"

void Actor_Attack(Actor* attacker, Actor* defender) {
  // Default attack implementation

  defender->energy -= attacker->strength;
}

void Actor_Defend(Actor* actor) {
  // Default defend implementation
}
