#pragma once

#include "wrapper.hpp"
#include "context.hpp"
#include "assets.hpp"
#include "../game/core.hpp"

const std::map<int, Double> MOVEMENT_MAP = {
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


class MainPanel : public Context {
private:
    AssetsManager *assets;
    game::Game *game;
    bool updateGame;
public:
    MainPanel(Wrapper *parent) : Context(parent) {
        this->assets = parent->getAssets();
        this->game = parent->getGame();
        this->updateGame = true;
    }
    
    ~MainPanel() {

    }

    void draw() {
        this->drawTiles();
        this->drawPlayer();
        // update game
        if (this->updateGame) this->game->update();
        this->updateGame = false;
    }

    void drawTiles() {
        auto roomName = game->getMap()->getCurrentRoom()->getName();
        auto tiles = this->game->getVisibleTiles();
        for (const auto& tile : tiles) {
            auto x = tile.x;
            auto y = tile.y;
            std::string tileName = roomName + ":" + tile.name;
            auto im = this->assets->getTile(tileName);
            this->parent->drawImage(im, x * TILE_WIDTH, y * TILE_HEIGHT);
        }
    }

    void drawPlayer() {
        auto im = this->assets->getPlayer();
        this->parent->drawImage(im, CENTER_X, CENTER_Y);
    }

    void handleKey(int key) {
        auto it = MOVEMENT_MAP.find(key);
        if (it != MOVEMENT_MAP.end()) {
            auto mpair = it->second;
            updateGame = this->game->movePlayer(mpair);
            return;
        }
        switch (key) {
        case ('f'):
            this->parent->toggleFullscreen();
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

    void inventoryMode() {
        // TODO
    }

    void spellCastMode() {
        // TODO
    }

    void consoleCommandMode() {
        // TODO
    }
};