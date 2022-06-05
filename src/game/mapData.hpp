#pragma once

#include <map>

#include "util.hpp"

using std::string;

class Room {
private:
    unsigned int height;
    unsigned int width;
    string name;
public:

};

class MapData : public ILoadable {
private:
    unsigned int playerY;
    unsigned int playerX;
    std::map<string, Room*> roomMap;
    Room* currentRoom;
public:
    MapData() {

    }

    ~MapData() {
        for (auto it = roomMap.begin(); it != roomMap.end(); it++)
            delete it->second;
    }

    void load(const string parentDir, const string path) {
        auto j = fs::readJS(fs::pathJoin(vector<string>{parentDir, path}));
        try {
            // assign values
            this->playerX = j["startX"];
            this->playerY = j["startY"];
            // load rooms

            for (auto it : j["rooms"].items()) {
                string roomName = 
                std::cout << it.key() << std::endl;
            }

            // string index = j["index"];
            // auto it = roomMap.find(index);
            // if (it == roomMap.end()) throw std::runtime_error("room " + index + " not declared");
            // this->currentRoom = it->second;
        }
        catch (nlohmann::detail::type_error er) {
            throw std::runtime_error("failed loading map data");
        }
    }

    void print() {
        using std::cout;
        using std::endl;
        cout << "-- Map --\n\tplayer y: " << this->playerY << "\n\tplayer x: " << this->playerX << endl;
    }
};