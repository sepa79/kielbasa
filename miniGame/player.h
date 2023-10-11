#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"

struct Player : public Actor {
  int experiencePoints;
};

void Player_Attack(Actor* attacker, Actor* defender);
void Player_Defend(Actor* actor);


#pragma compile("player.c")

#endif
