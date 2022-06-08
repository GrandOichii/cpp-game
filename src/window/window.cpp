#include "window.hpp"

// #include "../util.hpp"

int Window::getHeight() {
    return this->height;
}

int Window::getWidth() {
    return this->width;
}

void Window::setCurrentContext(Context *context) {
    this->currentContext = context;
}

SDL_Renderer* Window::getRenderer() const {
    return this->ren;
}

Window::Window(int width, int height) : width(width), height(height) {
    
}

Window::~Window() {
    delete currentContext;
}

void Window::drawTexture(SDL_Texture *texture, int x, int y) {
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
    SDL_RenderCopy(ren, texture, NULL, &pos);
}

void Window::handleKey(int key) {
    this->currentContext->handleKey(key);
}

void Window::draw() {
    SDL_RenderClear(ren);
    this->currentContext->draw();
    SDL_RenderPresent(ren);
}

void Window::toggleFullscreen() {
    this->fullscreen = !this->fullscreen;
    SDL_SetWindowFullscreen(win, this->fullscreen);
}

void Window::start() {
    // set up the window
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }
    this->win = SDL_CreateWindow(this->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, SDL_WINDOW_SHOWN);
    if (this->win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return;
    }
    this->ren = SDL_CreateRenderer(this->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (this->ren == nullptr){
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return;
    }
    this->setup();
    this->running = true;
    SDL_Event event;
    while (this->running) {
        // handle events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                this->running = false;
                break;
            case SDL_KEYDOWN:
                this->handleKey(event.key.keysym.sym);
            }
        }
        this->draw();
    }
}

void Window::close() {
    this->running = false;
}
