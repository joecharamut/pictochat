#include "Filesystem.h"
#include "../util/Util.h"

#include <filesystem>
#include <map>
#include <algorithm>
#include <fstream>

std::map<std::string, std::string> Filesystem::files;

bool Filesystem::init() {
    bool initSuccess;
#ifdef __EMSCRIPTEN__
    initSuccess = init_emscripten();
#else
    initSuccess = init_native();
#endif
    if (!initSuccess) return false;

    File index = File("fs/files.list");
    if (index.size() > 0) {
        std::shared_ptr<std::string> content = index.read();
        printf("content: %s\n", content->c_str());
        std::vector<std::string> split = Util::splitString(*content, ",");

        for (int i = 0; i < split.size() - 1; i+= 2) {
            files[split[i]] = split[i+1];
        }

        printf("done %ld files\n", files.size());
    }

    return true;
}

#ifdef __EMSCRIPTEN__
bool Filesystem::init_emscripten() {
    // chdir to / so relative path is the same for both
    // /fs is mounted in preinit js code
    MAIN_THREAD_EM_ASM(
            FS.chdir("/");
            );

    return true;
}

void Filesystem::sync_emscripten() {
    // mm js in c++ what more could you want
    // flush fs back to indexedDB
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

void Filesystem::unload() {
#ifdef __EMSCRIPTEN__
    sync_emscripten();
#endif
}

bool Filesystem::fileExists(const std::string &virtualPath) {
    return files.count(virtualPath) > 0;
}

std::shared_ptr<File> Filesystem::createFile(const std::string &virtualPath) {
    // create file if not exists, truncate, return file ptr
    std::string realPath;
    if (!fileExists(virtualPath)) {
        std::string randFile;
        do {
            randFile = "fs/" + std::to_string(rand()) + ".txt";
        } while (!std::filesystem::exists(randFile));
        realPath = randFile;

        files[virtualPath] = realPath;
    } else {
        realPath = files[virtualPath];
    }

    std::ofstream ofs;
    ofs.open(realPath, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    return nullptr;
}

std::shared_ptr<File> Filesystem::openFile(const std::string &virtualPath) {
    if (!fileExists(virtualPath)) {
        return nullptr;
    }

    return std::make_shared<File>(files[virtualPath]);
}

void Filesystem::deleteFile(const std::string &virtualPath) {

}



