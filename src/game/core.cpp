#include "core.hpp"

static constexpr int RAY_AMOUNT = 200;

static constexpr double rayRadOf(int i) {
    return 2 * M_PI * i / RAY_AMOUNT;
}

namespace game {

const string CLASSES_FILE = "classes.json";
const string CONTAINERS_FILE = "containers.json";
const string GAME_INFO_FILE = "game_info.json";
const string ITEMS_FILE = "items.json";
const string MAPDATA_FILE = "mapdata.json";




Game::Game(const char* path) {
    this->scriptOverseer = new scripting::ScriptOverseer(this);
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

Game::~Game() {
    delete scriptOverseer;
    delete gameInfo;
    delete mapData;
}

GameInfo * Game::getInfo() const { 
    return this->gameInfo;
}

map::MapData * Game::getMap() const {
    return this->mapData;
}

void Game::setWindowSize(int width, int height) {
    this->windowWidth = width;
    this->windowHeight = height;
    this->centerX = width / 2;
    this->centerY = height / 2;
}

void Game::print() {
    this->gameInfo->print();
    this->mapData->print();
}

void Game::start() {
    auto room = this->mapData->getCurrentRoom();
    room->executeLoadScript();
}

void Game::update() {
    auto map = this->getMap();
    auto pY = map->getPlayerY();
    auto pX = map->getPlayerX();
    auto tile = map->getCurrentRoom()->getLayout()[pY][pX];
    tile->execStepScript();
}

bool Game::movePlayer(const Double mpair) {
    return this->mapData->movePlayer(mpair);
}

vector<map::RoomPos> Game::getVisibleTiles() {
    int y = this->mapData->getPlayerY();
    int x = this->mapData->getPlayerX();
    auto room = this->mapData->getCurrentRoom();
    auto height = room->getHeight();
    auto width = room->getWidth();
    auto tiles = room->getLayout();
    double vr = room->getVisibleRange();
    vector<map::RoomPos> result;
    for (int i = 0; i < RAY_AMOUNT; i++) {
        double r = rayRadOf(i);
        for (float n = 0; n < vr; n++) {
            int newY = this->centerY + int(sin(r) * n);
            int newX = this->centerX + int(cos(r) * n);
            if (newY > windowHeight || newX > windowWidth){
                break;
            }

            int mi = newY - centerY + y;
            int mj = newX - centerX + x;

            string tileName = "unseen";
            auto transparent = true;
            if (newY < 0 || newX < 0) {
                continue;
            }
            if (mi >= 0 && mj >= 0 && mi < height && mj < width) {
                auto tile = tiles[mi][mj];
                tileName = tile->getName();
                transparent = tile->isTransparent();
            }
            result.push_back(map::RoomPos{newX, newY, tileName});
            if (!transparent) {
                break;
            }
        }
    }
    return result;
}

void Game::useWarpCode(std::string code) {
    auto prevRoom = this->mapData->getCurrentRoom()->getName();
    this->mapData->useWarpCode(code);
    auto room = this->mapData->getCurrentRoom();
    auto newRoom = room->getName();
    if (newRoom != prevRoom) room->executeLoadScript();
}

}