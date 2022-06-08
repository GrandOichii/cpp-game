#pragma once

#include "wrapper.hpp"
#include "context.hpp"
#include "assets.hpp"
#include "../game/core.hpp"
#include "util.hpp"

#include "customContexts.hpp"

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

class InteractContext : public Context {
private:
    std::function<void(void)> preDraw;
    std::function<void(Double, bool)> onClose;
    AssetsManager *assets;
    SDL_Texture *tex;
    int x;

public:
    InteractContext(Window *parent, AssetsManager *assets, std::function<void(void)> preDraw, std::function<void(Double, bool)> onClose) : Context(parent), assets(assets), preDraw(preDraw), onClose(onClose) {
        this->tex = assets->getMessage("[ Interact where? ]", SDL_Color{0, 255, 0, 0});
        auto size = getSize(this->tex);
        this->x = (WINDOW_WIDTH - size.x) / 2;
    }

    ~InteractContext() {
        SDL_DestroyTexture(tex);
    }

    void draw() {
        this->preDraw();
        this->parent->drawTexture(this->tex, this->x, 10);
    }

    void handleKey(int key) {
        if (key == SDLK_ESCAPE) {
            this->onClose(Double{0, 0}, false);
            return;
        }
        auto it = DIRECTION_MAP.find(key);
        if (it == DIRECTION_MAP.end()) return;
        this->onClose(it->second, true);
    }
};

class MainPanel : public Context, public game::GameWrapper {
private:
    AssetsManager *assets;
    game::Game *game;
    bool updateGame;
    InteractContext* iContext;
    bool focusedTiles[3][3]{};
public:
    MainPanel(Wrapper *parent) : Context(parent) {
        this->assets = parent->getAssets();
        this->game = parent->getGame();
        this->updateGame = true;
        auto onClose = [this](Double dir, bool interact){
            this->clearFocused();
            this->parent->setCurrentContext(this);
            if (!interact) return;
            this->game->interactAt(dir.a[0], dir.a[1]);
        };
        auto preDraw = [this](){
            this->draw();
        };
        this->iContext = new InteractContext(this->parent, assets, preDraw, onClose);
    }
    
    ~MainPanel() {
        delete iContext;
    }

    void clearFocused() {
        for (int i = 0; i < 3; i++)
            for (int ii = 0; ii < 3; ii++)
                this->focusedTiles[i][ii] = false;
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
            this->parent->drawTexture(im, x * TILE_WIDTH, y * TILE_HEIGHT);
        }
        auto im = this->assets->getFocusedTile();
        for (int i = 0; i < 3; i++)
            for (int ii = 0; ii < 3; ii++)
                if (focusedTiles[i][ii])
                    this->parent->drawTexture(im, CENTER_X + (ii-1)*TILE_WIDTH, CENTER_Y + (i-1)*TILE_HEIGHT);
    }

    void drawPlayer() {
        auto im = this->assets->getPlayer();
        this->parent->drawTexture(im, CENTER_X, CENTER_Y);
    }

    void handleKey(int key) {
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

    void interactMode() {
        auto pairs = this->game->getAdjacentInteractableTiles();
        for (const auto& pair : pairs)
            this->focusedTiles[pair.second+1][pair.first+1] = true;
        this->parent->setCurrentContext(iContext);
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

    void updateLog(string message) {
        // TODO
    }

    void sleep(int amount) {
        std::cout << "Sleeping for " << amount << std::endl;
        SDL_Delay(amount);
        this->parent->draw();
    }
};