#pragma once

#include <string>

#include "../scripting/script.hpp"
#include "../../nlohmann/json.hpp"

using std::string;

namespace game {
namespace map {

class Tile {
private:
    string ch;
    string name;
    string displayName;
    bool passable;
    bool transparent;
    scripting::Script* interactScript = nullptr;
    scripting::Script* stepScript = nullptr;

public:
    bool isPassable() const;
    bool isTransparent() const;
    string getName() const;
    Tile(nlohmann::json j);
    ~Tile();
    void setCh(const string ch);
    string getCh() const;
    void print(const char* prefix);
};

}
}

