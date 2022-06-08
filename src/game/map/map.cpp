#include "map.hpp"

#include <map>
#include <string>

#include "../util.hpp"
#include "../../util.hpp"
#include "../../nlohmann/json.hpp"
#include "../scripting/script.hpp"
#include "../scripting/overseer.hpp"

using std::string;

namespace game {
namespace map {

bool MapData::movePlayer(const Double mpair) {
    auto a = mpair.a;
    auto height = this->currentRoom->getHeight();
    auto width = this->currentRoom->getWidth();
    int newX = this->playerX + a[0];
    int newY = this->playerY + a[1];
    if (newX < 0 || newY < 0 || newX >= width || newY >= height) return false;
    auto tile = this->currentRoom->getLayout()[newY][newX];
    if (!tile->isPassable()) {
        return false;
    }
    this->playerX = newX;
    this->playerY = newY;
    return true;
}

Room* MapData::getCurrentRoom() const {
    return this->currentRoom;
}

std::map<string, Room*> MapData::getRoomMap() const {
    return this->roomMap;
}


unsigned int MapData::getPlayerY() const {
    return this->playerY;
}

unsigned int MapData::getPlayerX() const {
    return this->playerX;
}

MapData::MapData() {

}

MapData::~MapData() {
    for (auto it = roomMap.begin(); it != roomMap.end(); it++)
        delete it->second;
}

void MapData::load(const string parentDir, const string path, scripting::ScriptOverseer* so) {
    auto j = fs::readJS(fs::join(vector<string>{parentDir, path}));
    try {
        // assign values
        this->playerX = j["startX"];
        this->playerY = j["startY"];
        // load rooms

        for (auto it : j["rooms"].items()) {
            string roomName = it.key();
            if (roomMap.find(roomName) != roomMap.end()) throw std::runtime_error("room " + roomName + " defined more than once");
            
            auto values = it.value();
            unsigned int vr = values["visibleRange"];
            Room* room = new Room(roomName, vr);
            // load tile set
            string tilesetPath = values["tileset"];
            room->loadTilesetFrom(fs::join(vector<string>{parentDir, tilesetPath}), so);
            // load room layout
            string layoutPath = values["path"];
            room->loadLayoutFrom(fs::join(vector<string>{parentDir, layoutPath}), this);
            // load load script
            if (values.contains("loadScript")) {
                string scriptPath = values["loadScript"];
                room->loadLoadScriptFrom(fs::join(vector<string>{parentDir, scriptPath}), so);
            }
            roomMap[roomName] = room;
        }

        string index = j["index"];
        auto it = roomMap.find(index);
        if (it == roomMap.end()) throw std::runtime_error("room " + index + " not declared");
        this->currentRoom = it->second;
    }
    catch (nlohmann::detail::type_error er) {
        throw std::runtime_error(er.what());
    }
}

void MapData::print() {
    using std::cout;
    using std::endl;
    cout << "-- Map --\n\tplayer y: " << this->playerY << "\n\tplayer x: " << this->playerX << endl << "\tRooms:" << endl;
    for (auto it = roomMap.begin(); it != roomMap.end(); it++) {
        if (it->second == currentRoom) cout << "\t\t(Current room)" << endl;
        it->second->print("\t\t");
    }
}

void MapData::addWarpCode(string warpCode, int y, int x, string name) {
    auto it = this->warpMap.find(warpCode);
    if (it != this->warpMap.end()) throw std::runtime_error("warp code " + warpCode + " redefined");
    this->warpMap[warpCode] = RoomPos{x, y, name};
}

void MapData::useWarpCode(string code) {
    auto it1 = this->warpMap.find(code);
    if (it1 == warpMap.end()) throw std::runtime_error("unknown warpcode " + code);
    auto loc = it1->second;
    auto it2 = this->roomMap.find(loc.name);
    if (it2 == this->roomMap.end()) throw std::runtime_error("critical error: bad warpcode " + code + " (room " + loc.name + " not found");
    this->currentRoom = it2->second;
    this->playerX = loc.x;
    this->playerY = loc.y;
}

}
}
