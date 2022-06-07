#pragma once

#include <map>
#include <vector>
#include <math.h>

#include "map/map.hpp"
#include "gameInfo.hpp"
#include "scripting/overseer.hpp"
#include "../util.hpp"

using std::string;
using std::vector;

namespace game {

// class MapData;

class Game {
private:
    scripting::ScriptOverseer* scriptOverseer;
    GameInfo* gameInfo;
    map::MapData* mapData;

    unsigned int windowWidth;
    unsigned int windowHeight;
    unsigned int centerX;
    unsigned int centerY;
public:
    Game(const char* path);
    ~Game();
    GameInfo *getInfo() const;
    map::MapData *getMap() const;
    void setWindowSize(int width, int height);
    void print();
    void start();
    void update();
    bool movePlayer(const Double mpair);
    vector<map::RoomPos> getVisibleTiles();
    void useWarpCode(std::string code);
};

}