#include <opp/vector.h>

enum Action {
  ACTION_ATTACK,
  ACTION_DEFEND,
  ACTION_NONE
};

class Actor {
private:
  int strength;
  int dexterity;
  int energy;
  int regenerationPoints;
  int maxEnergy;
  int defence;
  int regeneration;
  int actionCallback;
//   opp::function<Action()> actionCallback;
  int defenceBonus;
  Actor* selectedOpponent;
//   vector<Trait*> traits;

public:
  Actor(int strength, int dexterity, int energy, int regenerationPoints) {
    this->strength = strength;
    this->dexterity = dexterity;
    this->energy = energy;
    this->regenerationPoints = regenerationPoints;
    this->maxEnergy = 1000;
    this->actionCallback = nullptr;
    this->defenceBonus = 0;
    this->selectedOpponent = nullptr;
    // this->traits = {};
  }

  int getStrength() { return strength; }
  int getDexterity() { return dexterity; }
  int getEnergy() { return energy; }
  int getRegenerationPoints() { return regenerationPoints; }
  int getMaxEnergy() { return maxEnergy; }

  int getStrengthBonus() { return (strength - 10) / 2; }
  int getDexterityBonus() { return (dexterity - 10) / 2; }

  int getInitiative() { return dexterity + getDexterityBonus(); }

//   void setActionCallback(opp::function<Action()> actionCallback) {
//     this->actionCallback = actionCallback;
//   }

  void setDefenceBonus(int defenceBonus) {
    this->defenceBonus = defenceBonus;
  }

  int getDefenceBonus() {
    return defenceBonus;
  }

  bool isAlive() { return energy > 0; }

  void setSelectedOpponent(Actor* selectedOpponent) {
    this->selectedOpponent = selectedOpponent;
  }

  Actor* getSelectedOpponent() {
    return selectedOpponent;
  }

  Action getAction() {
    if (actionCallback != nullptr) {
      Action action = ACTION_ATTACK;//actionCallback();
      if (action == ACTION_DEFEND) {
        setDefenceBonus(getDexterity() * 2 + getDexterityBonus());
      }
      return action;
    } else {
      return ACTION_NONE;
    }
  }

//   void addTrait(Trait* trait) {
//     this->traits.push_back(trait);
//   }

//   vector<Trait*> getTraits() {
//     return traits;
//   }

};
