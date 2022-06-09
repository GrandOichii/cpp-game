#include "main_panel.hpp"

#include "wrapper.hpp"
#include "context.hpp"
#include "assets.hpp"
#include "../game/core.hpp"
#include "util.hpp"

const std::map<int, Double> DIRECTION_MAP = {
    //                    x   y
    {SDLK_LEFT  , Double{-1,  0}},
    {SDLK_RIGHT , Double{ 1,  0}},
    {SDLK_UP    , Double{ 0, -1}},
    {SDLK_DOWN  , Double{ 0,  1}},
    {'y'        , Double{-1, -1}},
    {'u'        , Double{ 1, -1}},
    {'b'        , Double{-1,  1}},
    {'n'        , Double{ 1,  1}}
};

InteractContext::InteractContext(Window *parent, AssetsManager *assets, std::function<void(void)> preDraw, std::function<void(Double, bool)> onClose) : Context(parent), assets(assets), preDraw(preDraw), onClose(onClose) {
    this->tex = assets->getMessage("[ Interact where? ]", SDL_Color{0, 255, 0, 0});
    auto size = getSize(this->tex);
    this->x = (WINDOW_WIDTH - size.x) / 2;
}

InteractContext::~InteractContext() {
    SDL_DestroyTexture(tex);
}

void InteractContext::draw() {
    this->preDraw();
    this->parent->drawTexture(this->tex, this->x, 10);
}

void InteractContext::handleKey(int key) {
    if (key == SDLK_ESCAPE) {
        this->onClose(Double{0, 0}, false);
        return;
    }
    auto it = DIRECTION_MAP.find(key);
    if (it == DIRECTION_MAP.end()) return;
    this->onClose(it->second, true);
}

MainPanel::MainPanel(Wrapper *parent) : Context(parent) {
    this->assets = parent->getAssets();
    this->game = parent->getGame();
    this->updateGame = true;
    auto preDraw = [this](){
        this->draw();
    };
    auto onClose1 = [this](Double dir, bool interact){
        this->clearFocused();
        this->parent->setCurrentContext(this);
        std::cout << "Context set to main" << std::endl;

        // this->parent->draw();
        if (!interact) return;
        this->game->interactAt(dir.a[0], dir.a[1]);
    };
    this->iContext = new InteractContext(this->parent, assets, preDraw, onClose1);
}

MainPanel::~MainPanel() {
    delete iContext;
}

void MainPanel::clearFocused() {
    for (int i = 0; i < 3; i++)
        for (int ii = 0; ii < 3; ii++)
            this->focusedTiles[i][ii] = false;
}

void MainPanel::draw() {
    this->drawTiles();
    this->drawPlayer();
    // update game
    if (this->updateGame) this->game->update();
    this->updateGame = false;
}

void MainPanel::drawTiles() {
    auto roomName = game->getMap()->getCurrentRoom()->getName();
    auto tiles = this->game->getVisibleTiles();
    for (const auto& tile : tiles) {
        auto x = tile.x;
        auto y = tile.y;
        std::string tileName = roomName + ":" + tile.name;
        auto im = this->assets->getTile(tileName);
        this->parent->drawTexture(im, x * TILE_WIDTH, y * TILE_HEIGHT);
    }
    auto im = this->assets->getFocusedTile();
    for (int i = 0; i < 3; i++)
        for (int ii = 0; ii < 3; ii++)
            if (focusedTiles[i][ii])
                this->parent->drawTexture(im, CENTER_X + (ii-1)*TILE_WIDTH, CENTER_Y + (i-1)*TILE_HEIGHT);
}

void MainPanel::drawPlayer() {
    auto im = this->assets->getPlayer();
    this->parent->drawTexture(im, CENTER_X, CENTER_Y);
}

void MainPanel::handleKey(int key) {
    auto it = DIRECTION_MAP.find(key);
    if (it != DIRECTION_MAP.end()) {
        auto mpair = it->second;
        updateGame = this->game->movePlayer(mpair);
        return;
    }
    switch (key) {
    case ('f'):
        this->parent->toggleFullscreen();
        return;
    case ('e'):
        this->interactMode();
        return;
    case ('i'):
        this->inventoryMode();
        // change if item usage makes the game update
        return;
    case ('c'):
        this->spellCastMode();
        // change if casting spells makes the game update
        return;
    case ('~'):
        this->consoleCommandMode();
        return;
    case (SDLK_ESCAPE):
        this->parent->close();
        return;
    }
}

void MainPanel::interactMode() {
    auto pairs = this->game->getAdjacentInteractableTiles();
    for (const auto& pair : pairs)
        this->focusedTiles[pair.second+1][pair.first+1] = true;
    // this->parent->setCurrentContext(iContext);
    auto tex = assets->getMessage("[ Interact where? ]", SDL_Color{0, 255, 0, 0});
    // auto tex = assets->getPlayer();
    auto size = getSize(tex);
    auto x = (WINDOW_WIDTH - size.x) / 2;
    this->draw();
    this->parent->drawTexture(tex, x, 10);
    this->parent->flush();
    auto event = this->parent->getEvent();
    bool interact = false;
    Double dir;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(event)) {
            switch(event->type) {
            case SDL_QUIT:
                this->parent->close();
                return;
            case SDL_KEYDOWN:
                auto key = event->key.keysym.sym;
                auto it = DIRECTION_MAP.find(key);
                if (it != DIRECTION_MAP.end()) {
                    dir = it->second;
                    interact = true;
                }
                running = false;
            }
        }
    }
    SDL_DestroyTexture(tex);
    tex = nullptr;
    this->clearFocused();
    if (interact) this->game->interactAt(dir.a[0], dir.a[1]);
}

void MainPanel::inventoryMode() {
    // TODO
    
}

void MainPanel::spellCastMode() {
    // TODO
}

void MainPanel::consoleCommandMode() {
    // TODO
}

void MainPanel::updateLog(string message) {
    // TODO
    std::cout << message << std::endl;
}

void MainPanel::sleep(int amount) {
    this->parent->clear();
    this->draw();
    this->parent->flush();
    std::cout << "Sleeping for " << amount << std::endl;
    SDL_Delay(amount);
}

string MainPanel::requestChoice(string text, string choices) {
    int size = 2;
    std::string* ch = new std::string[size]{"Hello", "there"};
    std::string result = showMessageBox(this->parent, this->assets, text, ch, size);
    delete[] ch;
    return result;
}