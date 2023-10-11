#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"

typedef struct Player : public Actor {
  int experiencePoints;
} Player;

void Player_Attack(Actor* attacker, Actor* defender);
void Player_Defend(Actor* actor);


#pragma compile("player.c")

#endif
