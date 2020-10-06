#include "Main.h"
#include "Graphics.h"
#include "types/Text.h"
#include "ResourceManager.h"
#include "Input.h"
#include "util/Util.h"
#include "state/State.h"
#include "state/StateManager.h"
#include "types/Color.h"
#include "chat/ChatState.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <iostream>
#include <SDL_image.h>
#include <stdexcept>

const int Main::SCREEN_WIDTH = 256;
const int Main::SCREEN_HEIGHT = 192*2;
int Main::IS_MOBILE = false;

bool Main::quit_flag = false;

int main(int argc, char **argv) {
    try {
        return Main::programMain(argc, argv);
    } catch (std::exception &e) {
        printf("caught error: %s\n", e.what());
    }
}

bool Main::programMain(int argc, char **argv) {
    if (!initSDL()) {
        return EXIT_FAILURE;
    }

    if (!Graphics::init()) {
        return EXIT_FAILURE;
    }

    if (!load()) {
        return EXIT_FAILURE;
    }

    const char *var = std::getenv("ismobile");
    if (var) {
        IS_MOBILE = std::string(var) == "true";
    }

#ifdef __EMSCRIPTEN__
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

    printf("init img\n");
    int imgFlags = IMG_INIT_PNG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        printf("error init img: %s\n", SDL_GetError());
        return false;
    }

    printf("init ttf\n");
    if (TTF_Init() < 0) {
        printf("error init ttf: %s\n", SDL_GetError());
        return false;
    }

    printf("init mixer\n");
    int flags = MIX_INIT_OGG;
    int got = Mix_Init(flags);
    if ((got & flags) != flags) {
        printf("error init mixer: %s\n", SDL_GetError());
        return false;
    }

    printf("open audio\n");
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        printf("error open audio: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool Main::load() {
    bool error = false;

    std::shared_ptr<State> st = std::static_pointer_cast<State>(std::make_shared<ChatState>());
    if (!StateManager::setState(st)) {
        printf("error setting state\n");
        error = true;
    }

    return !error;
}

int frames = 0;
long long int lastTime = 0;

void Main::loop() {
    Graphics::update();
    Input::update();

    frames++;
    long long int time = Util::getMilliTime();

    if (time - lastTime > 5000) {
        float fps = frames / ((time - lastTime) / 1000.f);
        lastTime = time;
        frames = 0;
        printf("fps: %f\n", fps);
    }

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

#ifdef __EMSCRIPTEN__
    int shouldExit = EM_ASM_INT_V({
        return pictochatGetShouldExit();
    });

    if (shouldExit) {
        quit();
    }
#endif

}

void Main::quit() {
    printf("quitting\n");

    Graphics::unload();
    Font::unload();

    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    quit_flag = true;
#ifdef __EMSCRIPTEN__
    EM_ASM({
        pictochatSetExited();
    });
    emscripten_force_exit(EXIT_SUCCESS);
#endif
}
