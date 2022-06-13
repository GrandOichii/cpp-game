#pragma once

#include <map>
#include <set>
#include <vector>
#include <math.h>

#include "scripting/overseer.hpp"
#include "map/map.hpp"
#include "items/manager.hpp"
#include "items/container.hpp"
#include "game_info.hpp"
#include "player/pclass.hpp"
#include "player/player.hpp"

#include "../util.hpp"

using std::string;
using std::vector;

namespace game {

class GameWrapper {
public:
    virtual void updateLog(string message) = 0;
    virtual void sleep(int amount) = 0;
    virtual string requestChoice(string text, string choices) = 0;
    virtual bool accessContainer(items::Container* container, std::string top) = 0;
    virtual ~GameWrapper() = default;
};

class Game {
private:
    GameWrapper *wrapper;
    player::Player* player;
    player::ClassManager* classManager;
    scripting::ScriptOverseer* scriptOverseer;
    items::ItemManager* itemManager;
    items::ContainerManager* containerManager;
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
    std::string createPlayer(std::string savesDir, std::string name, std::string className);
    void loadPlayer(std::string saveFile);
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
    bool accessContainer(items::Container* container, std::string top);    
    vector<string> getLastLogs(int count);
    items::ItemManager* getItemManager();
    items::ContainerManager* getContainerManager();
    player::ClassManager* getClassManager();
    scripting::ScriptOverseer* getScriptOverseer();
};

}