#include "Player.h"

void Player_Attack(Actor* attacker) {
    // Player attack implementation
    Actor* defender = attacker->selectedTarget;
    defender->energy -= attacker->strength + 5;
}

void Player_Defend(Actor* actor) {
    // Player defend implementation
    actor->defenseBonus += 5;
}
