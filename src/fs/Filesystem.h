#ifndef PROJECT_FILESYSTEM_H
#define PROJECT_FILESYSTEM_H

//#define __EMSCRIPTEN__ 1

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class Filesystem {
public:
    static bool init();
    static void sync();

private:
#ifdef __EMSCRIPTEN__
    static bool init_emscripten();
    static void sync_emscripten();
#else
    static bool init_native();
#endif
};


#endif //PROJECT_FILESYSTEM_H
