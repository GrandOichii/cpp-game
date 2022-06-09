#pragma once

#include <SDL.h>
#include <string>

#include "assets.hpp"
#include "window.hpp"

SDL_Point getSize(SDL_Texture *texture);

std::string showMessageBox(Window* window, AssetsManager* assets, std::string text, std::string* choices, int choisesC);