#include "pclass.hpp"

#include "../core.hpp"
#include "../../util.hpp"


namespace game {
namespace player {

PClass::PClass(json j, items::ItemManager* itemManager) {
    this->name = j["name"];
    this->description = j["description"];
    for (const auto& [key, value] : j["attributes"].items()) {
        auto it = ATTRIBUTE_MAP.find(key);
        if (it == ATTRIBUTE_MAP.end()) throw std::runtime_error("unknown attribute: " + key);
        attributes[it->second] = value;
    }
    for (const auto& [key, value] : j["items"].items()) {
        auto item = itemManager->getItem(key);
        items.push_back(std::make_pair(item, value));
    }
}

PClass::~PClass() {

}

std::string PClass::getName() {
    return name;
}

std::map<Attribute, int> PClass::getAttributes() {
    return attributes;
}

std::vector<std::pair<items::Item*, int>> PClass::getItems() {
    return items;
}

ClassManager::ClassManager() {

}

ClassManager::~ClassManager() {
    for (int i = 0; i < count; i++) delete classes[i];
    delete[] classes;
}

void ClassManager::load(const string parentDir, const string path, scripting::ScriptOverseer* so) {
    auto j = fs::readJS(fs::join(vector<string>{parentDir, path}));
    count = j.size();
    int i = 0;
    classes = new PClass*[count];
    auto im = so->getGame()->getItemManager();
    for (const auto& [key, value] : j.items()) {
        classes[i] = new PClass(value, im);
        ++i;
    }
}

PClass** ClassManager::getClasses() {
    return classes;
}

int ClassManager::getCount() {
    return count;
}

}
}