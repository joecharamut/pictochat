#include "Graphics.h"
#include "Main.h"
#include "state/StateManager.h"
#include "types/Color.h"

SDL_Window *Graphics::window;
SDL_Renderer *Graphics::renderer;

SDL_Color Graphics::clearColor = COLOR(0x25, 0x25, 0x25);

bool Graphics::init() {
    printf("creating window\n");
    window = SDL_CreateWindow("hewwo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            Main::SCREEN_WIDTH, Main::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("error creating window: %s\n", SDL_GetError());
        return false;
    }

    printf("creating renderer\n");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("error creating renderer: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void Graphics::unload() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Graphics::drawTexture(SDL_Texture *texture, SDL_Rect *srcrect, SDL_Rect *destrect) {
    SDL_RenderCopy(renderer, texture, srcrect, destrect);
}

std::shared_ptr<Texture> Graphics::createTexture(SDL_Surface *surface) {
//    printf("creating texture\n");
    return std::make_shared<Texture>(SDL_CreateTextureFromSurface(renderer, surface));
}

void Graphics::update() {
    // clear it
    SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(renderer);

    // draw state gfx
    StateManager::update();

    // flip to display
    SDL_RenderPresent(renderer);
}

void Graphics::setClearColor(SDL_Color color) {
    clearColor = color;
}
