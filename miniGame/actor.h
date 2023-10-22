#ifndef ACTOR_H
#define ACTOR_H

enum Action {
  ACTION_NONE,
  ACTION_ATTACK,
  ACTION_DEFEND,
  ACTION_RUN,
};

struct Actor {
  const char* name;
  char strength;
  char dexterity;
  char energy;
  char defenseBonus;
  char regenerationPoints;
  bool playerControlled;
  bool inPlayerTeam;
  Action selectedAction;
  Actor* selectedTarget;
  void (*Attack)(Actor* attacker);
  void (*Defend)(Actor* actor);
};

int  Actor_getInitiative(Actor* actor);
bool Actor_isAlive(const Actor* actor);
void Actor_Attack(Actor* attacker);
void Actor_Defend(Actor* actor);

void Actor_selectAction(Actor* actor, Actor** friends, Actor** enemies);

#pragma compile("actor.c")

#endif
