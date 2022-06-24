#pragma once

#include "../core.hpp"
#include "../entities/entity.hpp"

namespace game {
namespace spells {

class Spell {
private:
    int cost;
    int amount;
    std::function<void(Spell*)> _cast;
    entities::Entity* target = nullptr;
public:
    Spell() {

    }

    int getAmount() { return amount; }
    entities::Entity* getTarget() { return target; }

    void setAmount(int v) { amount = v; }
    void setCost(int v) { cost = v; }
    void factorAmount(int f) { amount *= f; }
    void factorCost(int f) { cost *= f; }

    void setTarget(entities::Entity* target) { this->target = target; }
    void setCast(std::function<void(Spell*)> cast) { _cast = cast; }

    void cast(Game* game) {
        // spend mana
        // std::cout << _cast << std::endl;
        // cast the spell
        this->_cast(this);
    }
};

}}