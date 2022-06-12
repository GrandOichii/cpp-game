#pragma once

#include <SDL.h>
#include <vector>

#include "../window.hpp"
#include "../assets.hpp"
#include "../util.hpp"
#include "../../game/util.hpp"
#include "../../game/core.hpp"
#include "../../util.hpp"

constexpr int OFFSET = 20;
constexpr int MAX_WIDTH = 42;

class DescriptionWindow {
private:
    Window* parent;
    game::Game* game;
    game::IDescribable* item;
    SDL_Texture* bg;
    int y;
    int x;
    int amount;
    bool running;
    std::vector<SDL_Texture*> lines;
    int fs;
public:
    DescriptionWindow(Window* parent, AssetsManager* assets, game::Game* game, game::IDescribable* item, int amount) : 
        parent(parent), 
        game(game), 
        item(item), 
        amount(amount) 
    {
        this->bg = assets->getDescriptionBG();
        auto bgSize = getSize(bg);
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
    }

    ~DescriptionWindow() {
        for (const auto& e : lines)
            SDL_DestroyTexture(e);
    }

    void handleKey(int key) {
        if (key == SDLK_ESCAPE) {
            this->running = false;
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
            // draw
            // parent->clear();
            // parent->draw();
            parent->drawTexture(bg, x, y);
            for (int i = 0; i < lines.size(); i++) {
                if (lines[i] == nullptr) continue;
                parent->drawTexture(lines[i], OFFSET + x, OFFSET + y + i * fs);
            }
            parent->flush();            
        }
    }
};