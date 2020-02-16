#ifndef PROJECT_INPUT_H
#define PROJECT_INPUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <string>

class Input {
public:
    static void event(SDL_Event *event);
    static void update();

    static bool getKey(SDL_Keycode keycode);
    static bool getKeyUp(SDL_Keycode keycode);
    static bool getKeyDown(SDL_Keycode keycode);

    static void enableKeyBuffer();
    static void disableKeyBuffer();
    static std::string popBuffer(int chars = -1);

private:
    enum State {
        None, Down, Up
    };
    static State state[];

    static bool keyBufferEnabled;
    static std::string keyBuffer;
};


#endif //PROJECT_INPUT_H
