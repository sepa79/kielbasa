#ifndef ACTOR_H
#define ACTOR_H

struct Actor {
  const char* name;
  char strength;
  char dexterity;
  char energy;
  char defenseBonus;
  char regenerationPoints;
  void (*Attack)(Actor* attacker, Actor* defender);
  void (*Defend)(Actor* actor);
};

int getInitiative(Actor* actor);
void Actor_Attack(Actor* attacker, Actor* defender);
void Actor_Defend(Actor* actor);

#pragma compile("actor.c")

#endif
