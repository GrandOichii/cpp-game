#pragma once

#include "assets.hpp"
#include "context.hpp"

class Context1 : public Context {
private:
    std::function<void(void)> onClose;
    AssetsManager *assets;
    SDL_Texture *tex;
public:
    Context1(Window *parent, AssetsManager *assets, std::function<void(void)> onClose) : Context(parent), assets(assets), onClose(onClose) {
        this->tex = assets->getMessage("sussy baki (press enter)");
    }

    ~Context1() {
        SDL_DestroyTexture(tex);
    }

    void draw() {
        this->parent->drawTexture(this->tex, 10, 10);
    }

    void handleKey(int key) {
        if (key == 'c') this->onClose();
    }
};