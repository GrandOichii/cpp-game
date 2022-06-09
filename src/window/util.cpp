#include "util.hpp"

#include "assets.hpp"
#include "window.hpp"
#include "../util.hpp"


SDL_Point getSize(SDL_Texture *texture) {
    SDL_Point size;
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    return size;
}

std::string showMessageBox(Window* window, AssetsManager* assets, std::string text, std::string* choices, int choisesC) {
    const int xOffset = 15;
    const int yOffset = 15;
    window->draw();
    auto mbBG = assets->getMessageBoxBG();
    auto mbSize = getSize(mbBG);
    // calculate lines
    auto longt = assets->getMessage(text);
    auto mwidth = getSize(longt).x;
    int maxWidth = (text.size() * (mbSize.x-xOffset*2)) / mwidth;
    auto lines = str::widthSplit(text, maxWidth);
    SDL_DestroyTexture(longt);
    auto x = (window->getWidth() - mbSize.x) / 2;
    auto y = (window->getHeight() - mbSize.y) / 2;
    window->drawTexture(mbBG, x, y);
    auto fs = assets->getFontSize();
    for (int i = 0; i < lines.size(); i++) {
        auto tex = assets->getMessage(lines[i]);
        window->drawTexture(tex, x + xOffset, y + yOffset + i * fs);
        SDL_DestroyTexture(tex);
    }
    window->flush();
    auto event = window->getEvent();
    bool running = true;
    // bool running = false;
    while (running) {
        while (SDL_PollEvent(event)) {
            switch (event->type) {
            case SDL_QUIT:
                window->close();
                running = false;
            case SDL_KEYDOWN:
                auto key = event->key.keysym.sym;
                if (key == SDLK_ESCAPE) running = false;
            }
        }
    }

    return "";
}