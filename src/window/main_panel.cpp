#include "main_panel.hpp"

#include "inventory/inventory.hpp"
#include "wrapper.hpp"
#include "container.hpp"
#include "context.hpp"
#include "assets.hpp"
#include "../game/core.hpp"
#include "../game/items/container.hpp"
#include "../game/spells/builder.hpp"

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

MainPanel::MainPanel(Wrapper *parent) : Context(parent) {
    this->tileCountX = TILE_COUNT_X;
    this->logWidth = LOG_WIDTH;
    this->centerX = this->tileCountX / 2 * TILE_HEIGHT;
    this->assets = parent->getAssets();
    this->game = parent->getGame();
    this->commandLine = new CommandLine(parent, assets, game->getScriptOverseer(), 40, 20, WINDOW_HEIGHT - LOG_HEIGHT - assets->getFontSize());
    this->updateGame = true;
    auto fs = assets->getFontSize();
    auto lineAmount = LOG_HEIGHT / fs;
    this->logs = new CircularBuffer<std::string>(lineAmount);    
}

// void MainPanel::setupInfo() {
//     auto player = game->getPlayer();
//     this->nameLabelTex = assets->getMessage("Name:  ");
//     this->nameTex = assets->getMessage(player->getName(), SDL_Color{0, 200, 0, 0});
//     this->classLabelTex = assets->getMessage("Class: ");
//     this->classTex = assets->getMessage(player->getClassName());
// }

MainPanel::~MainPanel() {
    // delete iContext;
    delete commandLine;
    // SDL_DestroyTexture(nameLabelTex);
    // SDL_DestroyTexture(nameTex);
    // SDL_DestroyTexture(classLabelTex);
    // SDL_DestroyTexture(classTex);
}

void MainPanel::clearFocused() {
    for (int i = 0; i < 3; i++)
        for (int ii = 0; ii < 3; ii++)
            this->focusedTiles[i][ii] = false;
}

void MainPanel::draw() {
    this->drawTiles();
    this->drawLog();
    this->drawInfo();
    this->drawPlayer();
    // update game
    if (this->updateGame) this->game->update();
    this->updateGame = false;
}

void MainPanel::drawLog() {
    // auto tex = assets->getLogBackground();
    // this->parent->drawTexture(tex, 0, LOG_Y);
    this->parent->drawRect(0, LOG_Y, this->logWidth, LOG_HEIGHT, SDL_Color{80, 80, 80, 0}, true);
    auto lines = logs->getV();
    auto fs = this->assets->getFontSize();
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i].size() == 0) continue;
        auto tex = assets->getMessage(lines[i], SDL_Color{3, 138, 255});
        parent->drawTexture(tex, 5, LOG_Y + 5 + fs * i);
        SDL_DestroyTexture(tex);
    }
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
                this->parent->drawTexture(im, this->centerX + (ii-1)*TILE_WIDTH, CENTER_Y + (i-1)*TILE_HEIGHT);
}

void MainPanel::drawPlayer() {
    auto im = this->assets->getPlayer();
    this->parent->drawTexture(im, this->centerX, CENTER_Y);
    for (const auto& tex : apparel)
        this->parent->drawTexture(tex, this->centerX, CENTER_Y);

}

void MainPanel::drawInfo() {
    auto x = this->tileCountX * TILE_WIDTH;
    auto y = 0;
    auto bg = assets->getInfoBG();
    this->parent->drawTexture(bg, x, y);

    // draw name
    // parent->drawTexture(nameLabelTex, x + 10, y + 10);
    // draw class name
    // draw health and mana
    auto player = game->getPlayer();
    int xOffset = 10;
    int yOffset = 10;
    auto size = getSize(bg);
    auto barWidth = size.x - 2 * xOffset;
    auto barHeight = 20;

    parent->drawRect(x + xOffset, y + yOffset, barWidth, barHeight, SDL_Color{0, 0, 0, 0}, true);
    parent->drawRect(x + xOffset, y + yOffset, barWidth, barHeight, SDL_Color{255, 200, 200, 0}, false);
    auto health = (barWidth - 2) * player->getCurrentHealth() / player->getMaxHealth();
    parent->drawRect(x + xOffset + 1, y + yOffset + 1, health, barHeight - 2, SDL_Color{255, 0, 0, 0}, true);

    y += barHeight * 1.5;

    parent->drawRect(x + xOffset, y + yOffset, barWidth, barHeight, SDL_Color{0, 0, 0, 0}, true);
    parent->drawRect(x + xOffset, y + yOffset, barWidth, barHeight, SDL_Color{255, 200, 200, 0}, false);
    auto mana = (barWidth - 2) * player->getCurrentMana() / player->getMaxMana();
    parent->drawRect(x + xOffset + 1, y + yOffset + 1, mana, barHeight - 2, SDL_Color{0, 0, 255, 0}, true);
}

void MainPanel::toggleFullscreen() {
    this->parent->toggleFullscreen();
    auto width = parent->getWindowSize().first;
    this->tileCountX = (width - INFO_WIDTH) / TILE_WIDTH;
    this->game->setWindowSize(tileCountX, TILE_COUNT_Y);
    this->centerX = this->tileCountX / 2 * TILE_WIDTH;
    // change update logs
    this->logWidth = width - INFO_WIDTH;
    this->logs->clear();
    auto ll = this->game->getLastLogs(this->logs->getSize());
    for (const auto& m : ll) {
        if (m == "") continue;
        this->updateLog(m);
    }
}

void MainPanel::handleKey(int key) {
    auto it = DIRECTION_MAP.find(key);
    if (it != DIRECTION_MAP.end()) {
        auto mpair = it->second;
        updateGame = this->game->movePlayer(mpair);
        return;
    }
    switch (key) {
    case 'f':
        this->toggleFullscreen();
        return;
    case 'e':
        this->interactMode();
        return;
    case 'i':
        this->inventoryMode();
        // change if item usage makes the game update
        return;
    case 'c':
        this->spellCastMode();
        // change if casting spells makes the game update
        return;
    case '/':
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
    auto w = new InventoryWindow(this->parent, this->assets, this->game);
    w->start();
    delete w;
}

void MainPanel::spellCastMode() {
    std::string line = "SELF HEAL LOW";
    auto spell = game::spells::parse(game, line);
    spell->cast(this->game);
    delete spell;
}

void MainPanel::consoleCommandMode() {
    this->commandLine->show();
}

void MainPanel::updateLog(string message) {
    const int xOffset = 10;
    auto longt = assets->getMessage(message);
    auto mwidth = getSize(longt).x;
    int maxWidth = (message.size() * (this->logWidth)) / mwidth;
    auto lines = str::widthSplit(message, maxWidth);
    for (int i = 0; i < lines.size(); i++) logs->add(lines[i]);
}

void MainPanel::sleep(int amount) {
    this->parent->clear();
    this->draw();
    this->parent->flush();
    SDL_Delay(amount);
}

string MainPanel::requestChoice(string text, string choices) {
    std::string result = showMessageBox(this->parent, this->assets, text, str::split(choices, "_"));
    return result;
}

bool MainPanel::accessContainer(game::items::Container* container, std::string top) {
    auto w = new ContainerWindow(this->parent, assets, container, top, 20, 20);
    auto result = w->start();
    delete w;
    return result;
}

void MainPanel::updatePlayerLook() {
    // TODO
    apparel.clear();
    auto items = game->getPlayer()->getAllEquipment();
    for (const auto& item : items)
        apparel.push_back(assets->getApparelItem(item->getName()));
}
