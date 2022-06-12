#include "player.hpp"

namespace game {
namespace player {

Player::Player(std::string name) : Entity(), name(name) {
    this->inventory = new Inventory();
}

Player::~Player() {
    delete inventory;
}

Inventory* Player::getInventory() {
    return this->inventory;
}

}
}