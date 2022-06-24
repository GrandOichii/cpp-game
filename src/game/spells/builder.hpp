#pragma once

#include "spell.hpp"
#include "target.hpp"
#include "type.hpp"
#include "intensity.hpp"
#include "../core.hpp"
#include "../../util.hpp"

namespace game {
namespace spells {
 
Spell* parse(Game* game, std::string line) {
    auto words = str::split(line, " ");
    int count = 3;
    if (words.size() != count) throw std::runtime_error("incorrect number of incantations for spell");
    Incantation** incantations = new Incantation*[count];

    incantations[0] = targetFromMask(words[0]);
    incantations[1] = typeFromMask(words[1]);
    incantations[2] = intensityFromMask(words[2]);

    Spell* result = new Spell();
    for (int i = 0; i < count; i++) {
        // if (i == 1) break;
        incantations[i]->editSpell(result, game);
    }
    delete[] incantations;
    return result;
}
   
}
}
