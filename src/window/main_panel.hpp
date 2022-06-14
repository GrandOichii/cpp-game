#pragma once

#include "../game/core.hpp"
#include "../util.hpp"
#include "command_line.hpp"
#include "context.hpp"

class Wrapper;

class MainPanel : public Context, public game::GameWrapper {
private:
    AssetsManager *assets;
    game::Game *game;
    bool updateGame;
    bool focusedTiles[3][3]{};
    CircularBuffer<std::string>* logs;
    int tileCountX;
    int centerX;
    int logWidth;
    CommandLine* commandLine;
    vector<SDL_Texture*> apparel;
public:
    MainPanel(Wrapper *parent);
    ~MainPanel();
    void clearFocused();
    void draw();
    void drawTiles();
    void drawLog();
    void drawPlayer();
    void drawInfo();
    void handleKey(int key);
    void toggleFullscreen();
    void interactMode();
    void inventoryMode();
    void spellCastMode();
    void consoleCommandMode();
    void updateLog(string message);
    void sleep(int amount);
    string requestChoice(string text, string choices);
    bool accessContainer(game::items::Container* container, std::string top);
    void updatePlayerLook();
};