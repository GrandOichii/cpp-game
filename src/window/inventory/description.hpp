#pragma once

#include <SDL.h>
#include <vector>
#include <map>

#include "../window.hpp"
#include "../assets.hpp"
#include "../util.hpp"
#include "../../game/util.hpp"
#include "../../game/core.hpp"
#include "../../util.hpp"
#include "../../game/items/item.hpp"
#include "../../game/types.hpp"

constexpr int OFFSET = 20;
constexpr int MAX_WIDTH = 30;
constexpr int BUTTON_WIDTH = 100;
constexpr int BUTTON_HEIGHT = 40;
constexpr int SLOT_BUTTON_HEIGHT = 40;

class Button {
private:
    SDL_Texture* label;
    std::function<void(void)> f;
    int x;
    int y;
public:
    Button(SDL_Texture* label, std::function<void(void)> f) : label(label), f(f) {
        auto size = getSize(label);
        this->x = (BUTTON_WIDTH - size.x) / 2;
        this->y = (BUTTON_HEIGHT - size.y) / 2;
    }

    void draw(Window* parent, int x, int y, bool focused) {
        parent->drawRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, SDL_Color{40, 40, 40, 0}, focused);
        parent->drawTexture(label, x + this->x, y + this->y);
    }

    void press() {
        f();
    }

    ~Button() {
        SDL_DestroyTexture(label);
    }
};

class DescriptionWindow {
private:
    Window* parent;
    AssetsManager* assets;
    game::Game* game;
    game::IDescribable* item;
    SDL_Texture* bg;
    int y;
    int x;
    int amount;
    bool running;
    std::vector<SDL_Texture*> lines;
    int fs;
    std::vector<Button*> buttons;
    int buttonsX;
    int buttonI = 0;
public:
    DescriptionWindow(Window* parent, AssetsManager* assets, game::Game* game, game::IDescribable* item, int amount) : 
        parent(parent), 
        assets(assets),
        game(game), 
        item(item), 
        amount(amount) 
    {
        this->bg = assets->getDescriptionBG();
        auto bgSize = getSize(bg);
        buttonsX = bgSize.x - BUTTON_WIDTH - 20;
        this->x = (parent->getWidth() - bgSize.x) / 2;
        this->y = (parent->getHeight() - bgSize.y) / 2;
        auto lines = str::split(item->getBigDescription(amount), "\n");

        for (const auto& line : lines) {
            if (line == "") {
                this->lines.push_back(nullptr);
                continue;
            };
            auto split = str::widthSplit(line, MAX_WIDTH);
            for (const auto& l : split)
                this->lines.push_back(assets->getMessage(l));
        }
        this->fs = assets->getFontSize();

        auto op = item->getOperations();
        std::vector<std::pair<int, std::pair<std::function<void(void)>, std::string>>> opMap = {
            std::make_pair(game::USE_OPERATION, std::make_pair([this, item, game]() {
                auto uItem = (game::items::UsableItem*)item;
                uItem->use(game);
                this->running = false;
                // TODO maybe fix
                // push event to close menu
                SDL_Event quitEvent;
                quitEvent.type = SDL_KEYDOWN;
                quitEvent.key.keysym.sym = SDLK_ESCAPE;
                SDL_PushEvent(&quitEvent);
            }, "Use")),
            std::make_pair(game::EQUIP_OPERATION, std::make_pair([this]() {
                this->equipAction();
            }, "Equip")),
            std::make_pair(game::CLOSE_OPERATION, std::make_pair([this]() {
                this->running = false;
            }, "Close")),
        };
        for (auto it = opMap.begin(); it != opMap.end(); it++) {
            if (op & it->first) {
                auto label = assets->getMessage(it->second.second);
                buttons.push_back(new Button(label, it->second.first));
            }
        }
    }

    ~DescriptionWindow() {
        for (const auto& e : lines)
            SDL_DestroyTexture(e);
        for (const auto& e : buttons)
            delete e;
    }

