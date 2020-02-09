#ifndef PROJECT_RESOURCEMANAGER_H
#define PROJECT_RESOURCEMANAGER_H

#include <SDL_ttf.h>
#include <memory>

#include "types/Texture.h"

using namespace std;

class ResourceManager {
public:
    static shared_ptr<Texture> loadTexture(const string& filename);
};


#endif //PROJECT_RESOURCEMANAGER_H
