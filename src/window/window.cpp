#include "window.hpp"
#include "util.hpp"
#include "context.hpp"

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
    // std::cout << "Drawing texture at " << y << " " << x << std::endl;
}

void Window::drawTextureMiddle(SDL_Texture *texture) {
    auto size = getSize(texture);
    auto x = (this->width - size.x) / 2;
    auto y = (this->height - size.y) / 2;
    this->drawTexture(texture, x, y);
}

void Window::drawRect(int x, int y, int width, int height, SDL_Color color, bool fill) {
    SDL_SetRenderDrawColor( ren, color.r, color.g, color.b, 0 ); // last color.a?
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = width;
    r.h = height;
    if (fill)
        SDL_RenderFillRect(this->ren, &r);
    else SDL_RenderDrawRect(this->ren, &r);
    SDL_SetRenderDrawColor(this->ren, 0, 0, 0, 0 );
}

void Window::drawLine(int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_SetRenderDrawColor( ren, color.r, color.g, color.b, 0 ); // last color.a?
    SDL_RenderDrawLine(ren, x1, y1, x2, y2);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0 );
}

void Window::handleKey(int key) {
    this->currentContext->handleKey(key);
}

void Window::clear() {
    // SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    // SDL_RenderClear(ren);
    // SDL_RenderClear(ren);
}

void Window::flush() {
    SDL_RenderPresent(ren);
}

void Window::draw() {
    this->currentContext->draw();
}

void Window::toggleFullscreen() {
    this->fullscreen = !this->fullscreen;
    SDL_SetWindowFullscreen(win, this->fullscreen);
}

SDL_Window* Window::getWin() {
    return win;
}

std::pair<int, int> Window::getWindowSize() {
    int x, y;
    SDL_GetWindowSize(win, &x, &y);
    return std::make_pair(x, y);
}

void Window::start() {
    // set up the window
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_StartTextInput(); 
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
    while (this->running) {
        // handle events
        while (SDL_PollEvent(this->event)) {
            switch (event->type) {
            case SDL_QUIT:
                this->running = false;
                break;
            case SDL_KEYDOWN:
                this->handleKey(event->key.keysym.sym);
            }
        }
        this->clear();
        this->draw();
        this->flush();
    }
}

SDL_Event* Window::getEvent() {
    return this->event;
}

void Window::close() {
    this->running = false;
}
