#include "Graphics.h"
#include "Main.h"
#include "state/StateManager.h"

SDL_Window *Graphics::window;
SDL_Renderer *Graphics::renderer;

bool Graphics::initGraphics() {
    printf("creating window\n");
    window = SDL_CreateWindow("hello", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            Main::SCREEN_WIDTH, Main::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("error creating window: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    return true;
}

void Graphics::drawTexture(SDL_Texture *texture, SDL_Rect *srcrect, SDL_Rect *destrect) {
    SDL_RenderCopy(renderer, texture, srcrect, destrect);
}

void Graphics::update() {
    StateManager::update();

    SDL_RenderPresent(renderer);
}

std::shared_ptr<Texture> Graphics::createTexture(SDL_Surface *surface) {
    return std::make_shared<Texture>(SDL_CreateTextureFromSurface(renderer, surface));
}
