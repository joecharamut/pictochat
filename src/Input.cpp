#include "Input.h"

Input::State Input::state[128];

bool Input::keyBufferEnabled;
std::string Input::keyBuffer;

void Input::event(SDL_Event *event) {
    switch (event->type) {
        case SDL_KEYDOWN: {
//            printf("keydown: %s\n", SDL_GetKeyName(event->key.keysym.sym));
            if (event->key.keysym.sym < 128) {
                state[event->key.keysym.sym] = Down;
            }
        } break;

        case SDL_KEYUP: {
//            printf("keyup: %s\n", SDL_GetKeyName(event->key.keysym.sym));
            if (event->key.keysym.sym < 128) {
                state[event->key.keysym.sym] = Up;
            }
        } break;

        case SDL_TEXTINPUT: {
//            printf("text: %s\n", event->text.text);
            pushBuffer(event->text.text);
        } break;

        default: break;
    }
}

void Input::update() {
    for (auto &key : state) {
        if (key == Up) {
            key = None;
        }

        if (key == Down) {
            key = Hold;
        }
    }
}

bool Input::getKey(SDL_Keycode keycode) {
    return state[keycode] != None;
}

bool Input::getKeyUp(SDL_Keycode keycode) {
    return state[keycode] == Up;
}

bool Input::getKeyDown(SDL_Keycode keycode) {
    return state[keycode] == Down;
}

void Input::enableKeyBuffer() {
    keyBufferEnabled = true;
}

void Input::disableKeyBuffer() {
    keyBufferEnabled = false;
}

std::string Input::popBuffer(int chars) {
    if (!keyBufferEnabled) return "";

    if (chars == -1 || chars > keyBuffer.size()) {
        std::string tmp = keyBuffer;
        keyBuffer = "";
        return tmp;
    }

    std::string tmp = keyBuffer.substr(0, chars);
    keyBuffer = keyBuffer.substr(chars);
    return tmp;
}

void Input::pushBuffer(std::string str) {
    if (!keyBufferEnabled) return;

    keyBuffer += str;
}
