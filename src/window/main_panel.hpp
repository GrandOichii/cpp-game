#pragma once

#include "../game/core.hpp"
#include "context.hpp"
// #include "message_box.hpp"

class Wrapper;

class InteractContext : public Context {
private:
    std::function<void(void)> preDraw;
    std::function<void(Double, bool)> onClose;
    AssetsManager *assets;
    SDL_Texture *tex;
    int x;

public:
    InteractContext(Window *parent, AssetsManager *assets, std::function<void(void)> preDraw, std::function<void(Double, bool)> onClose);
    ~InteractContext();
    void draw();
    void handleKey(int key);
};

class MainPanel : public Context, public game::GameWrapper {
private:
    AssetsManager *assets;
    game::Game *game;
    bool updateGame;
    InteractContext* iContext;
    bool focusedTiles[3][3]{};
public:
    MainPanel(Wrapper *parent);
    ~MainPanel();
    void clearFocused();
    void draw();
    void drawTiles();
    void drawPlayer();
    void handleKey(int key);
    void interactMode();
    void inventoryMode();
    void spellCastMode();
    void consoleCommandMode();
    void updateLog(string message);
    void sleep(int amount);
    string requestChoice(string text, string choices);
};