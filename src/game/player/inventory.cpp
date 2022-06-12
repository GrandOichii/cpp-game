#include "inventory.hpp"

#include <string>
#include <map>
#include <vector>

#include "../items/item.hpp"

namespace game {
namespace player {

void Inventory::add(items::Item* item) {
    this->add(item, 1);
}

void Inventory::add(items::Item* item, int amount) {
    for (auto& pair : items) {
        if (pair.first == item) {
            pair.second += amount;
            return;
        }
    }
    items.push_back(std::make_pair(item, amount));
}

void Inventory::take(items::Item* item) {
    if (item->category() != "Other") throw std::runtime_error("can't take non-basic item " + item->getName());
    for (auto& pair : items) {
        if (pair.first == item) {
            pair.second--;
            return;
            // for now, don't remove the pair from the inventory: redundant if the item will be added later on (don't save pairs with 0 amount)
        }
    }
    throw std::runtime_error("can't take item " + item->getName() + ": player doens't have it");
}

siMap Inventory::getSorted() {
    siMap result;
    result["All"] = ipVector(0);
    for (const auto& pair : items) {
        std::string category = pair.first->category();
        auto it = result.find(category);
        if (it == result.end()) {
            result[category] = ipVector(0);
            it = result.find(category);
        }
        auto item = pair.first;
        int amount = pair.second;
        if (item->isStackable()) {
            auto s = new ItemStruct{item, amount};
            it->second.push_back(s);
            result["All"].push_back(s);
        } else {
            for (int i = 0; i < amount; i++) {
                auto s = new ItemStruct{item, 1};
                it->second.push_back(s);
                result["All"].push_back(s);
            }
        }
    }
    return result;
}

int Inventory::count(std::string itemName) {
    for (const auto& pair : items)
        if (pair.first->getName() == itemName)
            return pair.second;
    return 0;
}

void Inventory::addAll(std::vector<std::pair<items::Item*, int>> pairs) {
    for (const auto& pair : pairs)
        add(pair.first, pair.second);
}

}
}