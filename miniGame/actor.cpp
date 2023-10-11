#include "actor.h"

Actor::Actor(
    int strength,
    int dexterity,
    int energy,
    int regenerationPoints
) {
  this->strength = strength;
  this->dexterity = dexterity;
  this->action = ACTION_NONE;
  this->energy = energy;
  this->regenerationPoints = regenerationPoints;
  this->maxEnergy = 1000;
  this->actionCallback = 0;
  this->defenseBonus = 0;
  this->selectedOpponent = nullptr;
}