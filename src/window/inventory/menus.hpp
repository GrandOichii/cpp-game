#pragma once

#include "../util.hpp"
#include "inventory.hpp"

constexpr int SLOT_HEIGHT = 40;
constexpr int SLOT_WIDTH = 550;

class ItemSlot {
private:
    game::player::ItemStruct* item;
    game::Game* game;
    Window* parent;
    AssetsManager* assets;
    SDL_Renderer* renderer;
    SDL_Texture* image;
    SDL_Texture* label;
    int imageYOffset;
    int labelYOffset;
    int labelWidth;
    int width;
public:
    ItemSlot(game::Game* game, game::player::ItemStruct* item, AssetsManager* assets, Window* parent) : game(game), item(item), assets(assets), parent(parent) {
        this->renderer = parent->getRenderer();
        auto text = item->item->getDisplayName();
        if (item->amount > 1) text += " x " + std::to_string(item->amount); 
        this->label = assets->getMessage(text);
        auto size = getSize(label);
        this->labelWidth = size.x;
        this->image = assets->getItemTexture(item->item->getName());
        size = getSize(image);
        this->imageYOffset = (SLOT_HEIGHT - size.y) / 2;
        this->labelYOffset = (SLOT_HEIGHT - assets->getFontSize()) / 2;
    }

    ~ItemSlot() {
    }

    void draw(int x, int y, bool focused) {
        this->parent->drawRect(x, y, SLOT_WIDTH, SLOT_HEIGHT, SDL_Color{120, 120, 120, 0}, focused);
        this->parent->drawTexture(this->image, x + 10, y + imageYOffset);
        this->parent->drawTexture(this->label, x + 50, y + labelYOffset);
        if (this->item->item->isKnown()) return;
        this->parent->drawTexture(assets->getNewItemTex(), x + 50 + labelWidth + 10, y + labelYOffset);
    }

    game::player::ItemStruct* getItem() { return this->item; }
};

class InventoryWindow;

class Menu {
protected:
    InventoryWindow* parent;
public:
    Menu(InventoryWindow* parent);

    virtual ~Menu() = default;
    virtual void draw(int x, int y) = 0;
    virtual void handleKey(int key) = 0;
};

class ItemsSubMenu : public Menu {
private:
    game::Game* game;
    AssetsManager* assets;
    ListTemplate<ItemSlot*>* items;
public:
    ItemsSubMenu(vector<game::player::ItemStruct*> items, InventoryWindow* parent, AssetsManager* assets, game::Game* game);
    ~ItemsSubMenu();
    void rememberCurrent();
    void draw(int x, int y);
    void handleKey(int key);
};

class ItemsMenu : public Menu {
private:
    int maxLabelWidth;
    int menuCount;
    Menu** menus;
    SDL_Texture** menuLabels;
    int* menuLabelXs;
    int menuI = 0;
    int fs;
public:
    ItemsMenu(InventoryWindow* parent, AssetsManager* assets, game::Game* game);
    ~ItemsMenu();
    void rememberCurrent();
    void draw(int x, int y);
    void handleKey(int key);
};