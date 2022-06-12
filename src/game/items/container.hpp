#pragma once

#include <map>
#include <vector>

#include "item.hpp"
#include "manager.hpp"
#include "../scripting/overseer.hpp"
#include "../../util.hpp"
#include "../../nlohmann/json.hpp"

using nlohmann::json;

namespace game {
namespace items {

class Container {
private:
    int count;
    Item** items;
    int* amounts;
public:
    Container(json j, ItemManager* im);
    ~Container();
    std::vector<std::pair<Item*, int>> getPairs();
};

class ContainerManager : public ILoadable {
private:
    std::map<std::string, Container*> containerMap;
public:
    ContainerManager();
    ~ContainerManager();
    Container* get(std::string key);
    std::vector<std::string> keys();
    void load(const string parentDir, const string path, game::scripting::ScriptOverseer* so);
};

}
}
