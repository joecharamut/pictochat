#include "Main.h"
#include "Graphics.h"
#include "types/Text.h"
#include "ResourceManager.h"
#include "Input.h"
#include "Util.h"
#include "state/State.h"
#include "state/TestState.h"
#include "state/StateManager.h"
#include "types/Color.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <iostream>

const int Main::SCREEN_WIDTH = 640;
const int Main::SCREEN_HEIGHT = 480;
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

std::shared_ptr<State> testState;
void Main::callback(Network::Response resp) {
    std::string dataStr;
    for(int i = 0; i < resp.size; i++) {
        dataStr += resp.data.get()->at(i);
    }

    std::cout <<
              "status: " << resp.status <<
              ", statusCode: " << resp.statusCode <<
              ", statusText: " << resp.statusText <<
              ", size: " << resp.size <<
              ", data: \n" << dataStr << std::endl;

//    std::static_pointer_cast<TestState>(testState)->text = make_shared<Text>(dataStr, Font("res/bios.ttf"), 14,
//            Text::Blended_Wrapped, COLOR(0xff, 0x00, 0xff), COLOR(0x00, 0xff, 0x00), 640);
    std::static_pointer_cast<TestState>(testState)->text->setText(dataStr);
}

bool Main::load() {
    bool error = false;

    testState = std::static_pointer_cast<State>(std::make_shared<TestState>());
    if (!StateManager::setState(testState)) {
        printf("error setting state\n");
        error = true;
    }

    Network::Request *request = new Network::Request("http://www.spaghetti.rocks/form.php?view=1", Network::GET, Main::callback);
    request->execute();

    return !error;
}

int frames = 0;
long long int lastTime = 0;

void Main::loop() {
    Graphics::update();
    Input::update();

    frames++;
    long long int time = Util::getMilliTime();

    if (time - lastTime > 1000) {
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
#ifdef __EMSCRIPTEN__
    emscripten_force_exit(EXIT_SUCCESS);
#endif
}
