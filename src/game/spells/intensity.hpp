#pragma once

#include <string>
#include <array>

#include "incantation.hpp"
#include "spell.hpp"

using std::string;

namespace game {
namespace spells {
    
class IntensityIncantation : public Incantation {
private:
    int factor;
public:
    IntensityIncantation(string name, string mask, string description, int factor) : 
        Incantation(name, mask, description, "intensity"),
        factor(factor) {}

    void editSpell(Spell* spell, Game* game) {
        spell->factorAmount(factor);
        spell->factorCost(factor);
    }
};

std::array<IntensityIncantation, 3> INTENSITIES = {
    IntensityIncantation("low", "LOW", "An incantation to cast a weak spell.", 1),
    IntensityIncantation("medium", "MEDIUM", "An incantation to cast a standard spell.", 2),
    IntensityIncantation("high", "HIGH", "An incantation to cast a strong spell.", 3)
};

IntensityIncantation* intensityFromMask(string mask) {
    for (auto& in : INTENSITIES)
        if (in.getMask() == mask)
            return &in;
    throw std::runtime_error("unknown intensity mask: " + mask);
}

}
}