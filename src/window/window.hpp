#pragma once

#include <map>
#include <SDL.h>

// #include "context.hpp"

class Context;

class Window {
protected:
    Context *currentContext;
    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;
    bool fullscreen;
    bool running;
    const char* title;
    SDL_Event* event = new SDL_Event();
    int height;
    int width;

    void handleKey(int key);
    virtual void setup() = 0;

public:
    SDL_Event* getEvent();
    void clear();
    void flush();
    void draw();
    int getHeight();
    int getWidth();
    Window(int width, int height);
    ~Window();
    void start();
    void drawTexture(SDL_Texture *texture, int x, int y);
    void drawTextureMiddle(SDL_Texture *texture);
    void setCurrentContext(Context *context);
    void close();
    SDL_Renderer *getRenderer() const;
    void toggleFullscreen();
};