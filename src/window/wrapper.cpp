#include "wrapper.hpp"

#include <SDL.h>

#include "window.hpp"
#include "assets.hpp"
#include "mainPanel.hpp"
#include "../game/core.hpp"

Wrapper::Wrapper(std::string path, std::string assetsPath) : Window() {
    this->game = new game::Game(path.c_str());
    this->game->setWindowSize(TILE_COUNT_X, TILE_COUNT_Y);
    this->title = this->game->getInfo()->getName().c_str();
    this->assetsPath = assetsPath;
}

Wrapper::~Wrapper() {
    delete assets;
    delete game;
}

void Wrapper::setup() {
    this->assets = new AssetsManager(assetsPath, ren);
    // set the icon
    SDL_Surface *icon = SDL_LoadBMP(fs::join(assetsPath, "icon.bmp").c_str());
    if (icon == nullptr){
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("");
    }
    SDL_SetWindowIcon(win, icon);
    SDL_FreeSurface(icon);
    auto tilePanel = new MainPanel(this);
    this->setCurrentContext(tilePanel);
    this->game->start();
}

AssetsManager * Wrapper::getAssets() const {
    return this->assets;
}

game::Game * Wrapper::getGame() const {
    return this->game;
}