#include "item.hpp"

#include "../core.hpp"

namespace game {
namespace items {


bool Item::isStackable() { return stackable; }
string Item::getName() { return name; }
string Item::getDisplayName() { return displayName; }
string Item::getDescription() { return description; }

Item::Item(json j, bool stackable, bool canTake) : stackable(stackable), ct(canTake) {
    this->name = j["name"];
    this->displayName = j["displayName"];
    this->description = j["description"];
}

bool Item::canTake() {
    return ct;
}

string Item::getBigDescription(int amount) {
    string result = "";
    result = displayName;
    if (amount > 1) {
        result += " (x " + std::to_string(amount) + ")";
    }
    result += "\n\n" + additionalDescriptionInfo();
    result += description;
    return result;
}

int Item::getOperations() { return CLOSE_OPERATION; }
void Item::setKnown(bool value) { this->known = value; }
bool Item::isKnown() { return known; }

BasicItem::BasicItem(json j) : Item(j, false, true) {}
string BasicItem::category() { return "Other"; }
string BasicItem::additionalDescriptionInfo() { return ""; }

AmmoItem::AmmoItem(json j) : Item(j, true, true) {
    this->damage = j["damage"];
    auto it1 = DAMAGE_TYPE_MAP.find(j["damageType"]);
    if (it1 == DAMAGE_TYPE_MAP.end()) throw std::runtime_error("unknown damage type: " + (string)j["damageType"]);
    this->damageType = it1->second;
    auto it2 = AMMO_TYPE_MAP.find(j["ammoType"]);
    if (it2 == AMMO_TYPE_MAP.end()) throw std::runtime_error("unknown damage type: " + (string)j["ammoType"]);
    this->ammoType = it2->second;
}

string AmmoItem::category() { return "Ammo"; }

string AmmoItem::additionalDescriptionInfo() {
    string result = "Ammo type: ";
    for (auto it = AMMO_TYPE_MAP.begin(); it != AMMO_TYPE_MAP.end(); it++){
        if (it->second == ammoType) {
            result += it->first;
            break;
        }
    }
    result += "\nDamage: " + std::to_string(damage) + "  Type: ";
    for (auto it = DAMAGE_TYPE_MAP.begin(); it != DAMAGE_TYPE_MAP.end(); it++) {
        if (it->second == damageType) {
            result += it->first;
            break;
        }
    }
    result += "\n\n";
    return result;
}

EquipableItem::EquipableItem(json j) : Item(j, false, false) {
    auto it = EQUIP_SLOT_MAP.find(j["slot"]);
    if (it == EQUIP_SLOT_MAP.end()) throw std::runtime_error("unknown equip slot: " + (string)j["slot"]);
    slot = it->second;
    for (const auto& [key, value] : j["requirements"].items()) {
        auto it = ATTRIBUTE_MAP.find(key);
        if (it == ATTRIBUTE_MAP.end()) throw std::runtime_error("unknown attribute: " + key);
        requirements.insert(std::make_pair(it->second, value));
    }
}

EquipSlot EquipableItem::getSlot() {
    return slot;
}


string EquipableItem::additionalDescriptionInfo() {
    string result = "Requirements:\n";
    for (auto it = requirements.begin(); it != requirements.end(); it++) {
        if (it->second == 0) continue;
        result += " " + attributeToString(it->first) + ": [" + std::to_string(it->second) + "]";
    }
    return result + "\n";
}

int EquipableItem::getOperations() {
    return EQUIP_OPERATION | CLOSE_OPERATION;
}

ArmorItem::ArmorItem(json j) : EquipableItem(j) {
    this->armorRating = j["armorRating"];
}

string ArmorItem::category() { return "Armor"; }

string ArmorItem::additionalDescriptionInfo() { return "Armor rating: " + std::to_string(armorRating) + "\n\n"; }

string ArmorItem::extendedDisplayName() {
    return displayName + " (" + std::to_string(armorRating) + ")";
}

Weapon::Weapon(json j) : EquipableItem(j) {
    this->minDamage = j["minDamage"];
    this->maxDamage = j["maxDamage"];
    this->range = j["range"];
}

string Weapon::category() { return "Weapons"; }

string Weapon::additionalDescriptionInfo() {
    return EquipableItem::additionalDescriptionInfo() + "Damage: " + std::to_string(minDamage) + " - " + std::to_string(maxDamage) + " Range: " + std::to_string(range) + "\n\n";
}

string Weapon::extendedDisplayName() {
    return displayName + " (" + std::to_string(minDamage) + " - " + std::to_string(maxDamage) + ", " + std::to_string(range) + ")";
}

MeleeWeapon::MeleeWeapon(json j) : Weapon(j) {
    auto it = DAMAGE_TYPE_MAP.find(j["damageType"]);
    if (it == DAMAGE_TYPE_MAP.end()) throw std::runtime_error("unknown damage type: " + (string)j["damageType"]);
    this->dType = it->second;
}

RangedWeapon::RangedWeapon(json j) : Weapon(j) {
    auto it = AMMO_TYPE_MAP.find(j["ammoType"]);
    if (it == AMMO_TYPE_MAP.end()) throw std::runtime_error("unknown ammo type: " + (string)j["ammoType"]);
}

UsableItem::UsableItem(json j, bool stackable) : Item(j, stackable, true) {

}

int UsableItem::getOperations() {
    return USE_OPERATION | CLOSE_OPERATION;
}

IncantationBookItem::IncantationBookItem(json j) : UsableItem(j, false) {

}

void IncantationBookItem::use(Game* game) {
    // TODO
    std::cout << "Using " << this->name << std::endl;
    // remove item from inventory
    game->getPlayer()->getInventory()->take(this);
}

string IncantationBookItem::category() {
    return "Books";
}

string IncantationBookItem::additionalDescriptionInfo() {
    return "";
}



}
}