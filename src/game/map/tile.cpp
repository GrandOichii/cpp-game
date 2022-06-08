#include "tile.hpp"

#include <string>

#include "../scripting/script.hpp"
#include "../scripting/overseer.hpp"
#include "../../nlohmann/json.hpp"
#include "../../util.hpp"

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

string Tile::getWarpCode() const {
    return warpCode;
}

Tile::Tile(nlohmann::json j, std::string path, scripting::ScriptOverseer* so) {
    this->name = j["name"];
    this->displayName = j["displayName"];
    this->passable = j["passable"];
    this->transparent = j["transparent"];
    if (j.contains("warpcode")) {
        this->warpCode = j["warpcode"];
    }
    if (j.contains("interactScript")) {
        std::string ispath = j["interactScript"];
        this->interactScript = new scripting::Script(fs::join(path, ispath).c_str(), so);
    }
    if (j.contains("stepScript")) {
        std::string sspath = j["stepScript"];
        this->stepScript = new scripting::Script(fs::join(path, sspath).c_str(), so);
    }
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

void Tile::execStepScript() {
    if (this->stepScript == nullptr) return;
    this->stepScript->exec();
}

void Tile::execInteractScript() {
    if (this->interactScript == nullptr) return;
    this->interactScript->exec();
}

bool Tile::hasInteractScript() {
    return this->interactScript != nullptr;
}

}
}