class BattleEngine {
public:
  BattleEngine(vector<Actor*> teamA, vector<Actor*> teamB) {
    this->teamA = teamA;
    this->teamB = teamB;

    // Initialize the allActors vector.
    allActors = {};
    allActors.insert(allActors.end(), teamA.begin(), teamA.end());
    allActors.insert(allActors.end(), teamB.begin(), teamB.end());

    // Sort the actors by initiative.
    sortActorsByInitiative();
  }

  void startBattle() {
    // Start the battle loop.
    while (!teamA.empty() && !teamB.empty()) {
      // Get the current actor.
      Actor* currentActor = allActors[0];

      // Process the current actor's turn.
      processTurn(currentActor);

      // Move the current actor to the end of the allActors vector.
      allActors.push_back(allActors.front());
      allActors.erase(allActors.begin());
    }

    // Display the winner of the battle.
    if (!teamA.empty()) {
      cout << "Team A wins!" << endl;
    } else {
      cout << "Team B wins!" << endl;
    }
  }

private:
  vector<Actor*> teamA;
  vector<Actor*> teamB;
  vector<Actor*> allActors;

  void sortActorsByInitiative() {
    std::sort(allActors.begin(), allActors.end(), [](Actor* actor1, Actor* actor2) {
      return actor1->getInitiative() < actor2->getInitiative();
    });
  }

  void processTurn(Actor* currentActor) {
    // Get the actor's action.
    Action action = currentActor->getAction();

    // Execute the action.
    switch (action) {
      case Action::Attack:
        attack(currentActor, currentActor->getSelectedOpponent());
        break;
      case Action::Defend:
        defend(currentActor);
        break;
      default:
        break;
    }

    // Process the actor's Traits.
    for (Trait* trait : currentActor->getTraits()) {
      trait->onTurn(currentActor);
    }
  }

  void attack(Actor* attacker, Actor* defender) {
    // Check if the defender is alive.
    if (!defender->isAlive()) {
      return;
    }

    // Calculate the damage.
    int damage = attacker->getStrength() + getWeaponDamage() + attacker->getStrengthBonus() - defender->getDefense() - defender->getDexterityBonus() - defender->getDefenceBonus();

    // Deal the damage.
    defender->energy -= damage;

    // Display the results of the attack.
    cout << attacker->getName() << " attacks " << defender->getName() << " for " << damage << " damage!" << endl;

    // Check if the defender is still alive.
    if (defender->isAlive()) {
      // Process the actor's Traits after the defend.
      for (Trait* trait : actor->getTraits()) {
        trait->onDefend(actor);
      }
    } else {
      // Remove actor from all vectors
      removeActorFromAllVectors(defender);
    }
  }

  void defend(Actor* actor) {
    actor->setDefenceBonus(actor->getDexterity() * 2 + actor->getDexterityBonus());

    // Display the results of the defend.
    cout << actor->getName() << " defends!" << endl;
  }
};
