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
class Game;
namespace items {
 

class Item : public IDescribable {
private:
    bool stackable;
    bool ct;
    bool known = false;
protected:
    string name;
    string displayName;
    string description;
public:
    bool canTake();
    bool isStackable();
    string getName();
    string getDisplayName();
    string getDescription();
    virtual ~Item() = default;

    Item(json j, bool stackable, bool canTake);

    string getBigDescription(int amount);

    virtual string category() = 0;
    virtual string additionalDescriptionInfo() = 0;

    int getOperations();
    void setKnown(bool value);
    bool isKnown();
};

class BasicItem : public Item {
public:
    BasicItem(json j);
    string category() override;
    string additionalDescriptionInfo() override;
};

class AmmoItem : public Item {
private:
    int damage;
    DamageType damageType;
    AmmoType ammoType;
public:
    AmmoItem(json j);

    string category() override;

    string additionalDescriptionInfo() override;
};

class EquipableItem : public Item {
private:
    EquipSlot slot;
    std::map<Attribute, int> requirements;
public:
    EquipableItem(json j);

    EquipSlot getSlot();

    virtual string extendedDisplayName() = 0;

    string additionalDescriptionInfo() override;

    int getOperations() override;
};

class ArmorItem : public EquipableItem {
private:
    int armorRating;
public:
    ArmorItem(json j);

    string category() override;

    string additionalDescriptionInfo() override;

    string extendedDisplayName() override;
};

class Weapon : public EquipableItem {
protected:
    int minDamage;
    int maxDamage;
    int range;
public:
    Weapon(json j);

    string category() override;

    string additionalDescriptionInfo() override;

    string extendedDisplayName() override;
};

class MeleeWeapon : public Weapon {
private:
    DamageType dType;
public:
    MeleeWeapon(json j);
};

class RangedWeapon : public Weapon {
private:
    AmmoType aType;
public:
    RangedWeapon(json j);
};

class UsableItem : public Item {
private:
public:
    UsableItem(json j, bool stackable);
    virtual void use(Game* game) = 0;
    int getOperations() override;
};

class IncantationBookItem : public UsableItem {
private:
public:
    IncantationBookItem(json j);
    void use(Game* game);
    string category();
    string additionalDescriptionInfo();

};

}
}