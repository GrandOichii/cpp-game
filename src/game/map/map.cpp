#include "map.hpp"

#include <map>
#include <string>

#include "../scripting/script.hpp"
#include "../scripting/overseer.hpp"
#include "../../nlohmann/json.hpp"
#include "../util.hpp"
#include "../../util.hpp"

using std::string;

namespace game {
namespace map {

MapData::MapData() {

}

MapData::~MapData() {
    for (auto it = roomMap.begin(); it != roomMap.end(); it++)
        delete it->second;
}

void MapData::load(const string parentDir, const string path, const scripting::ScriptOverseer* so) {
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
            room->loadTilesetFrom(fs::join(vector<string>{parentDir, tilesetPath}));
            // load room layout
            string layoutPath = values["path"];
            room->loadLayoutFrom(fs::join(vector<string>{parentDir, layoutPath}));
            // load load script
            string scriptPath = values["loadScript"];
            room->loadLoadScriptFrom(fs::join(vector<string>{parentDir, scriptPath}));
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

}
}
