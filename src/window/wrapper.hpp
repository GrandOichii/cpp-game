#pragma once

#include "window.hpp"
#include "assets.hpp"
#include "../game/core.hpp"


class Wrapper : public Window {
private:
    game::Game *game;
    AssetsManager *assets;
    std::string assetsPath;
    void setup();

public:
    Wrapper(std::string path, std::string assetsPath);
    ~Wrapper();
    AssetsManager *getAssets() const;
    game::Game *getGame() const;
};