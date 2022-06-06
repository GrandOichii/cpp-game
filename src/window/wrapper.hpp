#pragma once

#include <map>
#include <SDL.h>

#include "assets.hpp"
#include "../util.hpp"
#include "../game/core.hpp"

constexpr int TILE_WIDTH = 32;
constexpr int TILE_HEIGHT = 32;
constexpr int TILE_COUNT_Y = 11;
constexpr int TILE_COUNT_X = 15;
constexpr int CENTER_Y = TILE_COUNT_Y / 2 * TILE_HEIGHT;
constexpr int CENTER_X = TILE_COUNT_X / 2 * TILE_HEIGHT;
constexpr int WINDOW_HEIGHT = TILE_HEIGHT * TILE_COUNT_Y;
constexpr int WINDOW_WIDTH = TILE_WIDTH * TILE_COUNT_X;

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

class Window {
private:
    game::Game *game;
    AssetsManager *assets;
    const char* title;
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;
    bool running;
public:
    Window(const char* path) {
        this->game = new game::Game(path);
        this->game->setWindowSize(TILE_COUNT_X, TILE_COUNT_Y);
        this->title = this->game->getInfo()->getName().c_str();
    }

    ~Window() {
        delete assets;
        delete game;
    }

    void drawImage(SDL_Texture *texture, int x, int y) {
        SDL_Rect pos;
        pos.x = x;
        pos.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
        SDL_RenderCopy(ren, texture, NULL, &pos);
    }

    bool handleKey(int key) {
        auto it = MOVEMENT_MAP.find(key);
        if (it != MOVEMENT_MAP.end()) {
            auto mpair = it->second;
            return this->game->movePlayer(mpair);
        }
        switch (key) {
        case (SDLK_ESCAPE):
            this->running = false;
            return false;
        }
        return false;
    }

    void loadTextures(std::string assetsPath) {
        this->assets = new AssetsManager(assetsPath, ren);
    }

    void draw() {
        this->drawTiles();
        this->drawPlayer();
    }

    void drawTiles() {
        auto roomName = game->getMap()->getCurrentRoom()->getName();
        auto tiles = this->game->getVisibleTiles();
        for (const auto& tile : tiles) {
            auto x = tile.x;
            auto y = tile.y;
            std::string tileName = roomName + ":" + tile.name;
            auto im = this->assets->getTile(tileName);
            this->drawImage(im, x * TILE_WIDTH, y * TILE_HEIGHT);
        }
    }

    void drawPlayer() {
        auto im = this->assets->getPlayer();
        this->drawImage(im, CENTER_X, CENTER_Y);
    }

    void start(std::string assetsPath) {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return;
        }
        this->win = SDL_CreateWindow(this->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (this->win == nullptr){
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            return;
        }
        this->ren = SDL_CreateRenderer(this->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (this->ren == nullptr){
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            return;
        }
        this->loadTextures(assetsPath);
        SDL_Surface *icon = SDL_LoadBMP(fs::join(assetsPath, "icon.bmp").c_str());
        if (icon == nullptr){
            std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
            throw std::runtime_error("");
        }
        SDL_SetWindowIcon(win, icon);
        SDL_FreeSurface(icon);
        this->running = true;
        SDL_Event event;
        while (this->running) {
            // handle events
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT:
                    this->running = false;
                    break;
                case SDL_KEYDOWN:
                    this->handleKey(event.key.keysym.sym);
                }
            }
            SDL_RenderClear(ren);
            this->draw();
            SDL_RenderPresent(ren);
        }
    }
};