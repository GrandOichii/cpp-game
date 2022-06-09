#pragma once

#include "window.hpp"
#include "assets.hpp"
#include "../game/core.hpp"

constexpr int TILE_WIDTH = 32;
constexpr int TILE_HEIGHT = 32;
constexpr int TILE_COUNT_Y = 15;
constexpr int TILE_COUNT_X = 21;
constexpr int CENTER_Y = TILE_COUNT_Y / 2 * TILE_HEIGHT;
constexpr int CENTER_X = TILE_COUNT_X / 2 * TILE_HEIGHT;
constexpr int LOG_Y = TILE_HEIGHT * TILE_COUNT_Y;
constexpr int LOG_HEIGHT = 5 * TILE_HEIGHT;
constexpr int LOG_WIDTH = TILE_COUNT_X * TILE_WIDTH;
constexpr int WINDOW_HEIGHT = LOG_Y + LOG_HEIGHT;
constexpr int WINDOW_WIDTH = TILE_WIDTH * TILE_COUNT_X;

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