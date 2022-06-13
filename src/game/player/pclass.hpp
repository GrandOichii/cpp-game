#pragma once

#include <map>
#include <vector>

#include "../util.hpp"
#include "../types.hpp"
#include "../items/item.hpp"
#include "../items/manager.hpp"
// #include "../core.hpp"

using nlohmann::json;

namespace game {
namespace player {

class PClass {
private:
    std::string name;
    std::string description;
    std::map<Attribute, int> attributes;
    std::vector<std::pair<items::Item*, int>> items;
public:
    PClass(json j, items::ItemManager* itemManager);
    ~PClass();
    std::string getName();
    std::map<Attribute, int> getAttributes();
    std::vector<std::pair<items::Item*, int>> getItems();
};

class ClassManager : public ILoadable {
private:
    int count;
    PClass** classes;
public:
    ClassManager();
    ~ClassManager();
    void load(const string parentDir, const string path, scripting::ScriptOverseer* so);
    PClass** getClasses();
    int getCount();
};

    
}
}
