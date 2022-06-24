#pragma once

#include <string>
#include <array>

#include "incantation.hpp"

using std::string;

namespace game {
namespace spells {

class TargetIncantation : public Incantation {
private:

public:
    TargetIncantation(string name, string mask, string description) : 
        Incantation(name, mask, description, "target") {}

};

class SelfTargetIncantation : public TargetIncantation {
private:

public:
    SelfTargetIncantation() : TargetIncantation(
        "self",
        "SELF",
        "Incantation for casting the spell on self."
    ) {}

    void editSpell(Spell* spell, Game* game) {
        auto target = game->getPlayer();
        spell->setAmount(0);
        spell->setCost(5);
        spell->setTarget(target);
    }
};

std::array<TargetIncantation*, 1> TARGETS = {
    new SelfTargetIncantation()
};
    
TargetIncantation* targetFromMask(string mask) {
    for (auto& in : TARGETS)
        if (in->getMask() == mask)
            return in;
    throw std::runtime_error("unknown target mask: " + mask);
}


}
}
