#include "Player.h"

void Player_Attack(Actor* attacker, Actor* defender) {
  // Player attack implementation

  defender->energy -= attacker->strength + 5;
}

void Player_Defend(Actor* actor) {
  // Player defend implementation

  actor->defenseBonus += 5;
}
