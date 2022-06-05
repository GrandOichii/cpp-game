#pragma once

#include "../game/core.hpp"
#include "color.hpp"

using std::string;

class Wrapper {
private:
    game::Game* game;
public:
    Wrapper(string gamePath) {
        this->game = new game::Game("demo");
    }

    void start() {
        this->game->print();
        nc::CCTMessage cct("${red}Hello, world ${green-blue}lmao");
    }

    ~Wrapper() {
        delete game;
    }
};