#pragma once

#include <map>
#include <string>

#include "../scripting/script.hpp"
#include "../scripting/overseer.hpp"
#include "../util.hpp"
#include "../../util.hpp"
#include "room.hpp"

using std::string;

namespace game {
namespace map {

class MapData : public ILoadable {
private:
    unsigned int playerY;
    unsigned int playerX;
    std::map<string, Room*> roomMap;
    Room* currentRoom;
public:
    Room* getCurrentRoom() const;
    bool movePlayer(const Double mpair);
    unsigned int getPlayerY() const;
    unsigned int getPlayerX() const;
    MapData();
    ~MapData();
    void load(const string parentDir, const string path, const scripting::ScriptOverseer* so);
    void print();
};

}
}