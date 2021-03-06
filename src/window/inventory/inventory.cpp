#include "inventory.hpp"

#include "menus.hpp"


InventoryWindow::InventoryWindow(Window* parent, AssetsManager* assets, game::Game* game) : parent(parent), assets(assets) {
    this->bg = this->assets->getInventoryBG();
    auto bgSize = getSize(bg);
    this->x = (parent->getWidth() - bgSize.x) / 2;
    this->y = (parent->getHeight() - bgSize.y) / 2;

    menuCount = 1;
    menuLabels = new SDL_Texture*[menuCount];
    menus = new Menu*[menuCount];
    maxLabelWidth = (getSize(bg).x - 2 * MENU_LABELS_X_OFFSET) / menuCount;
    menus[0] = new ItemsMenu(this, assets, game);
    menuLabels[0] = assets->getMessage("Items");
    menuLabelXs = new int[menuCount];
    for (int i = 0; i < menuCount; i++) {
        auto size = getSize(menuLabels[i]);
        menuLabelXs[i] = this->x + (maxLabelWidth - size.x) / 2 + i*maxLabelWidth + MENU_LABELS_X_OFFSET;
    }
    this->fs = assets->getFontSize();

}

InventoryWindow::~InventoryWindow() {
    for (int i = 0; i < menuCount; i++) {
        delete menus[i];
        SDL_DestroyTexture(menuLabels[i]);
    }
    delete[] menuLabelXs;
    delete[] menuLabels;
    delete[] menus;
}

Window* InventoryWindow::getParent() {
    return parent;
}

void InventoryWindow::handleKey(int key) {
    if (key == SDLK_ESCAPE) {
        this->running = false;
        return;
    }
    switch(key) {
    case 'q':
        menuI--;
        if (menuI < 0) menuI = menuCount - 1;
        return;
    case 'e':
        menuI++;
        if (menuI >= menuCount) menuI = 0;
        return;
    }
    menus[menuI]->handleKey(key);
}

void InventoryWindow::start() {
    auto event = parent->getEvent();
    this->running = true;
    while (running) {
        // handle input
        while (SDL_PollEvent(event)) {
            switch (event->type) {
            case SDL_QUIT:
                parent->close();
                running = false;
            case SDL_KEYDOWN:
                auto key = event->key.keysym.sym;
                this->handleKey(key);
            }
        }
        // draw
        parent->drawTexture(bg, this->x, this->y);
        for (int i = 0; i < menuCount; i++) {
            parent->drawRect(this->x + i * maxLabelWidth + MENU_LABELS_X_OFFSET, this->y + MENU_LABELS_Y_OFFSET, maxLabelWidth, fs, SDL_Color{0, 0, 150, 0}, i == menuI);
            parent->drawTexture(menuLabels[i], menuLabelXs[i], this->y + MENU_LABELS_Y_OFFSET);
        }
        menus[menuI]->draw(this->x, this->y + fs);
        parent->flush();
    }
}

void InventoryWindow::close() { this->running = false; }