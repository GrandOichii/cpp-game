#include "tile.hpp"

#include <string>

#include "../../nlohmann/json.hpp"

using std::string;

namespace game {
namespace map {

bool Tile::isPassable() const { 
    return passable; 
}

bool Tile::isTransparent() const { 
    return transparent; 
}

string Tile::getName() const { 
    return name; 
}

Tile::Tile(nlohmann::json j) {
    this->name = j["name"];
    this->displayName = j["displayName"];
    this->passable = j["passable"];
    this->transparent = j["transparent"];
}

Tile::~Tile() {
    delete interactScript;
    delete stepScript;
}

void Tile::setCh(const string ch) { 
    this->ch = ch; 
}

string Tile::getCh() const { 
    return this->ch; 
}

void Tile::print(const char* prefix) {
    using std::cout;
    using std::endl;
    cout << prefix << name << endl;
}

}
}