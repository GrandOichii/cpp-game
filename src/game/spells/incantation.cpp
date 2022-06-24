#include "incantation.hpp"

#include "../types.hpp"

namespace game {
namespace spells {


Incantation::Incantation(string name, string mask, string description, string type) : 
    name(name), 
    mask(mask), 
    description(description),
    type(type) {}

string Incantation::getName() const { return name; }
string Incantation::getMask() const { return mask; }
string Incantation::getDescription() const { return description; }
string Incantation::getType() const { return type; }

int Incantation::getOperations() {
    return CLOSE_OPERATION;
}

string Incantation::getBigDescription(int amount) {
    return "Incantation " + mask + "\n\n" + description;
}


}}

