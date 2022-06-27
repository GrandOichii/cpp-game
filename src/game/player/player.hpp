#pragma once

#include <string>
#include <vector>
#include <map>

#include "pclass.hpp"
#include "inventory.hpp"
#include "../types.hpp"
#include "../items/item.hpp"
#include "../entities/entity.hpp"

namespace game {
class Game;
namespace player {

class Inventory;

class Player : public entities::Entity {
private:
    int currency;
    std::string name;
    std::string className;
    Inventory* inventory;
    std::map<PlayerSlot, items::EquipableItem*> equipment;
public:
    Player(std::string name, PClass* pClass);
    ~Player();
    Inventory* getInventory();
    void equip(PlayerSlot slot, items::EquipableItem* item, Game* game);
    items::EquipableItem* getEquipped(PlayerSlot slot);
    std::vector<std::pair<PlayerSlot, items::EquipableItem*>> getEquipLines(EquipSlot slot);
    std::vector<items::EquipableItem*> getAllEquipment();

    std::string getName();
    std::string getClassName();

    int getMaxHealth();
    int getCurrentHealth();
    int getMaxMana();
    int getCurrentMana();

    int getCurrency();
    void addCurrency(int value);

};

}
}
