#ifndef PROJECT_FILESYSTEM_H
#define PROJECT_FILESYSTEM_H

//#define __EMSCRIPTEN__ 1

#include <string>
#include <memory>
#include <vector>
#include <map>

#include "File.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class Filesystem {
public:
    static bool init();
    static void unload();

    static bool fileExists(const std::string &virtualPath);
    static std::shared_ptr<File> createFile(const std::string &virtualPath);
    static std::shared_ptr<File> openFile(const std::string &virtualPath);
    static void deleteFile(const std::string &virtualPath);

private:
    // virtual path -> real file map
    static std::map<std::string, std::string> files;

#ifdef __EMSCRIPTEN__
    static bool init_emscripten();
    static void sync_emscripten();
#else
    static bool init_native();
#endif
};

#endif //PROJECT_FILESYSTEM_H