    void equipAction() {
        auto item = (game::items::EquipableItem*)this->item;
        auto player = game->getPlayer();
        auto lines = player->getEquipLines(item->getSlot());
        auto count = lines.size();
        std::vector<int> xs;
        std::vector<int> ys;
        SDL_Texture** buttons = new SDL_Texture*[count];
        SDL_Texture** highButtons = new SDL_Texture*[count];
        auto destroyTextures = [count, buttons, highButtons](){
            for (int i = 0; i < count; i++)
                SDL_DestroyTexture(buttons[i]);
                delete[] buttons;
            for (int i = 0; i < count; i++)
                SDL_DestroyTexture(highButtons[i]);
                delete[] highButtons;
        };

        int buttonWidth = 0;
        for (int i = 0; i < count; i++) {
            auto line = lines[i];
            auto l = toString(line.first);
            if (line.second) l += " (" + line.second->getDisplayName() + ")";
            buttons[i] = assets->getMessage(l);
            highButtons[i] = assets->getMessage(l, SDL_Color{0, 255, 0, 0});
            auto size = getSize(buttons[i]);
            if (buttonWidth < size.x) buttonWidth = size.x;
        }
        int xOffset = 10;
        buttonWidth += 2 * xOffset;
        for (int i = 0; i < count; i++) {
            auto size = getSize(buttons[i]);
            auto x = (buttonWidth - size.x) / 2;
            auto y = (BUTTON_HEIGHT - size.y) / 2;
            xs.push_back(x);
            ys.push_back(y);
        }
        auto size = getSize(bg);
        auto buttonsY = (size.y - count * SLOT_BUTTON_HEIGHT) / 2;
        int buttonsX = (size.x - buttonWidth) / 2;
        bool running = true;
        int buttonI = 0;
        auto event = parent->getEvent();
        while (running) {
            // handle input
            while (SDL_PollEvent(event)) {
                switch (event->type) {
                case SDL_QUIT:
                    parent->close();
                    running = false;
                    this->running = false;
                    destroyTextures();
                    return;
                case SDL_KEYDOWN:
                    auto key = event->key.keysym.sym;
                    switch (key) {
                    case SDLK_ESCAPE:
                        destroyTextures();
                        return;
                    case SDLK_DOWN:
                        buttonI++;
                        if (buttonI >= count) buttonI = 0;
                        break;
                    case SDLK_UP:
                        buttonI--;
                        if (buttonI < 0) buttonI = count - 1;
                        break;
                    case SDLK_RETURN:
                        running = false;
                        break;
                    }
                }
            }
            // draw
            this->draw();
            for (int i = 0; i < count; i++) {
                parent->drawRect(buttonsX + this->x, this->y + i * SLOT_BUTTON_HEIGHT + buttonsY, buttonWidth, SLOT_BUTTON_HEIGHT, SDL_Color{0, 0, 0, 0}, true);
                parent->drawTexture(buttonI == i ? highButtons[i] : buttons[i], buttonsX + this->x + xs[i], buttonsY + this->y + i * BUTTON_HEIGHT + ys[i]);
            }
            parent->flush();            
        }
        auto slot = lines[buttonI].first;
        player->equip(slot, item, game);
        destroyTextures();
    }

    void handleKey(int key) {
        if (key == SDLK_ESCAPE) {
            this->running = false;
            return;
        }
        switch (key) {
        case SDLK_RETURN:
            buttons[buttonI]->press();
            return;
        case SDLK_UP:
            buttonI--;
            if (buttonI < 0) buttonI = buttons.size() - 1;
            return;
        case SDLK_DOWN:
            buttonI++;
            if (buttonI >= buttons.size()) buttonI = 0;
            return;
        }
    }

    void start() {
        auto event = parent->getEvent();
        this->running = true;
        while (running) {
            // handle input
            while (SDL_PollEvent(event)) {
                switch (event->type) {
                case SDL_QUIT:
                    parent->close();
                    running = false;
                    SDL_PushEvent(event);
                    return;
                case SDL_KEYDOWN:
                    auto key = event->key.keysym.sym;
                    this->handleKey(key);
                }
            }
            this->draw();
            // draw
            parent->flush();            
        }
    }

    void draw() {
        parent->drawTexture(bg, x, y);
        for (int i = 0; i < lines.size(); i++) {
            if (lines[i] == nullptr) continue;
            parent->drawTexture(lines[i], OFFSET + x, OFFSET + y + i * fs);
        }
        for (int i = 0; i < buttons.size(); i++)
            buttons[i]->draw(parent, this->x + buttonsX, this->y + i * BUTTON_HEIGHT + 20, i == buttonI);
        
    }
};