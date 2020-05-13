#include "MobileTextInput.h"

#define __EMSCRIPTEN__ 1

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL2/SDL.h>
#endif

void MobileTextInput::showInput() {
#ifdef __EMSCRIPTEN__
    EM_ASM(
        showTextInput();
    );

    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
    SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
    SDL_EventState(SDL_KEYUP, SDL_DISABLE);
#endif
}

bool MobileTextInput::pollInput() {
    bool ret = false;
#ifdef __EMSCRIPTEN__
    ret = EM_ASM_INT_V({
       return isTextWaiting();
    });

    if (!ret) {
        SDL_EventState(SDL_TEXTINPUT, SDL_ENABLE);
        SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
        SDL_EventState(SDL_KEYUP, SDL_ENABLE);
    }
#endif
    return ret;
}

#ifdef __EMSCRIPTEN__
EM_JS(char *, em_get_input, (), {
    var str = getText();
    var len = lengthBytesUTF8(str) + 1;
    var heapStr = _malloc(len);
    stringToUTF8(str, heapStr, len);
    return heapStr;
});
#endif

std::string MobileTextInput::getInput() {
#ifdef __EMSCRIPTEN__
    char *str = em_get_input();
    std::string theString(str);
    free(str);
    return theString;
#else
    return std::string();
#endif
}
