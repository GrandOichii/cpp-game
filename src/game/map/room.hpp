#pragma once

#include "tile.hpp"

namespace game {
namespace map {

struct RoomPos {
    int x;
    int y;
    string name;
};

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
    int getVisibleRange() const;
    string getName() const;
    unsigned int getHeight() const;
    unsigned int getWidth() const;
    Tile*** getLayout() const;
    void loadTilesetFrom(const string path);
    void loadLayoutFrom(const string path);
    void loadLoadScriptFrom(const string path);
    void print(const char* prefix);
};

}
}