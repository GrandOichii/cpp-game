#pragma once

#include <SDL.h>

#include "window.hpp"
#include "assets.hpp"

class Window;

class Context {
protected:
    Window *parent;
public:
    Context(Window *parent) : parent(parent) {}

    virtual ~Context() = default;

    virtual void draw() = 0;

    virtual void handleKey(int key) = 0;
};