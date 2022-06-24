#pragma once 

#include <string>
#include <array>

#include "incantation.hpp"
#include "spell.hpp"

using std::string;

namespace game {
namespace spells {
 

class TypeIncantation : public Incantation {
private:
    std::function<void(Spell*)> cast;
public:
    TypeIncantation(string name, string mask, string description, std::function<void(Spell*)> cast):
        Incantation(name, mask, description, "type"),
        cast(cast) {}

    void editSpell(Spell* spell, Game* game) {
        spell->setCast(this->cast);
    }
};

class HealTypeIncantation : public TypeIncantation {
private:

public:
    HealTypeIncantation() : TypeIncantation(
        "heal",
        "HEAL",
        "An incantation to cast a spell that heals dealt damage.",
        [](Spell* spell) {
            std::cout << "Hello" << std::endl;
            auto target = spell->getTarget();
            std::cout << target->getMaxHealth() << std::endl;
        }
    ) {}

};

std::array<TypeIncantation*, 1> TYPES = {
    new HealTypeIncantation()
};

   
TypeIncantation* typeFromMask(string mask) {
    for (auto& in : TYPES)
        if (in->getMask() == mask)
            return in;
    throw std::runtime_error("unknown type mask: " + mask);
}


}
}