#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#include <SDL_ttf.h>
#include "net/Request.h"

class Main {
public:
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;
    static int IS_MOBILE;

    static bool programMain(int argc, char **argv);

    static bool initSDL();
    static bool load();
    static void loop();
    static void quit();

private:
    static bool quit_flag;
};


#endif //PROJECT_MAIN_H
