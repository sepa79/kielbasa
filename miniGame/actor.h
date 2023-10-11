#ifndef ACTOR_H
#define ACTOR_H

typedef struct Actor {
  const char* name;
  char strength;
  char dexterity;
  char energy;
  char defenseBonus;
  char regenerationPoints;
  void (*Attack)(Actor* attacker, Actor* defender);
  void (*Defend)(Actor* actor);
} Actor;

#pragma compile("actor.c")

#endif
