#include <vector>
#include "ResourceManager.h"
#include "util/Util.h"
#include "Graphics.h"

shared_ptr<Texture> ResourceManager::loadTexture(const string& filename) {
    vector<string> split = Util::splitString(filename, ".");
    string ext = split.back();

    SDL_Surface *surface = nullptr;

    if (ext == "bmp") {
        surface = SDL_LoadBMP(filename.c_str());
    } else {
        printf("ERR: No loader function for images of type %s (%s)\n", ext.c_str(), filename.c_str());
        return nullptr;
    }

    if (!surface) {
        printf("image loader err: %s\n", SDL_GetError());
//        SDL_ClearError();
        return nullptr;
    }

    shared_ptr<Texture> texture = Graphics::createTexture(surface);

    SDL_FreeSurface(surface);
    return texture;
}
