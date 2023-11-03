#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"

struct Player : public Actor {
  int experiencePoints;
};

void Player_Attack(Actor* attacker);
void Player_Defend(Actor* actor);


#pragma compile("player.c")

#endif
