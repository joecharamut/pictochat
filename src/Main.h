#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#include <SDL_ttf.h>

class Main {
public:
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 480;

    static bool programMain(int argc, char **argv);

    static bool initSDL();
    static bool loadFiles();
    static void loop();
    static void quit();

private:
    static bool quit_flag;
};


#endif //PROJECT_MAIN_H
