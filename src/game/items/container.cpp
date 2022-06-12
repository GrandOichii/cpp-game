#include "container.hpp"

#include "../core.hpp"


namespace game {
namespace items {

Container::Container(json j, ItemManager* im) {
    this->count = j.size();
    this->items = new Item*[count];
    this->amounts = new int[count];
    int i = 0;
    for (const auto& [key, value] : j.items()) {
        auto item = im->getItem(key);
        this->items[i] = item;
        this->amounts[i] = value;
        ++i;
    }
}

Container::~Container() {
    delete[] items;
    delete[] amounts;
}

std::vector<std::pair<Item*, int>> Container::getPairs() {
    std::vector<std::pair<Item*, int>> result;
    for (int i = 0; i < count; i++)
        result.push_back(std::make_pair(items[i], amounts[i]));
    return result;
}

ContainerManager::ContainerManager() {

}

ContainerManager::~ContainerManager() {
    for (auto it = containerMap.begin(); it != containerMap.end(); it++) {
        delete it->second;
    }
}

std::vector<std::string> ContainerManager::keys() {
    std::vector<std::string> result;
    for (auto it = containerMap.begin(); it != containerMap.end(); it++)
        result.push_back(it->first);
    return result;
}


void ContainerManager::load(const string parentDir, const string path, game::scripting::ScriptOverseer* so) {
    auto j = fs::readJS(fs::join(parentDir, path));
    auto itemManager = so->getGame()->getItemManager();
    for (const auto& [key, value] : j.items()) {
        auto container = new Container(value, itemManager);
        containerMap[key] = container;
    }
}

Container* ContainerManager::get(std::string key) {
    auto it = containerMap.find(key);
    if (it == containerMap.end()) throw std::runtime_error("no container with key " + key);
    return it->second;
}

}
}
