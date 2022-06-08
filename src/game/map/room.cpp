#include "room.hpp"

#include <string>

#include "map.hpp"
#include "../scripting/overseer.hpp"
#include "../scripting/script.hpp"
#include "../../util.hpp"

using std::string;

namespace game {
namespace map {

int Room::getVisibleRange() const {
    return this->visibleRange;
}

Tile*** Room::getLayout() const {
    return this->layout;
}

unsigned int Room::getHeight() const {
    return this->height;
}

unsigned int Room::getWidth() const {
    return this->width;
}

string Room::getName() const {
    return this->name;
}

int Room::getTileSetSize() {
    return this->tileCount;
}

Tile** Room::getTileSet() const {
    return this->tiles;
}

Room::Room(string name, unsigned int visibleRange) : name(name), visibleRange(visibleRange) {}

Room::~Room() {
    for (int i = 0; i < tileCount; i++) {
        delete tiles[i];
    }
    delete[] tiles;
    delete loadScript;
    for (int i = 0; i < height; i++) delete[] layout[i];
    delete[] layout;
}

Tile* Room::tileWithCh(const char ch) {
    for (int i = 0; i < this->tileCount; i++) 
        if (tiles[i]->getCh()[0] == ch) 
            return tiles[i];
    throw std::runtime_error(str::format("no tile with ch %c", ch));
}

void Room::loadTilesetFrom(const string path, scripting::ScriptOverseer* so) {
    auto j = fs::readJS(path);
    this->tileCount = j.size();
    this->tiles = new Tile*[this->tileCount];
    int i = 0;
    for (auto item : j.items()) {
        string ch = item.key();
        tiles[i] = new Tile(item.value(), fs::dirOf(path), so);
        tiles[i]->setCh(ch);
        i++;
    }
}

void Room::loadLayoutFrom(const string path, MapData * map) {
    // TODO
    auto lines = fs::readAllLines(path.c_str());
    this->height = lines.size();
    this->width = lines[0].size();
    this->layout = new Tile**[this->height];
    for (int i = 0; i < this->height; i++) {
        this->layout[i] = new Tile*[this->width];
        for (int ii = 0; ii < this->width; ii++) {
            auto tile = this->tileWithCh(lines[i][ii]);
            this->layout[i][ii] = tile;
            auto warpCode = tile->getWarpCode();
            if (warpCode == "") continue;
            map->addWarpCode(warpCode, i, ii, this->name);   
        }
    }
}

void Room::loadLoadScriptFrom(const string path, scripting::ScriptOverseer* so) {
    this->loadScript = new scripting::Script(path.c_str(), so);
}

void Room::print(const char* prefix) {
    using std::cout;
    using std::endl;
    cout << prefix << "= " + name + " =" << endl;
    cout << prefix << "Visible range: " << visibleRange << endl;
    cout << prefix << "Size: " << height << " x " << width << endl;
    for (int i = 0; i < tileCount; i++)
        tiles[i]->print(str::format("\t\t%d) ", i).c_str());
    for (int i = 0; i < height; i++) { 
        for (int ii = 0; ii < width; ii++)
            cout << (layout[i][ii]->isPassable() ? " " : "#") << " ";
        cout << endl;
    }
}

void Room::executeLoadScript() {
    if (this->loadScript == nullptr) return;
    this->loadScript->exec();
}


}
}