#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>

#include "font.hpp"
#include "../util.hpp"

class AssetsManager {
private:
    int fontSize;
    SDL_Renderer *ren = nullptr;
    SDL_Texture *player = nullptr;
    SDL_Texture *mbBackground = nullptr;
    SDL_Texture* focusedTile;
    Font *font;
    std::map<std::string, SDL_Texture*> tileMap;
public:
    AssetsManager(const std::string assetsPath, int fontSize, SDL_Renderer *ren) : ren(ren), fontSize(fontSize) {
        auto j = fs::readJS(fs::join(vector<string>{assetsPath, "imagesmap.json"}));
        auto tiles = j["tiles"];
        for (const auto& item : tiles.items()) {
            auto key = item.key();
            auto p = item.value();
            tileMap[key] = this->loadTile(fs::join(assetsPath, p).c_str());
        }
        this->player = this->loadTile(fs::join(assetsPath, j["player"]).c_str());
        this->focusedTile = this->loadTile(fs::join(assetsPath, j["focused_tile"]).c_str());
        this->mbBackground = this->loadTile(fs::join(assetsPath, j["message_box"]).c_str());
        this->font = new Font(fs::join(assetsPath, j["font"]).c_str(), fontSize, this->ren);
    }

    ~AssetsManager() {
        SDL_DestroyTexture(player);
        SDL_DestroyTexture(focusedTile);
        SDL_DestroyTexture(mbBackground);
        for (auto it = tileMap.begin(); it != tileMap.end(); it++)
            SDL_DestroyTexture(it->second);
        delete font;
    }

    int getFontSize() {
        return this->fontSize;
    }

    SDL_Texture *loadTile(const char* path) {
        // std::cout << "Loading " << path << std::endl << ren << std::endl;
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

    SDL_Texture *getFocusedTile() {
        return this->focusedTile;
    }

    SDL_Texture *getMessageBoxBG() {
        return this->mbBackground;
    }

    SDL_Texture *getMessage(std::string message) {
        return this->font->get(message);
    }
    
    SDL_Texture *getMessage(std::string message, SDL_Color color) {
        return this->font->get(message, color);
    }
};