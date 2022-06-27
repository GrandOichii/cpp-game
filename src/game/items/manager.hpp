#pragma once

#include <vector>
#include <map>

#include "../../util.hpp"
#include "../scripting/overseer.hpp"
#include "item.hpp"

namespace game {
namespace items {

template<class T>
static std::vector<Item*> loadItems(json j, std::string label) {
    if (!j.contains(label)) throw std::runtime_error("can't find items type " + label + " in items");
    auto items = j[label];
    std::vector<Item*> result;
    for (const auto& [key, value] : items.items()) {
        auto item = new T(value);
        result.push_back(item);
        // std::cout << item->getName() << std::endl;
    }
    return result;
}

class ItemManager : public ILoadable {
private:
    Item** allItems;
    int itemCount;
public:
    ItemManager() {

    }

    ~ItemManager() {
        for (int i = 0; i < itemCount; i++)
            delete allItems[i];
        delete[] allItems;
    }

    void load(const string parentDir, const string path, game::scripting::ScriptOverseer* so) {
        auto j = fs::readJS(fs::join(std::vector<string>{parentDir, path}));
        auto basic = loadItems<BasicItem>(j, "basic");
        auto ammo = loadItems<AmmoItem>(j, "ammo");
        auto armor = loadItems<ArmorItem>(j, "armor");
        auto mWeapons = loadItems<MeleeWeapon>(j, "meleeWeapons");
        auto rWeapons = loadItems<RangedWeapon>(j, "rangedWeapons");
        auto iBooks = loadItems<IncantationBookItem>(j, "incantationBooks");
        auto cItems = loadItems<CurrencyItem>(j, "currency");
        std::vector<std::vector<Item*>> v{basic, ammo, armor, mWeapons, rWeapons, iBooks, cItems};
        this->itemCount = 0;
        for (auto const& sv : v) this->itemCount += sv.size();
        this->allItems = new Item*[this->itemCount];
        int ii = 0;
        for (const auto& sv : v) {
            for (const auto& item : sv) {
                this->allItems[ii++] = item;
            }
        }
    }

    Item* getItem(std::string name) {
        for (int i = 0; i < itemCount; i++)
            if (allItems[i]->getName() == name)
                return allItems[i];
        throw std::runtime_error("no item with name " + name);
    }

    Item** getAllItems() {
        return this->allItems;
    }

    int getItemCount() {
        return this->itemCount;
    }
};  

}
}
