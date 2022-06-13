#pragma once

#include <string>

#include "pclass.hpp"
#include "inventory.hpp"
#include "../entities/entity.hpp"

namespace game {
namespace player {

class Inventory;

class Player : public entities::Entity {
private:
    std::string name;
    Inventory* inventory;
public:
    Player(std::string name, PClass* pClass);
    ~Player();
    Inventory* getInventory();
};

}
}
