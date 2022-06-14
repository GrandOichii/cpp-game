#pragma once

#include <SDL.h>

#include "assets.hpp"
#include "window.hpp"
#include "../util.hpp"
#include "util.hpp"
#include "../game/scripting/overseer.hpp"
#include "../game/scripting/command.hpp"

constexpr SDL_Color COMMAND_COLOR {255, 100, 255, 0};
constexpr SDL_Color PREV_COMMAND_COLOR {100, 100, 100, 0};
constexpr SDL_Color PREV_ERROR_COLOR {250, 100, 100, 0};

class CommandLine {
private:
    Window* parent;
    AssetsManager* assets;
    bool running;
    bool update;
    LineEdit* line = nullptr;
    int y, x;
    int fs;
    CircularBuffer<std::pair<std::string, bool>>* last;
    game::scripting::ScriptOverseer* so;
public:
    CommandLine(Window* parent, AssetsManager* assets, game::scripting::ScriptOverseer* so, int maxWidth, int x, int y) : parent(parent), assets(assets), so(so), x(x), y(y) {
        this->line = new LineEdit("", maxWidth);
        this->last = new CircularBuffer<std::pair<std::string, bool>>(10);
        fs = assets->getFontSize();
    }

    ~CommandLine() {
        delete line;
    }

    void handleKey(int key, bool shift) {
        update = true;
        switch (key) {
        case SDLK_ESCAPE:
            running = false;
            return;
        case SDLK_RETURN:
            this->executeCurrent();
            return;
        case SDLK_BACKSPACE:
            line->eraseAtCursor();
            return;
        case SDLK_LEFT:
            line->cursorLeft();
            return;
        case SDLK_RIGHT:
            line->cursorRight();
            return;
        // TODO: fix this
        case '\'':
            if (!shift) break;
            key = '\"';
            shift = false;
            break;
        case ';':
            if (!shift) break;
            key = ':';
            shift = false;
            break;
        case '-':
            if (!shift) break;
            key = '_';
            shift = false;
            break;
        }
        update = line->addCharacter(key - shift*32);
    }

    void executeCurrent() {
        auto text = line->getText();
        // try to execute
        game::scripting::Command* c = nullptr;
        std::string message = text;
        bool isError = false;
        try { 
            c = new game::scripting::Command(text, so);
            c->exec();
        } catch (const std::exception& ex) {
            message = ex.what();
            isError = true;
        }
        delete c;
        last->add(std::make_pair(message, isError));
        this->line->clear();
    }

    void draw() {
        if (!update) return;
        parent->clear();
        parent->draw();
        auto text = line->getText();
        int cursorPos = 0;
        if (text.size()) {
            auto tex = assets->getMessage(text, COMMAND_COLOR);
            parent->drawTexture(tex, x, y);
            SDL_DestroyTexture(tex);
            auto subtext = text.substr(0, line->getCursor());
            if (subtext.size()) {
                auto subtex = assets->getMessage(subtext);
                cursorPos = getSize(subtex).x;
                SDL_DestroyTexture(subtex);
            }
        }
        parent->drawLine(this->x + cursorPos, this->y, this->x+cursorPos, this->y + fs, SDL_Color{255, 255, 255, 0});
        int i = -1;
        auto vec = last->getV();
        auto size = vec.size();
        auto y = this->y - size*fs;
        for (const auto& e : vec) {
            ++i;
            auto me = e.first;
            if (me.size() == 0) continue;
            auto tex = assets->getMessage(me, e.second ? PREV_ERROR_COLOR : PREV_COMMAND_COLOR);
            parent->drawTexture(tex, this->x, y + i * fs);
            SDL_DestroyTexture(tex);
        }
        parent->flush();
    }

    void show() {
        running = true;
        update = true;
        auto event = parent->getEvent();
        while (running) {
            // handle input
            while (SDL_PollEvent(event)) {
                switch (event->type) {
                case SDL_QUIT:
                    parent->close();
                    running = false;
                case SDL_KEYDOWN:
                    auto key = event->key.keysym.sym;
                    this->handleKey(key, event->key.keysym.mod);
                }
            }
            // draw
            this->draw();
        }
    }
};