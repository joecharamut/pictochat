#include "Graphics.h"
#include "Main.h"
#include "state/StateManager.h"

int Graphics::scanlineOffset = 0;

#include <GL/glew.h>
GPU_Target *gWindow;
GPU_Image *target;
GPU_Image *scanlines;

int vert, frag, shader;
GPU_ShaderBlock block;

bool Graphics::init() {
    printf("creating window\n");

    gWindow = GPU_Init(Main::SCREEN_WIDTH, Main::SCREEN_HEIGHT, GPU_DEFAULT_INIT_FLAGS);
    if (!gWindow) {
        printf("error creating window: %s\n", SDL_GetError());
        return false;
    }

    target = GPU_CreateImage(Main::SCREEN_WIDTH, Main::SCREEN_HEIGHT, GPU_FORMAT_RGBA);
    if (!target) {
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

    scanlines = GPU_CopyImageFromSurface(scanlineSurface);
    SDL_FreeSurface(loadSurface);
    SDL_FreeSurface(scanlineSurface);


    vert = GPU_LoadShader(GPU_VERTEX_SHADER, "res/shader/crt.vert");
    printf("vert: %s\n", GPU_GetShaderMessage());

    frag = GPU_LoadShader(GPU_FRAGMENT_SHADER, "res/shader/crt.frag");
    printf("frag: %s\n", GPU_GetShaderMessage());

    shader = GPU_LinkShaders(vert, frag);
    printf("link: %s\n", GPU_GetShaderMessage());

    block = GPU_LoadShaderBlock(shader, "gpu_Vertex", "gpu_TexCoord", "gpu_Color", "gpu_ModelViewProjectionMatrix");
    int screenSize[] {Main::SCREEN_WIDTH, Main::SCREEN_HEIGHT};
    GPU_SetAttributeiv(GPU_GetUniformLocation(shader, "size"), 2, (int *) &screenSize);

    GPU_FreeShader(vert);
    GPU_FreeShader(frag);

    return true;
}

void Graphics::unload() {
    GPU_FreeImage(target);
    GPU_FreeImage(scanlines);
}

void Graphics::drawTexture(GPU_Image *texture, GPU_Rect *srcrect, GPU_Rect *destrect) {
    GPU_BlitRect(texture, srcrect, gWindow, destrect);
}

std::shared_ptr<Texture> Graphics::createTexture(SDL_Surface *surface) {
//    printf("creating texture\n");
    return std::make_shared<Texture>(GPU_CopyImageFromSurface(surface));
}

void Graphics::update() {
    GPU_ClearRGBA(gWindow, 0x25, 0x25, 0x25, 0xff);
    // draw state gfx
    StateManager::update();

    // update scanline positions
    scanlineOffset++;
    if (scanlineOffset > SCANLINE_LIMIT) scanlineOffset = 0;
    GPU_Rect rect {0, (scanlineOffset / 2.f) - Main::SCREEN_HEIGHT, (float) Main::SCREEN_WIDTH, (float) Main::SCREEN_HEIGHT * 2};
    // draw scanlines
//    GPU_BlitRect(scanlines, nullptr, gWindow, &rect);

    GPU_FreeImage(target);
    target = GPU_CopyImageFromTarget(gWindow);
    GPU_ClearRGBA(gWindow, 0x25, 0x25, 0x0, 0xff);

    GPU_ActivateShaderProgram(shader, &block);
    GPU_SetShaderImage(target, GPU_GetUniformLocation(shader, "sampler"), 1);

    GPU_BlitRect(target, nullptr, gWindow, nullptr);

    GPU_ActivateShaderProgram(0, nullptr);

    GPU_Flip(gWindow);
}
