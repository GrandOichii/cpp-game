#pragma once

#include <map>
#include <set>
#include <vector>
#include <math.h>

#include "player/player.hpp"
#include "map/map.hpp"
#include "items/manager.hpp"
#include "game_info.hpp"
#include "scripting/overseer.hpp"
#include "../util.hpp"

using std::string;
using std::vector;

namespace game {

class GameWrapper {
public:
    virtual void updateLog(string message) = 0;
    virtual void sleep(int amount) = 0;
    virtual string requestChoice(string text, string choices) = 0;
    virtual ~GameWrapper() = default;
};

class Game {
private:
    GameWrapper *wrapper;
    player::Player* player;
    scripting::ScriptOverseer* scriptOverseer;
    GameInfo* gameInfo;
    items::ItemManager* itemManager;
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
    void createPlayer(std::string name);
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
    player::Player* getPlayer() const;
    string requestChoice(string text, string choices);
    vector<string> getLastLogs(int count);
    items::ItemManager* getItemManager();
};

}