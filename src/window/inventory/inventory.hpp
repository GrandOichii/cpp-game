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
static constexpr int ITEMS_Y_OFFSET = 30;
static constexpr int ITEMS_X_OFFSET = 20;

const std::string ORDERED_LABELS[] = {"All", "Weapons", "Armor", "Ammo", "Other"};
const int LABELS_COUNT = 5;

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
