#include "player.hpp"

#include "../core.hpp"

namespace game {
namespace player {

Player::Player(std::string name, PClass* pClass) : Entity(), name(name) {
    this->inventory = new Inventory();
    auto pairs = pClass->getItems();
    for (const auto& pair : pairs) {
        this->inventory->add(pair.first, pair.second);
    }
    // TODO set attributes
    equipment.insert(std::make_pair(PlayerSlot::P_ARM1, nullptr));
    equipment.insert(std::make_pair(PlayerSlot::P_ARM2, nullptr));
    equipment.insert(std::make_pair(PlayerSlot::P_FINGER1, nullptr));
    equipment.insert(std::make_pair(PlayerSlot::P_FINGER2, nullptr));
    equipment.insert(std::make_pair(PlayerSlot::P_HEAD, nullptr));
    equipment.insert(std::make_pair(PlayerSlot::P_TORSO, nullptr));
    equipment.insert(std::make_pair(PlayerSlot::P_LEGS, nullptr));
}

items::EquipableItem* Player::getEquipped(PlayerSlot slot) {
    if (slot == PlayerSlot::P_ARMS) return getEquipped(PlayerSlot::P_ARM1);
    auto it = equipment.find(slot);
    if (it == equipment.end()) throw std::runtime_error("can't get equipment");
    return it->second;
}

void Player::equip(PlayerSlot slot, items::EquipableItem* item, Game* game) {
    if (slot == PlayerSlot::P_ARMS) {
        equipment[PlayerSlot::P_ARM1] = item;
        equipment[PlayerSlot::P_ARM2] = item;
    } else {
        auto ss = toString(slot);
        if (ss.rfind(ss, 0) == 0) {
            auto ei = getEquipped(PlayerSlot::P_ARM1);
            if (ei != nullptr && ei->getSlot() == EquipSlot::ARMS) {
                equipment[PlayerSlot::P_ARM1] = nullptr;
                equipment[PlayerSlot::P_ARM2] = nullptr;
            }
        }
        equipment[slot] = item;
        // if item is equipped to other limb, unequip it
        auto count = inventory->count(item->getName());
        for (auto it = equipment.begin(); it != equipment.end(); it++) {
            if (it->first == slot) continue;
            auto oi = it->second;
            if (oi == item) {
                --count;
                if (count > 0) continue;
                equipment[it->first] = nullptr;
            }
        }
    }
    game->updatePlayerLook();
}

Player::~Player() {
    delete inventory;
}

Inventory* Player::getInventory() {
    return this->inventory;
}


// first - slot, second - item name (if any)
std::vector<std::pair<PlayerSlot, items::EquipableItem*>> Player::getEquipLines(EquipSlot slot) {
    std::vector<std::pair<PlayerSlot, items::EquipableItem*>> result;
    auto slots = SLOT_CORRELATION_MAP.find(slot);
    if (slots == SLOT_CORRELATION_MAP.end()) throw std::runtime_error("no correlation for " + toString(slot));
    for (const auto& sl : slots->second) {
        auto item = getEquipped(sl);
        result.push_back(std::make_pair(sl, item));
    }
    return result;
}

std::vector<items::EquipableItem*> Player::getAllEquipment() {
    std::vector<items::EquipableItem*> result;
    for (auto it = equipment.begin(); it != equipment.end(); it++)
        if (it->second)
            result.push_back(it->second);
    return result;
}


}
}