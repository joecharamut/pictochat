#include "Graphics.h"
#include "Main.h"
#include "state/StateManager.h"
#include "types/Color.h"
#include <cmath>

SDL_Window *Graphics::window;
SDL_Renderer *Graphics::renderer;

SDL_Color Graphics::clearColor = SDL_Color {0x00, 0x00, 0x00, 0x00};

std::map<Graphics::CursorType, SDL_Cursor *> Graphics::cursors;

SDL_Texture *Graphics::onePixel;

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

    cursors[Default] = SDL_GetDefaultCursor();
    cursors[Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    cursors[Crosshair] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);

    onePixel = createTexture(1, 1);
    return true;
}

void Graphics::unload() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    for (const auto &pair : cursors) {
        SDL_FreeCursor(pair.second);
    }
    SDL_DestroyTexture(onePixel);
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

#define RENDERTEX_BEGIN(tex) SDL_SetRenderTarget(renderer, tex); unsigned char _r, _g, _b, _a; \
    SDL_GetRenderDrawColor(renderer, &_r, &_g, &_b, &_a)

#define RENDERTEX_END() SDL_SetRenderDrawColor(renderer, _r, _g, _b, _a); SDL_SetRenderTarget(renderer, nullptr)

void Graphics::textureDrawLine(SDL_Texture *tex, SDL_Color color, int x1, int y1, int x2, int y2, int thickness) {
    RENDERTEX_BEGIN(tex);

    SDL_SetRenderTarget(renderer, onePixel);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, tex);

    int length = (int) std::round( std::sqrt( std::pow((x2 - x1), 2) + std::pow((y2 - y1), 2) ) );
    SDL_Rect rect {x1, y1, length, thickness};

    double slope = std::atan( (double) (y2 - y1) / (double) (x2 - x1) ) * (180 / pi());
    if (std::isnan(slope)) {
        slope = 0;
    }

    SDL_RenderCopyEx(renderer, onePixel, nullptr, &rect, slope, nullptr, SDL_FLIP_NONE);

    RENDERTEX_END();
}

void Graphics::textureDrawPoint(SDL_Texture *tex, SDL_Color color, int x, int y, int thickness) {
    RENDERTEX_BEGIN(tex);

    SDL_SetRenderTarget(renderer, onePixel);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, tex);

    SDL_Rect rect {x + (thickness / 2), y + (thickness / 2), thickness, thickness};
    SDL_RenderCopy(renderer, onePixel, nullptr, &rect);

    RENDERTEX_END();
}

std::shared_ptr<unsigned char> Graphics::getTexturePixels(SDL_Texture *tex) {
    RENDERTEX_BEGIN(tex);

    int w, h;
    SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);

    int pitch = w * 4;
    int bytes = w * 4 * h;

    auto *mem = new unsigned char[bytes];
    if (SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA32, mem, pitch) < 0) {
        printf("SDL_RenderReadPixels() error: %s\n", SDL_GetError());
        exit(1);
    }

    RENDERTEX_END();
    return std::shared_ptr<unsigned char>(mem);
}

SDL_Texture *Graphics::createTexture(int w, int h) {
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
    RENDERTEX_BEGIN(tex);

    SDL_RenderClear(renderer);

    RENDERTEX_END();
}

void Graphics::setCurosr(Graphics::CursorType type) {
    SDL_SetCursor(cursors[type]);
}
