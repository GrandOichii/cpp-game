#pragma once

#include <SDL.h>
#include <string>

#include "window.hpp"
#include "assets.hpp"

#include "../game/items/container.hpp"
#include "../game/items/container.hpp"

#include "util.hpp"
#include "../util.hpp"

constexpr int X_OFFSET = 20;
constexpr int LINE_HEIGHT = 40;

class ItemLine {
private:
    SDL_Texture* label;
    SDL_Texture* image;
    game::items::Item* item;
    int imageY;
    int labelY;
    int amount;
    int width;
    int imageWidth;
public:
    ItemLine(SDL_Texture* label, SDL_Texture* image, game::items::Item* item, int amount, int width) : 
        label(label), 
        image(image), 
        item(item), 
        amount(amount),
        width(width)
    {
        auto size = getSize(image);
        imageWidth = size.x;
        imageY = (LINE_HEIGHT - size.y) / 2;
        labelY = (LINE_HEIGHT - getSize(label).y) / 2;
    }

    ~ItemLine() {
        SDL_DestroyTexture(label);
    }

    void draw(Window* parent, int x, int y) {
        parent->drawTexture(image, x + X_OFFSET, y + imageY);
        parent->drawTexture(label, x + X_OFFSET + imageWidth + 5, y + labelY);
    }
};

constexpr int MAX_ITEM_AMOUNT = 10;

class ContainerWindow {
private:
    Window* parent;
    bool running = false;
    ListTemplate<ItemLine*>* items;
    SDL_Texture* bg;
    SDL_Texture* top;
    SDL_Texture** buttons;
    const int buttonCount = 2;
    int buttonI = 0;
    bool loot;
    int fs;
    int buttonsY;
    int* buttonXs;
    int buttonHeight;
    int buttonWidth;

    int x, y;
public:
    ContainerWindow(Window* parent, AssetsManager* assets, game::items::Container* container, std::string top, int x, int y) : parent(parent), x(x), y(y) {
        this->top = assets->getMessage(top, SDL_Color{250, 0, 250, 0});
        fs = assets->getFontSize();
        auto pairs = container->getPairs();
        loot = false;
        items = new ListTemplate<ItemLine*>(MAX_ITEM_AMOUNT);
        this->bg = assets->getContainerBG();
        auto bgSize = getSize(bg);
        auto width = bgSize.x - X_OFFSET*2;
        for (const auto& pair : pairs) {
            auto item = pair.first;
            auto amount = pair.second;
            std::string displayName = item->getDisplayName();
            displayName += " x " + std::to_string(amount);
            auto label = assets->getMessage(displayName);
            auto image = assets->getItemTexture(item->getName());
            
            auto line = new ItemLine(label, image, item, amount, width);
            items->add(line);
        }
        buttons = new SDL_Texture*[buttonCount];
        buttons[0] = assets->getMessage("Loot all", SDL_Color{255, 255, 255, 0});
        buttons[1] = assets->getMessage("Exit", SDL_Color{255, 255, 255, 0});
        buttonsY = bgSize.y - fs - 30;
        buttonXs = new int[buttonCount];
        buttonWidth = width / 2;
        buttonHeight = 40;
        for (int i = 0; i < buttonCount; i++)
            buttonXs[i] = buttonWidth * i + (buttonWidth - getSize(buttons[i]).x) / 2;
    }

    ~ContainerWindow() {
        for (const auto& e : items->getAll()) {
            delete e;
        } 
        delete items;
        SDL_DestroyTexture(top);
        for (int i = 0; i < buttonCount; i++) {
            SDL_DestroyTexture(buttons[i]);
        }
        delete buttonXs;
        delete[] buttons;
    }

    void handleKey(int key) {
        switch(key) {
        case SDLK_ESCAPE:
            this->running = false;
            return;
        case SDLK_RETURN:
            this->running = false;
            if (buttonI == 0) loot = true;
            return;
        case SDLK_UP:
            this->items->scrollUp();
            return;
        case SDLK_DOWN:
            this->items->scrollDown();
            return;
        case SDLK_LEFT:
            buttonI--;
            if (buttonI < 0) buttonI = buttonCount - 1;
            return;
        case SDLK_RIGHT:
            buttonI++;
            if (buttonI >= buttonCount) buttonI = 0;
            return;
        }
    }

    bool start() {
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
            parent->clear();
            parent->draw();
            parent->drawTexture(bg, this->x, this->y);
            // print top
            parent->drawTexture(top, this->x + 20, this->y + 12);
            // print items
            auto visible = items->getVisible();
            for (int i = 0; i < visible.size(); i++)
                visible[i]->draw(parent, this->x + 10, this->y + i * LINE_HEIGHT + 45);
            for (int i = 0; i < buttonCount; i++) {
                parent->drawRect(this->x + i * buttonWidth + X_OFFSET, this->y + buttonsY, buttonWidth, buttonHeight, SDL_Color{0, 255, 100, 0}, i==buttonI);
                parent->drawTexture(buttons[i], this->x + buttonXs[i] + X_OFFSET, this->y + buttonsY);
            }
            parent->flush();
        }
        return loot;
    }
};