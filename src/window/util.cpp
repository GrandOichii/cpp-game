#include "util.hpp"

#include <vector>

#include "assets.hpp"
#include "window.hpp"
#include "../util.hpp"


SDL_Point getSize(SDL_Texture *texture) {
    SDL_Point size;
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    return size;
}

std::string showMessageBox(Window* window, AssetsManager* assets, std::string text, std::vector<std::string> choices) {
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
    auto size = lines.size();
    SDL_Texture** tlines = new SDL_Texture*[size];
    int cancelI = -1;
    for (int i = 0; i < size; i++){
        tlines[i] = assets->getMessage(lines[i]);
        if (lines[i] == "Cancel") cancelI = -1;
    }
    // calculate choices
    auto color1 = SDL_Color{255, 255, 255, 0};
    auto color2 = SDL_Color{0, 255, 0, 0};
    auto choicesC = choices.size();
    SDL_Texture*** buttons = new SDL_Texture**[choicesC];
    for (int i = 0; i < choicesC; i++) {
        buttons[i] = new SDL_Texture*[2];
        buttons[i][0] = assets->getMessage(choices[i], color1);
        buttons[i][1] = assets->getMessage(choices[i], color2);
    }
    auto x = (window->getWidth() - mbSize.x) / 2;
    auto y = (window->getHeight() - mbSize.y) / 2;
    auto fs = assets->getFontSize();
    
    auto event = window->getEvent();
    bool running = true;
    // bool running = false;
    auto buttonsY = y + mbSize.y - 2 * yOffset - fs * choicesC;
    int buttonI = 0;
    while (running) {
        while (SDL_PollEvent(event)) {
            switch (event->type) {
            case SDL_QUIT:
                window->close();
                running = false;
            case SDL_KEYDOWN:
                auto key = event->key.keysym.sym;
                switch(key) {
                case SDLK_ESCAPE:
                    if (cancelI != -1) {
                        buttonI = cancelI;
                        running = false;
                    }
                    break;
                case SDLK_RETURN:
                    running = false;
                    break;
                case SDLK_UP:
                    buttonI--;
                    if (buttonI < 0) buttonI = choicesC - 1;
                    break;
                case SDLK_DOWN:
                    buttonI++;
                    if (buttonI >= choicesC) buttonI = 0;
                    break;
                }
            }
        }
        // draw
        window->clear();
        window->draw();
        window->drawTexture(mbBG, x, y);
        for (int i = 0; i < size; i++) {
            window->drawTexture(tlines[i], x + xOffset, y + yOffset + i * fs);
        }
        for (int i = 0; i < choicesC; i++) {
            int p = 0;
            if (i == buttonI) p = 1;
            // window->drawTexture(assets->getPlayer(), x + xOffset, buttonsY + fs*i);
            window->drawTexture(buttons[i][p], x + xOffset, buttonsY + fs*i);
        }
        window->flush();
    }

    SDL_DestroyTexture(longt);
    for (int i = 0; i < size; i++)
        SDL_DestroyTexture(tlines[i]);
    delete[] tlines;
    for (int i = 0; i < choicesC; i++) {
        SDL_DestroyTexture(buttons[i][0]);
        SDL_DestroyTexture(buttons[i][1]);
        delete[] buttons[i];
    }
    delete[] buttons;
    return choices[buttonI];
}