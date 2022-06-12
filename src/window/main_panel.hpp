#pragma once

#include "../game/core.hpp"
#include "../util.hpp"
#include "context.hpp"

class Wrapper;

class MainPanel : public Context, public game::GameWrapper {
private:
    AssetsManager *assets;
    game::Game *game;
    bool updateGame;
    // InteractContext* iContext;
    bool focusedTiles[3][3]{};
    CircularBuffer<std::string>* logs;
public:
    MainPanel(Wrapper *parent);
    ~MainPanel();
    void clearFocused();
    void draw();
    void drawTiles();
    void drawLog();
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