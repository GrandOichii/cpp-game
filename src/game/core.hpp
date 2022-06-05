#pragma once

#include <map>
#include <vector>

#include "gameInfo.hpp"
#include "map/map.hpp"
#include "scripting/overseer.hpp"
#include "../util.hpp"

using std::string;
using std::vector;

namespace game {
    const string CLASSES_FILE = "classes.json";
    const string CONTAINERS_FILE = "containers.json";
    const string GAME_INFO_FILE = "game_info.json";
    const string ITEMS_FILE = "items.json";
    const string MAPDATA_FILE = "mapdata.json";

    class Game {
    private:
        scripting::ScriptOverseer* scriptOverseer;
        GameInfo* gameInfo;
        map::MapData* mapData;
    public:
        Game(const char* path) {
            this->scriptOverseer = new scripting::ScriptOverseer();
            this->gameInfo = new GameInfo();
            this->mapData = new map::MapData();
            auto p = string(path);

            // load info
            std::map<string, ILoadable*> loadMap = {
                {GAME_INFO_FILE, gameInfo},
                {MAPDATA_FILE, mapData}
            };
            for (auto it = loadMap.begin(); it != loadMap.end(); it++) {
                it->second->load(p, it->first.c_str(), this->scriptOverseer);
            }
        }

        ~Game() {
            delete scriptOverseer;
            delete gameInfo;
            delete mapData;
        }

        void print() {
            this->gameInfo->print();
            this->mapData->print();
        }
    };
}