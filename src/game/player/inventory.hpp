#pragma once

#include <iostream>
#include <vector>
#include <set>

#include "../items/item.hpp"
#include "../player/player.hpp"
// #inclide

namespace game {
namespace player {

struct ItemStruct {
    items::Item* item;
    int amount;
};

using ipVector = std::vector<ItemStruct*>;
using siMap = std::map<std::string, ipVector>;

class Inventory {
private:
    std::vector<std::pair<items::Item*, int>> items;
public:
    void add(items::Item* item);
    void add(items::Item* item, int amount);
    void take(items::Item* item);
    int count(std::string itemName);
    void addAll(std::vector<std::pair<items::Item*, int>> pairs);
    siMap getSorted();
};

}
}