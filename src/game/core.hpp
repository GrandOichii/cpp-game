#pragma once

#include <map>
#include <vector>

#include "gameInfo.hpp"
#include "mapData.hpp"
#include "util.hpp"

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
        GameInfo* gameInfo;
        MapData* mapData;
    public:
        Game(const char* path) {
            this->gameInfo = new GameInfo();
            this->mapData = new MapData();
            auto p = string(path);

            // load info
            std::map<string, ILoadable*> loadMap = {
                {GAME_INFO_FILE, gameInfo},
                {MAPDATA_FILE, mapData}
            };
            for (auto it = loadMap.begin(); it != loadMap.end(); it++) {
                it->second->load(p, it->first.c_str());
            }
        }

        ~Game() {
            delete gameInfo;
            delete mapData;
        }

        void print() {
            this->gameInfo->print();
            this->mapData->print();
        }
    };

}