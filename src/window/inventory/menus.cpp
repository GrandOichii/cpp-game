#include "menus.hpp"

#include <vector>

#include "inventory.hpp"
#include "description.hpp"
#include "../util.hpp"

Menu::Menu(InventoryWindow* parent) : parent(parent) {
}

ItemsSubMenu::ItemsSubMenu(vector<game::player::ItemStruct*> items, InventoryWindow* parent, AssetsManager* assets, game::Game* game) : Menu(parent), assets(assets), game(game){
    // std::cout << "Creating sub menu" << std::endl;
    this->items = new ListTemplate<ItemSlot*>(MAX_ITEMS);
    if (items.size() == 0) return;
    for (const auto& i : items)
        this->items->add(new ItemSlot(game, i, assets, parent->getParent()));
}

ItemsSubMenu::~ItemsSubMenu() {
    for (const auto& e : items->getAll()) delete e;
    delete items;
}

void ItemsSubMenu::rememberCurrent() {
    auto item = items->getCurrent()->getItem();
    item->item->setKnown(true);
}

void ItemsSubMenu::draw(int x, int y) {
    if (this->items->size() == 0) return;
    auto visible = items->getVisible();
    auto c = items->getCursor();
    for (int i = 0; i < visible.size(); i++) 
        visible[i]->draw(x + 10 + ITEMS_X_OFFSET, y + SLOT_HEIGHT * i + ITEMS_Y_OFFSET, i == c);
}

void ItemsSubMenu::handleKey(int key) {
    if (key == SDLK_ESCAPE) {
        parent->close();
        return;
    }
    if (items->size() == 0) return;
    // std::cout << "moving" << std::endl;
    switch(key) {
    case SDLK_UP:
        rememberCurrent();
        items->moveUp();
        break;
    case SDLK_DOWN:
        rememberCurrent();
        items->moveDown();
        break;
    case SDLK_RETURN:
        rememberCurrent();
        auto current = items->getCurrent()->getItem();
        auto w = new DescriptionWindow(parent->getParent(), assets, game, current->item, current->amount);
        w->start();
        delete w;
    }
}

const std::string ORDERED_LABELS[] = {"All", "Weapons", "Armor", "Ammo", "Other"};
const int LABELS_COUNT = 5;

ItemsMenu::ItemsMenu(InventoryWindow* parent, AssetsManager* assets, game::Game* game) : Menu(parent) {
    auto player = game->getPlayer();
    auto inventory = player->getInventory();
    auto sorted = inventory->getSorted();
    menuCount = LABELS_COUNT;
    menuLabels = new SDL_Texture*[menuCount];
    menus = new Menu*[menuCount];
    auto bg = assets->getInventoryBG();
    maxLabelWidth = (getSize(bg).x - 2 * MENU_LABELS_X_OFFSET) / menuCount ;
    // for (auto it = sorted.begin(); it != sorted.end(); it++) {
    //     std::cout << it->first << ":" << std::endl;
    //     for (const auto& pair : it->second)
    //         std::cout << "\t" << pair->item->getName() << std::endl;
    // }
    for (int i = 0; i < menuCount; i++) {
        menuLabels[i] = assets->getMessage(ORDERED_LABELS[i]);
        auto it = sorted.find(ORDERED_LABELS[i]);
        // if (it == sorted.end()) throw std::runtime_error("unknown item category: " + ORDERED_LABELS[i]);
        vector<game::player::ItemStruct*> items(0);
        if (it != sorted.end()) items = it->second;
        menus[i] = new ItemsSubMenu(items, parent, assets, game);
    }
    menuLabelXs = new int[menuCount];
    for (int i = 0; i < menuCount; i++) {
        auto size = getSize(menuLabels[i]);
        menuLabelXs[i] = (maxLabelWidth - size.x) / 2 + i*maxLabelWidth + MENU_LABELS_X_OFFSET;
    }
    this->fs = assets->getFontSize();
}

ItemsMenu::~ItemsMenu() {
    for (int i = 0; i < menuCount; i++) {
        delete menus[i];
        SDL_DestroyTexture(menuLabels[i]);
    }
    delete[] menuLabelXs;
    delete[] menuLabels;
    delete[] menus;
}

void ItemsMenu::draw(int x, int y) {
    for (int i = 0; i < menuCount; i++) {
        parent->getParent()->drawRect(x + i * maxLabelWidth + MENU_LABELS_X_OFFSET, y + MENU_LABELS_Y_OFFSET, maxLabelWidth, fs, SDL_Color{255, 0, 0, 0}, i == menuI);
        parent->getParent()->drawTexture(menuLabels[i], x + menuLabelXs[i], y + MENU_LABELS_Y_OFFSET);
    }
    menus[menuI]->draw(x, y + fs);
}

void ItemsMenu::handleKey(int key) {
    if (key == SDLK_ESCAPE) {
        this->parent->close();
        return;
    }
    switch(key) {
    case SDLK_LEFT:
        menuI--;
        if (menuI < 0) menuI = menuCount - 1;
        return;
    case SDLK_RIGHT:
        menuI++;
        if (menuI >= menuCount) menuI = 0;
        return;
    }
    menus[menuI]->handleKey(key);
}
