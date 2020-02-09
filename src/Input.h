#ifndef PROJECT_INPUT_H
#define PROJECT_INPUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>

class Input {
private:
    enum State {
        None, Down, Up
    };
    static State state[];

public:
    static void event(SDL_Event *event);
    static void update();

    static bool getKey(SDL_Keycode keycode);
    static bool getKeyUp(SDL_Keycode keycode);
    static bool getKeyDown(SDL_Keycode keycode);
};


#endif //PROJECT_INPUT_H
