#pragma once

#include "tile.hpp"

namespace game {
namespace map {

class Room {
private:
    unsigned int height;
    unsigned int width;
    string name;
    unsigned int visibleRange;
    Tile** tiles;
    unsigned int tileCount;
    scripting::Script* loadScript = nullptr;
    Tile*** layout;
    Tile* tileWithCh(const char ch);
public:
    Room(string name, unsigned int visibleRange);
    ~Room();
    void loadTilesetFrom(const string path);
    void loadLayoutFrom(const string path);
    void loadLoadScriptFrom(const string path);
    void print(const char* prefix);
};

}
}