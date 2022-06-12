#pragma once

#include <SDL.h>
#include <string>
#include <iostream>
#include <vector>

#include "../window.hpp"
#include "../assets.hpp"
#include "../util.hpp"
#include "../../util.hpp"
#include "../../game/core.hpp"
#include "../../game/items/item.hpp"
#include "../../game/player/inventory.hpp"

static constexpr int MAX_ITEMS = 10;
static constexpr int ITEMS_Y_OFFSET = 20;
static constexpr int ITEMS_X_OFFSET = 20;
static constexpr int MENU_LABELS_Y_OFFSET = 15;
static constexpr int MENU_LABELS_X_OFFSET = 15;

class Menu;

class InventoryWindow {
private:
    Window* parent;
    AssetsManager* assets;
    SDL_Texture* bg;
    bool running = false;
    int y;
    int x;
    int fs;

    int maxLabelWidth;
    int menuCount;
    Menu** menus;
    SDL_Texture** menuLabels;
    int* menuLabelXs;
    int menuI = 0;

public:
    InventoryWindow(Window* parent, AssetsManager* assets, game::Game* game);
    ~InventoryWindow();
    Window* getParent();
    void handleKey(int key);
    void start();
    void close();
};
