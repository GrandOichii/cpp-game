#pragma once

#include <map>
#include <string>

#include "../scripting/overseer.hpp"
#include "../../util.hpp"
#include "../util.hpp"
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
    std::map<string, RoomPos> warpMap;
public:
    Room* getCurrentRoom() const;
    std::map<string, Room*> getRoomMap() const;
    bool movePlayer(const Double mpair);
    unsigned int getPlayerY() const;
    unsigned int getPlayerX() const;
    MapData();
    ~MapData();
    void load(const string parentDir, const string path, scripting::ScriptOverseer* so);
    void print();
    void addWarpCode(string warpCode, int y, int x, string name);   
    void useWarpCode(string code);
};

}
}