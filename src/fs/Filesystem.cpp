#include "Filesystem.h"

#include <filesystem>

bool Filesystem::init() {
#ifdef __EMSCRIPTEN__
    return init_emscripten();
#else
    return init_native();
#endif
}

void Filesystem::sync() {
#ifdef __EMSCRIPTEN__
    sync_emscripten();
#endif
}

#ifdef __EMSCRIPTEN__
bool Filesystem::init_emscripten() {
    // test code
//    for (const auto &entry : std::filesystem::directory_iterator("/fs")) {
//        printf("%s\n", entry.path().c_str());
//    }
//
//    srand(time(0));
//    std::string file = "/fs/test" + std::to_string(rand());
//    std::ofstream out(file);
//    out << "hello world" << std::endl;
//    out.close();
//
//    for (const auto &entry : std::filesystem::directory_iterator("/fs")) {
//        printf("%s\n", entry.path().c_str());
//    }

    // chdir to / so relative path is the same for both
    MAIN_THREAD_EM_ASM(
            FS.chdir("/");
            );

    return true;
}

void Filesystem::sync_emscripten() {
    MAIN_THREAD_EM_ASM(
            FS.syncfs(false, function (err) {
                console.log("idbfs sync complete");
                if (err) console.log(err);
            });
            );
}
#else
bool Filesystem::init_native() {
    if (!std::filesystem::exists("./fs")) {
        if (!std::filesystem::create_directory("./fs")) {
            return false;
        }
    }

    return true;
}
#endif

