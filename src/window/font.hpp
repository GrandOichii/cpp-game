#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

class Font {
private:
    TTF_Font *font;
    SDL_Renderer *ren;
public:
    Font(const char* path, int fontSize, SDL_Renderer* ren) : ren(ren) {
        if (TTF_Init() != 0){
            std::cout << "TTF_init Error: " << SDL_GetError() << std::endl;
            throw std::runtime_error("failed initializing font");
        }
        this->font = TTF_OpenFont(path, fontSize);
        if (font == nullptr){
            std::cout << "TTF_OpenFont Error: " << SDL_GetError() << std::endl;
            throw std::runtime_error("failed opening font file " + std::string(path));
        }       
    }

    ~Font() {
        TTF_CloseFont(font);
    }

    SDL_Texture * get(std::string message, SDL_Color color) {
        SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
        if (surf == nullptr){
            TTF_CloseFont(font);
            std::cout << "TTF_RenderText Error: " << SDL_GetError() << std::endl;
            throw std::runtime_error("failed rendering message " + message);
        }
        SDL_Texture *result = SDL_CreateTextureFromSurface(this->ren, surf);
        if (result == nullptr){
            std::cout << "CreateTexture Error: " << SDL_GetError() << std::endl;

            throw std::runtime_error("failed creating texture of message " + message);
        }
        SDL_FreeSurface(surf);
        return result;
    }

    SDL_Texture * get(std::string message) {
        return this->get(message, SDL_Color{255, 255, 255, 255});
    }
};