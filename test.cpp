#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void loop();
void quit();
bool init();
bool setup();
int main(int argc, char* argv[]);

SDL_Window *window = nullptr;
SDL_Surface *surface = nullptr;

unsigned char r = rand(), g = rand(), b = rand();

bool quit_flag = false;

void loop() {
    surface = SDL_GetWindowSurface(window);

    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, r, g, b));
    SDL_UpdateWindowSurface(window);

    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_KEYDOWN:
                printf("keydown: %s\n", SDL_GetKeyName(event.key.keysym.sym));
                break;

            case SDL_KEYUP:
                printf("keyup: %s\n", SDL_GetKeyName(event.key.keysym.sym));
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit();
                        break;
                    case SDLK_SPACE:
                        r = rand() % 255;
                        g = rand() % 255;
                        b = rand() % 255;
                        break;
                }
                break;

            case SDL_QUIT:
                quit();
                break;

            default:
                break;
        }
    }
}

void quit() {
    printf("exiting\n");
    SDL_DestroyWindow(window);
    SDL_Quit();
    quit_flag = true;
}

TTF_Font* FNT_COMIC_SANS_16 = nullptr;
TTF_Font* FNT_COMIC_SANS_24 = nullptr;
TTF_Font* FNT_BIOS_16 = nullptr;
TTF_Font* FNT_BIOS_24 = nullptr;

bool loadFiles() {
    printf("loading fonts\n");

    FNT_COMIC_SANS_16 = TTF_OpenFont("res/font.ttf", 16);
    if(!FNT_COMIC_SANS_16) printf("font err: %s\n", TTF_GetError());
    FNT_COMIC_SANS_24 = TTF_OpenFont("res/font.ttf", 24);
    if(!FNT_COMIC_SANS_24) printf("font err: %s\n", TTF_GetError());
    FNT_BIOS_16 = TTF_OpenFont("res/bios.ttf", 16);
    if(!FNT_BIOS_16) printf("font err: %s\n", TTF_GetError());
    FNT_BIOS_24 = TTF_OpenFont("res/bios.ttf", 24);
    if(!FNT_BIOS_24) printf("font err: %s\n", TTF_GetError());

    printf("done\n");
}

bool setup() {
    printf("text start\n");
    SDL_Renderer* renderer = SDL_GetRenderer(window);
    SDL_Color white = {255, 255, 255};
    SDL_Color black = {0, 0, 0};
    SDL_Surface* messageSurface = TTF_RenderText_Shaded(FNT_COMIC_SANS_24, "comic sans", white, black);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, messageSurface);

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 640;
    rect.h = 480;
    SDL_RenderCopy(renderer, message, NULL, &rect);
    printf("text end\n");
    return true;
}

bool init() {
    printf("init sdl\n");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("error init sdl: %s\n", SDL_GetError());
        return -1;
    }

    printf("init ttf\n");
    if (TTF_Init() < 0) {
        printf("error init ttf: %s\n", SDL_GetError());
        return -1;
    }

    printf("creating window\n");
    window = SDL_CreateWindow("hello", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("error creating window: %s\n", SDL_GetError());
        return -1;
    }

    loadFiles();


#ifdef EMSCRIPTEN
    emscripten_set_main_loop(loop, 0, true);
#endif

    // setup();

    return true;
}

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = `%s`\n", i, argv[i]);
    }

    init();

#ifndef EMSCRIPTEN
    while (!quit_flag) {
        loop();
    }
#endif

    return 0;
}
