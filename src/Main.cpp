#include "Main.h"
#include "Graphics.h"
#include "types/Text.h"
#include "ResourceManager.h"
#include "Input.h"
#include "Util.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

shared_ptr<Texture> tex;

bool Main::quit_flag = false;

int main(int argc, char **argv) {
    return Main::programMain(argc, argv);
}

bool Main::programMain(int argc, char **argv) {
    if (!initSDL()) {
        return EXIT_FAILURE;
    }

    if (!Graphics::initGraphics()) {
        return EXIT_FAILURE;
    }

    if (!loadFiles()) {
        return EXIT_FAILURE;
    }

#ifdef EMSCRIPTEN
    emscripten_set_main_loop(loop, 0, true);
#else
    while (!quit_flag) {
        loop();
    }
#endif

    return EXIT_SUCCESS;
}

bool Main::initSDL() {
    printf("init sdl\n");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("error init sdl: %s\n", SDL_GetError());
        return false;
    }

    printf("init ttf\n");
    if (TTF_Init() < 0) {
        printf("error init ttf: %s\n", SDL_GetError());
        return false;
    }

    return true;
}



bool Main::loadFiles() {
    bool error = false;

    printf("loading images\n");
    tex = ResourceManager::loadTexture("res/test.bmp");

    printf("loading test audio\n");

    return !error;
}

Text *textInst = nullptr;

int frames = 0;
long long int lastTime = 0;


void Main::loop() {
    Graphics::update();
    Input::update();

    frames++;
    long long int time = Util::getMilliTime();

    if (time - lastTime > 500) {
        float fps = frames / ((time - lastTime) / 1000.f);
        lastTime = time;
        frames = 0;
        printf("fps: %f\n", fps);
    }


    if (!textInst) {
        textInst = new Text("sans", Font("res/bios.ttf"), 24, Text::Solid,
                {0xff, 0x00, 0xff}, {0x00, 0xff, 0x00});
    }

    SDL_Rect rect;
    rect.x = (Main::SCREEN_WIDTH/2)-16;
    rect.y = (Main::SCREEN_HEIGHT/2)-16;
    rect.w = 32;
    rect.h = 32;

    Vector2 size = textInst->getSize();
    SDL_Rect rect2 = {0, 0, size.x, size.y};

    textInst->draw(&rect2);
    tex->draw(nullptr, &rect);

    SDL_Event event;
    while(SDL_PollEvent(&event)){
        // process input events
        Input::event(&event);

        // do other events
        switch (event.type) {
            case SDL_QUIT:
                quit();
                break;

            default:
                break;
        }
    }

    if (Input::getKeyUp(SDLK_ESCAPE)) {
        quit();
    }
}

void Main::quit() {
    printf("quitting\n");
    TTF_Quit();
    SDL_Quit();
    quit_flag = true;
#ifdef EMSCRIPTEN
    emscripten_force_exit(EXIT_SUCCESS);
#endif
}
