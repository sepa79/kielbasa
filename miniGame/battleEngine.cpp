#include "battleEngine.h"
#include <opp/algorithm.h>

BattleEngine::BattleEngine(const opp::vector<Actor*> & teamA, const opp::vector<Actor*> & teamB) {
  this->teamA = teamA;
  this->teamB = teamB;

  // Initialize the allActors vector.
  allActors = opp::vector<Actor*>(teamA.size() + teamB.size());
  for (size_t i = 0; i < teamA.size(); i++) {
    allActors.push_back(teamA[i]);
  }
  for (size_t i = 0; i < teamB.size(); i++) {
    allActors.push_back(teamB[i]);
  }
  // Sort the actors by initiative.
  sortActorsByInitiative();
}

void BattleEngine::startBattle() {
  // Start the battle loop.
  while (!teamA.empty() && !teamB.empty()) {
    // Get the current actor.
    Actor* currentActor = allActors[0];

    // Process the current actor's turn.
    processTurn(currentActor);

    // Remove the current actor from the allActors vector.
    // allActors.erase(allActors.begin());
  }

  // Display the winner of the battle.
  if (!teamA.empty()) {
    // cout << "Team A wins!" << endl;
  } else {
    // cout << "Team B wins!" << endl;
  }
}

void BattleEngine::sortActorsByInitiative() {
  opp::sort(allActors.begin(), allActors.end(), [](Actor* actor1, Actor* actor2) {
    return actor1->getInitiative() < actor2->getInitiative();
  });
}

void BattleEngine::processTurn(Actor* currentActor) {
  // Get the actor's action.
  Action action = currentActor->getAction();

  // Execute the action.
  switch (action) {
    case ACTION_ATTACK:
      attack(currentActor, currentActor->getSelectedOpponent());
      break;
    case ACTION_DEFEND:
      defend(currentActor);
      break;
    default:
      break;
  }

}

void BattleEngine::attack(Actor* attacker, Actor* defender) {
  // Check if the defender is alive.
  if (!defender->isAlive()) {
    return;
  }

  // Calculate the damage.
  // todo: add weapon damage when the time comes
  int damage = attacker->getStrength() + attacker->getStrengthBonus() - defender->getDefense() - defender->getDexterityBonus() - defender->getDefenseBonus();

  // Deal the damage.
  defender->energy -= damage;

  // Display the results of the attack.
//   cout << attacker->getName() << " attacks " << defender->getName() << " for " << damage << " damage!" << endl;
}

void BattleEngine::defend(Actor* actor) {
  actor->setDefenseBonus(actor->getDexterity() * 2 + actor->getDexterityBonus());
}
