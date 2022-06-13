#include "player.hpp"

namespace game {
namespace player {

Player::Player(std::string name, PClass* pClass) : Entity(), name(name) {
    this->inventory = new Inventory();
    auto pairs = pClass->getItems();
    for (const auto& pair : pairs) {
        this->inventory->add(pair.first, pair.second);
    }
    // TODO
}

Player::~Player() {
    delete inventory;
}

Inventory* Player::getInventory() {
    return this->inventory;
}

}
}