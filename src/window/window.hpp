#pragma once

#include <map>
#include <SDL.h>

#include "context.hpp"

constexpr int TILE_WIDTH = 32;
constexpr int TILE_HEIGHT = 32;
constexpr int TILE_COUNT_Y = 15;
constexpr int TILE_COUNT_X = 21;
constexpr int CENTER_Y = TILE_COUNT_Y / 2 * TILE_HEIGHT;
constexpr int CENTER_X = TILE_COUNT_X / 2 * TILE_HEIGHT;
constexpr int WINDOW_HEIGHT = TILE_HEIGHT * TILE_COUNT_Y;
constexpr int WINDOW_WIDTH = TILE_WIDTH * TILE_COUNT_X;

class Window {
protected:
    Context *currentContext;
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;
    bool fullscreen;
    bool running;
    const char* title;

    void handleKey(int key);
    virtual void setup() = 0;
    void draw();

protected:

public:
    Window();

    ~Window();

    void start();

    void drawImage(SDL_Texture *texture, int x, int y);

    void setCurrentContext(Context *context);

    void close();

    SDL_Renderer *getRenderer() const;

    void toggleFullscreen();

};