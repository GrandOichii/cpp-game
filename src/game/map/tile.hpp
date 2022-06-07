#pragma once

#include <string>

#include "../scripting/script.hpp"
#include "../scripting/overseer.hpp"
#include "../../nlohmann/json.hpp"

using std::string;

namespace game {
namespace map {

class Tile {
private:
    string ch;
    string name;
    string displayName;
    string warpCode;
    bool passable;
    bool transparent;
    scripting::Script* interactScript = nullptr;
    scripting::Script* stepScript = nullptr;

public:
    bool isPassable() const;
    bool isTransparent() const;
    string getWarpCode() const;
    string getName() const;
    Tile(nlohmann::json j, std::string path, scripting::ScriptOverseer* so);
    ~Tile();
    void setCh(const string ch);
    string getCh() const;
    void print(const char* prefix);
    void execStepScript();
    void execInteractScript();
};

}
}

