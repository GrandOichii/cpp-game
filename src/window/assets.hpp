#pragma once

#include <SDL.h>
#include <map>

#include "../util.hpp"

class AssetsManager {
private:
    SDL_Renderer *ren = nullptr;
    SDL_Texture *player = nullptr;
    std::map<std::string, SDL_Texture*> tileMap;
public:
    AssetsManager(const std::string assetsPath, SDL_Renderer *ren) : ren(ren) {
        auto j = fs::readJS(fs::join(vector<string>{assetsPath, "imagesmap.json"}));
        auto tiles = j["tiles"];
        for (const auto& item : tiles.items()) {
            auto key = item.key();
            auto p = item.value();
            tileMap[key] = this->loadImage(fs::join(assetsPath, p).c_str());
        }
        this->player = this->loadImage(fs::join(assetsPath, j["player"]).c_str());
    }

    ~AssetsManager() {
        SDL_DestroyTexture(player);
        for (auto it = tileMap.begin(); it != tileMap.end(); it++)
            SDL_DestroyTexture(it->second);
    }

    SDL_Texture *loadImage(const char* path) {
        SDL_Surface *bmp = SDL_LoadBMP(path);
        if (bmp == nullptr){
            std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
            throw std::runtime_error("");
        }
        SDL_Texture *result = SDL_CreateTextureFromSurface(ren, bmp);
        SDL_FreeSurface(bmp);
        if (result == nullptr){
            std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
            throw std::runtime_error("");
        }
        return result;
    }

    SDL_Texture * getPlayer() { 
        return this->player;
    }

    SDL_Texture *getTile(std::string name) {
        auto it = this->tileMap.find(name);
        if (it == tileMap.end()) throw std::runtime_error("tile " + name + " no found in assets");
        return it->second;
    }
};