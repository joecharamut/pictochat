#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#include <SDL_ttf.h>
#include "net/Request.h"

class Main {
public:
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 480;

    static bool programMain(int argc, char **argv);

    static bool initSDL();
    static bool load();
    static void loop();
    static void quit();
    static void callback(Network::Response *resp);

private:
    static bool quit_flag;
};


#endif //PROJECT_MAIN_H
