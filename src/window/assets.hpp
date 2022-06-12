#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <vector>

#include "font.hpp"
#include "../util.hpp"
#include "../game/core.hpp"

class AssetsManager {
private:
    int fontSize;
    SDL_Renderer *ren = nullptr;
    SDL_Texture *player = nullptr;
    SDL_Texture *mbBackground = nullptr;
    SDL_Texture* focusedTile = nullptr;
    SDL_Texture* logBG = nullptr;
    SDL_Texture* inventoryBG = nullptr;
    SDL_Texture* infoBG = nullptr;
    SDL_Texture* descriptionBG = nullptr;
    SDL_Texture* newItemTex = nullptr;
    Font *font;
    std::map<std::string, SDL_Texture*> tileMap;
    std::map<std::string, SDL_Texture*> itemTextures; // first - picture
public:
    AssetsManager(const std::string assetsPath, int fontSize, SDL_Renderer *ren, game::Game* game) : ren(ren), fontSize(fontSize) {
        auto j = fs::readJS(fs::join(vector<string>{assetsPath, "imagesmap.json"}));
        auto tiles = j["tiles"];
        for (const auto& item : tiles.items()) {
            auto key = item.key();
            auto p = item.value();
            tileMap[key] = this->loadImage(fs::join(assetsPath, p).c_str());
        }
        this->player = this->loadImage(fs::join(assetsPath, j["player"]).c_str());
        this->focusedTile = this->loadImage(fs::join(assetsPath, j["focused_tile"]).c_str());
        this->mbBackground = this->loadImage(fs::join(assetsPath, j["message_box"]).c_str());
        this->logBG = this->loadImage(fs::join(assetsPath, j["logBG"]).c_str());
        this->inventoryBG = this->loadImage(fs::join(assetsPath, j["inventoryBG"]).c_str());
        this->infoBG = this->loadImage(fs::join(assetsPath, j["infoBG"]).c_str());
        this->descriptionBG = this->loadImage(fs::join(assetsPath, j["descriptionBG"]).c_str());
        this->font = new Font(fs::join(assetsPath, j["font"]).c_str(), fontSize, this->ren);
        this->newItemTex = font->get("[NEW]", SDL_Color{0, 255, 0, 0});;

        auto itemJ = j["items"];
        auto missing = this->loadImage(fs::join(assetsPath, itemJ["_missing"]).c_str());
        auto im = game->getItemManager();
        auto items = im->getAllItems();
        int count = im->getItemCount();
        for (int i = 0; i < count; i++) {
            auto image = missing;
            auto name = items[i]->getName();
            if (itemJ.contains(name)) image = this->loadImage(fs::join(assetsPath, itemJ[name]).c_str());
            itemTextures[name] = image;
        }
    }
        
    ~AssetsManager() {
        SDL_DestroyTexture(player);
        SDL_DestroyTexture(focusedTile);
        SDL_DestroyTexture(logBG);
        SDL_DestroyTexture(mbBackground);
        SDL_DestroyTexture(inventoryBG);
        SDL_DestroyTexture(descriptionBG);
        SDL_DestroyTexture(infoBG);
        SDL_DestroyTexture(newItemTex);
        for (auto it = tileMap.begin(); it != tileMap.end(); it++)
            SDL_DestroyTexture(it->second);        
        for (auto it = itemTextures.begin(); it != itemTextures.end(); it++)
            SDL_DestroyTexture(it->second);
        delete font;
    }

    SDL_Texture* getDescriptionBG() {
        return descriptionBG;
    }

    SDL_Texture* getInfoBG() {
        return infoBG;
    }

    SDL_Texture* getNewItemTex() {
        return this->newItemTex;
    }

    SDL_Texture* getInventoryBG() {
        return this->inventoryBG;
    }

    SDL_Texture* getItemTexture(std::string name) {
        auto it = itemTextures.find(name);
        if (it == itemTextures.end()) throw std::runtime_error("to textures for item " + name);
        return it->second;
    }

    int getFontSize() {
        return this->fontSize;
    }

    SDL_Texture* getLogBackground() {
        return this->logBG;
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