#include "Main.h"
#include "Graphics.h"
#include "types/Text.h"
#include "ResourceManager.h"
#include "Input.h"
#include "Util.h"
#include "state/State.h"
#include "state/TestState.h"
#include "state/StateManager.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

bool Main::quit_flag = false;

int main(int argc, char **argv) {
    return Main::programMain(argc, argv);
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

    printf("init mixer\n");
    int flags = MIX_INIT_OGG;
    int got = Mix_Init(flags);
    printf("fl: %d, got: %d, and: %d\n", flags, got, (got&flags));
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

    std::shared_ptr<State> testState = std::static_pointer_cast<State>(std::make_shared<TestState>());
    if (!StateManager::setState(testState)) {
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

    if (time - lastTime > 500) {
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
}

void Main::quit() {
    printf("quitting\n");

    Graphics::unload();

    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
    quit_flag = true;
#ifdef EMSCRIPTEN
    emscripten_force_exit(EXIT_SUCCESS);
#endif
}
