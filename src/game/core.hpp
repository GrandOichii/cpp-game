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

class GameWrapper {
public:
    virtual void updateLog(string message) = 0;
    virtual void sleep(int amount) = 0;
    virtual ~GameWrapper() = default;
};

class Game {
private:
    GameWrapper *wrapper;
    scripting::ScriptOverseer* scriptOverseer;
    GameInfo* gameInfo;
    map::MapData* mapData;
    CircularBuffer<std::string> * logs;

    unsigned int windowWidth;
    unsigned int windowHeight;
    unsigned int centerX;
    unsigned int centerY;
public:
    void setWrapper(GameWrapper * wrapper);
    GameWrapper * getWrapper();
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
    void addToLog(std::string message);
    vector<std::pair<int, int>> getAdjacentInteractableTiles();
    void interactAt(int xdiff, int ydiff);
    void sleep(int amount);
};

}