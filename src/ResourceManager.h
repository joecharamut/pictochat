#ifndef PROJECT_RESOURCEMANAGER_H
#define PROJECT_RESOURCEMANAGER_H

#include <SDL_ttf.h>
#include <memory>
#include <map>

#include "types/Texture.h"
#include "types/Sound.h"

class ResourceManager {
public:
    static std::shared_ptr<Texture> loadTexture(const std::string &filename);
    static std::shared_ptr<Sound> loadSound(const std::string &filename);

private:
    static std::map<std::string, std::shared_ptr<Texture>> loadedTextures;
};


#endif //PROJECT_RESOURCEMANAGER_H
