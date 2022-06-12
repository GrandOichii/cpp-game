#include "menus.hpp"

#include <vector>

#include "inventory.hpp"
#include "description.hpp"
#include "../util.hpp"

Menu::Menu(InventoryWindow* parent) : parent(parent) {
}

ItemsSubMenu::ItemsSubMenu(vector<game::player::ItemStruct*> items, InventoryWindow* parent, AssetsManager* assets, game::Game* game) : Menu(parent), assets(assets), game(game) {
    this->items = new ListTemplate<ItemSlot*>(MAX_ITEMS);
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
    auto visible = items->getVisible();
    auto c = items->getCursor();
    for (int i = 0; i < visible.size(); i++) 
        visible[i]->draw(x + 10 + ITEMS_X_OFFSET, y + 10 + SLOT_HEIGHT * i + ITEMS_Y_OFFSET, i == c);
}

void ItemsSubMenu::handleKey(int key) {
    if (key == SDLK_ESCAPE) {
        parent->close();
        return;
    }
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