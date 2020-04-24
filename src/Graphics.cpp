#include "Graphics.h"
#include "Main.h"
#include "state/StateManager.h"
#include "types/Color.h"

SDL_Window *Graphics::window;
SDL_Renderer *Graphics::renderer;

SDL_Color Graphics::clearColor = SDL_Color {0x00, 0x00, 0x00, 0x00};


bool Graphics::init() {
    printf("creating window\n");
    window = SDL_CreateWindow("pictochat", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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

void Graphics::setWindowSize(int w, int h) {
    SDL_SetWindowSize(window, w, h);
}

void Graphics::textureDrawLine(SDL_Texture *tex, int x1, int y1, int x2, int y2) {
    SDL_SetRenderTarget(renderer, tex);
    unsigned char r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);


    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);


    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_SetRenderTarget(renderer, nullptr);
}

void Graphics::textureDrawPoint(SDL_Texture *tex, int x, int y) {
    SDL_SetRenderTarget(renderer, tex);
    unsigned char r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);


    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderDrawPoint(renderer, x, y);


    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_SetRenderTarget(renderer, nullptr);
}

std::shared_ptr<unsigned char> Graphics::getTexturePixels(SDL_Texture *tex, int w, int h) {
    SDL_SetRenderTarget(renderer, tex);

    int pitch = w * 4;
    void *ptr = nullptr;
    SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA32, ptr, pitch);
    auto *pixels = (unsigned char *) ptr;

    int bytes = w * 4 * h;

    auto *mem = new unsigned char[bytes];
    std::copy(pixels, pixels + bytes, mem);
    std::shared_ptr<unsigned char> pixelCopy(mem);

    SDL_SetRenderTarget(renderer, nullptr);
    return pixelCopy;
}

SDL_Texture *Graphics::createTexture(int w, int h) {
    printf("%d %d\n", w, h);
    SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);

    if (!tex) {
        printf("createTexture() error: %s\n", SDL_GetError());
        exit(1);
    }

    if (SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND) < 0) {
        printf("createTexture() error: %s\n", SDL_GetError());
        exit(1);
    }
    return tex;
}

void Graphics::clearTexture(SDL_Texture *tex) {
    SDL_SetRenderTarget(renderer, tex);

    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, nullptr);
}




