#include "Graphics.h"
#include "Main.h"
#include "state/StateManager.h"

#include <SDL_gpu.h>
#include <iostream>

SDL_Window *Graphics::window;
SDL_Renderer *Graphics::renderer;

int Graphics::scanlineOffset = 0;
SDL_Texture *Graphics::textureTarget;
SDL_Texture *Graphics::scanlineTexture;

bool Graphics::init() {
    std::cout << "sdl_gpu start" << std::endl;
    std::cout << "compiled: " << std::to_string(GPU_GetCompiledVersion().major) << "." << std::to_string(GPU_GetCompiledVersion().minor) << "." << std::to_string(GPU_GetCompiledVersion().patch) << std::endl;
    std::cout << "linked: " << std::to_string(GPU_GetLinkedVersion().major) << "." << std::to_string(GPU_GetLinkedVersion().minor) << "." << std::to_string(GPU_GetLinkedVersion().patch) << std::endl;
    std::cout << "sdl_gpu end" << std::endl;

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

    textureTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,
            Main::SCREEN_WIDTH, Main::SCREEN_HEIGHT);
    if (!textureTarget) {
        printf("error creating render target texture: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface *loadSurface = SDL_LoadBMP("res/scanlines3.bmp");
    if (!loadSurface) {
        printf("error loading scanline texture: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface *scanlineSurface = SDL_CreateRGBSurfaceWithFormat(0, Main::SCREEN_WIDTH,
            Main::SCREEN_HEIGHT * 2, 32, SDL_PIXELFORMAT_RGBA32);
    if (!scanlineSurface) {
        printf("error creating scanline surface: %s\n", SDL_GetError());
        return false;
    }

    for (int x = 0; x < Main::SCREEN_WIDTH; x += loadSurface->w) {
        for (int y = 0; y < Main::SCREEN_HEIGHT * 2; y += loadSurface->h) {
            SDL_Rect rect {x, y, x + loadSurface->w, y + loadSurface->h};
            SDL_BlitSurface(loadSurface, nullptr, scanlineSurface, &rect);
        }
    }

    scanlineTexture = SDL_CreateTextureFromSurface(renderer, scanlineSurface);
    SDL_FreeSurface(loadSurface);
    SDL_FreeSurface(scanlineSurface);

    return true;
}

void Graphics::unload() {
    SDL_DestroyTexture(scanlineTexture);
    SDL_DestroyTexture(textureTarget);
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
    // set target to texture
    SDL_SetRenderTarget(renderer, textureTarget);
    // clear it
    SDL_SetRenderDrawColor(renderer, 0x25, 0x25, 0x25, 0xff);
    SDL_RenderClear(renderer);
    // draw state gfx
    StateManager::update();

    // update scanline positions
    scanlineOffset++;
    if (scanlineOffset > SCANLINE_LIMIT) scanlineOffset = 0;
    SDL_Rect rect {0, (scanlineOffset / 2) - Main::SCREEN_HEIGHT, Main::SCREEN_WIDTH, Main::SCREEN_HEIGHT * 2};
    // draw scanlines
    SDL_RenderCopy(renderer, scanlineTexture, nullptr, &rect);

    // target window
    SDL_SetRenderTarget(renderer, nullptr);
    // clear it
    SDL_RenderClear(renderer);
    // draw target with scanlines
    SDL_RenderCopy(renderer, textureTarget, nullptr, nullptr);

    // flip to display
    SDL_RenderPresent(renderer);
}
