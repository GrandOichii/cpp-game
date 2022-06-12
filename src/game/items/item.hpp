#pragma once

#include <string>
#include <map>
#include <vector>

#include "../util.hpp"
#include "../types.hpp"
#include "../../nlohmann/json.hpp"

using std::string;
using nlohmann::json;

namespace game {
namespace items {
 

class Item : public IDescribable {
private:
    bool stackable;
    bool known = false;
protected:
    string name;
    string displayName;
    string description;
public:
    bool isStackable() { return stackable; }
    string getName() { return name; }
    string getDisplayName() { return displayName; }
    string getDescription() { return description; }
    virtual ~Item() = default;

    Item(json j, bool stackable) : stackable(stackable) {
        this->name = j["name"];
        this->displayName = j["displayName"];
        this->description = j["description"];
    }

    string getBigDescription(int amount) {
        string result = "";
        result = displayName;
        if (amount > 1) {
            result += " (x " + std::to_string(amount) + ")";
        }
        result += "\n\n" + additionalDescriptionInfo();
        result += description;
        return result;
    }

    virtual string category() = 0;
    virtual string additionalDescriptionInfo() = 0;

    virtual int getOperations() { return CLOSE_OPERATION; }
    void setKnown(bool value) { this->known = value; }
    bool isKnown() { return known; }
};

class BasicItem : public Item {
public:
    BasicItem(json j) : Item(j, false) {}
    string category() override { return "Other"; }
    string additionalDescriptionInfo() override { return ""; }
};

class AmmoItem : public Item {
private:
    int damage;
    DamageType damageType;
    AmmoType ammoType;
public:
    AmmoItem(json j) : Item(j, true) {
        this->damage = j["damage"];
        auto it1 = DAMAGE_TYPE_MAP.find(j["damageType"]);
        if (it1 == DAMAGE_TYPE_MAP.end()) throw std::runtime_error("unknown damage type: " + (string)j["damageType"]);
        this->damageType = it1->second;
        auto it2 = AMMO_TYPE_MAP.find(j["ammoType"]);
        if (it2 == AMMO_TYPE_MAP.end()) throw std::runtime_error("unknown damage type: " + (string)j["ammoType"]);
        this->ammoType = it2->second;
    }

    string category() override { return "Ammo"; }

    string additionalDescriptionInfo() override {
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
};

class EquipableItem : public Item {
private:
    EquipSlot slot;
    std::map<Attribute, int> requirements;
public:
    EquipableItem(json j) : Item(j, false) {
        auto it = EQUIP_SLOT_MAP.find(j["slot"]);
        if (it == EQUIP_SLOT_MAP.end()) throw std::runtime_error("unknown equip slot: " + (string)j["slot"]);
        slot = it->second;
        for (const auto& [key, value] : j["requirements"].items()) {
            auto it = ATTRIBUTE_MAP.find(key);
            if (it == ATTRIBUTE_MAP.end()) throw std::runtime_error("unknown attribute: " + key);
            requirements.insert(std::make_pair(it->second, value));
        }
    }
    virtual string extendedDisplayName() = 0;

    string additionalDescriptionInfo() override {
        string result = "Requirements:\n";
        for (auto it = requirements.begin(); it != requirements.end(); it++) {
            if (it->second == 0) continue;
            result += " " + attributeToString(it->first) + ": [" + std::to_string(it->second) + "]";
        }
        return result + "\n";
    }

    int getOperations() override {
        return EQUIP_OPERATION | CLOSE_OPERATION;
    }
};

class ArmorItem : public EquipableItem {
private:
    int armorRating;
public:
    ArmorItem(json j) : EquipableItem(j) {
        this->armorRating = j["armorRating"];
    }

    string category() override { return "Armor"; }

    string additionalDescriptionInfo() override { return "Armor rating: " + std::to_string(armorRating) + "\n\n"; }

    string extendedDisplayName() override {
        return displayName + " (" + std::to_string(armorRating) + ")";
    }
};

class Weapon : public EquipableItem {
protected:
    int minDamage;
    int maxDamage;
    int range;
public:
    Weapon(json j) : EquipableItem(j) {
        this->minDamage = j["minDamage"];
        this->maxDamage = j["maxDamage"];
        this->range = j["range"];
    }

    string category() override { return "Weapons"; }

    string additionalDescriptionInfo() override {
        return EquipableItem::additionalDescriptionInfo() + "Damage: " + std::to_string(minDamage) + " - " + std::to_string(maxDamage) + " Range: " + std::to_string(range) + "\n\n";
    }

    string extendedDisplayName() override {
        return displayName + " (" + std::to_string(minDamage) + " - " + std::to_string(maxDamage) + ", " + std::to_string(range) + ")";
    }
};

class MeleeWeapon : public Weapon {
private:
    DamageType dType;
public:
    MeleeWeapon(json j) : Weapon(j) {
        auto it = DAMAGE_TYPE_MAP.find(j["damageType"]);
        if (it == DAMAGE_TYPE_MAP.end()) throw std::runtime_error("unknown damage type: " + (string)j["damageType"]);
        this->dType = it->second;
    }
};

class RangedWeapon : public Weapon {
private:
    AmmoType aType;
public:
    RangedWeapon(json j) : Weapon(j) {
        auto it = AMMO_TYPE_MAP.find(j["ammoType"]);
        if (it == AMMO_TYPE_MAP.end()) throw std::runtime_error("unknown ammo type: " + (string)j["ammoType"]);
    }
};

  
}
}