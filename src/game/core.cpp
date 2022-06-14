#include "core.hpp"

static constexpr int LOG_COUNT = 5;
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

void Game::setWrapper(GameWrapper * wrapper) {
    this->wrapper = wrapper;
}

GameWrapper * Game::getWrapper() {
    return this->wrapper;
}

Game::Game(const char* path) {
    this->scriptOverseer = new scripting::ScriptOverseer(this);
    this->gameInfo = new GameInfo();
    this->mapData = new map::MapData();
    this->itemManager = new items::ItemManager();
    this->containerManager = new items::ContainerManager();
    this->classManager = new player::ClassManager();
    auto p = string(path);

    // load items before everything else
    itemManager->load(p, ITEMS_FILE.c_str(), this->scriptOverseer);
    std::map<string, ILoadable*> loadMap = {
        {GAME_INFO_FILE, gameInfo},
        {MAPDATA_FILE, mapData},
        {CONTAINERS_FILE, containerManager},
        {CLASSES_FILE, classManager}
    };
    for (auto it = loadMap.begin(); it != loadMap.end(); it++) {
        it->second->load(p, it->first.c_str(), this->scriptOverseer);
    }

    this->logs = new CircularBuffer<std::string>(LOG_COUNT);
}

std::string Game::createPlayer(std::string savesDir, std::string name, std::string className) {
    player::PClass* pc = nullptr;
    auto count = classManager->getCount();
    auto classes = classManager->getClasses();
    for (int i = 0; i < count; i++)
        if (className == classes[i]->getName())
            pc = classes[i];
    if (!pc) throw std::runtime_error("no class with name " + className);
    this->player = new player::Player(name, pc);
    return "";
}

void Game::loadPlayer(std::string saveFile) {
    
}

items::ItemManager* Game::getItemManager() {
    return this->itemManager;
}

items::ContainerManager* Game::getContainerManager() {
    return containerManager;
}

Game::~Game() {
    delete scriptOverseer;
    delete classManager;
    delete gameInfo;
    delete mapData;
    delete itemManager;
    delete containerManager;
    delete logs;
    delete player;
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

void Game::addToLog(std::string message) {
    message = "- " + message;
    this->logs->add(message);
    this->wrapper->updateLog(message);
    // for (auto v : this->logs->getV())
    //     std::cout << v << std::endl;
}

constexpr int DIR_COUNT = 8;
constexpr int DIRS[DIR_COUNT][2] = {
    {0, -1},
    {0, 1},
    {-1, 0},
    {1, 0},
    {-1, -1},
    {1, 1},
    {1, -1},
    {-1, 1}
};

vector<std::pair<int, int>> Game::getAdjacentInteractableTiles() {
    vector<std::pair<int, int>> result;
    auto room = mapData->getCurrentRoom();
    auto layout = room->getLayout();
    auto height = room->getHeight();
    auto width = room->getWidth();
    for (int i = 0; i < DIR_COUNT; i++) {
        auto dir = DIRS[i];
        auto x = dir[0] + mapData->getPlayerX();
        auto y = dir[1] + mapData->getPlayerY();
        if (y < 0 || x < 0 || y >= height || x >= width) {
            continue;
        }
        auto tile = layout[y][x];
        if (tile->hasInteractScript()) {
            result.push_back(std::make_pair(dir[0], dir[1]));
        }
    }
    return result;
}

void Game::interactAt(int xdiff, int ydiff) {
    auto pX = this->mapData->getPlayerX();
    auto pY = this->mapData->getPlayerY();
    auto layout = this->mapData->getCurrentRoom()->getLayout();
    layout[pY+ydiff][pX+xdiff]->execInteractScript();
}

void Game::sleep(int amount) {
    this->wrapper->sleep(amount);
}

string Game::requestChoice(string text, string choices) {
    return this->wrapper->requestChoice(text, choices);
}

bool Game::accessContainer(items::Container* container, std::string top) {
    return this->wrapper->accessContainer(container, top);
}

vector<string> Game::getLastLogs(int count) {
    return this->logs->getLast(count);
}

player::Player* Game::getPlayer() const {
    return this->player;
}

player::ClassManager* Game::getClassManager() {
    return classManager;
}

scripting::ScriptOverseer* Game::getScriptOverseer() {
    return scriptOverseer;
}

void Game::updatePlayerLook() {
    this->wrapper->updatePlayerLook();
}

}