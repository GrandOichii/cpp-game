#pragma once

#include <string>

#include "spell.hpp"
#include "../core.hpp"

using std::string;

namespace game {
namespace spells {

class Incantation {
private:
    string name;
    string mask;
    string type;
    string description;
public:
    Incantation(string name, string mask, string description, string type);
    string getName() const;
    string getMask() const;
    string getType() const;
    string getDescription() const;
    int getOperations();
    string getBigDescription(int amount);
    virtual void editSpell(Spell* spell, Game* game) = 0;
};


}
}