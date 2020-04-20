#include <vector>
#include <SDL_image.h>
#include "ResourceManager.h"
#include "util/Util.h"
#include "Graphics.h"

std::map<std::string, std::shared_ptr<Texture>> ResourceManager::loadedTextures;

std::shared_ptr<Texture> ResourceManager::loadTexture(const std::string& filename) {
    if (loadedTextures.count(filename) > 0) {
        return loadedTextures[filename];
    }

    std::vector<std::string> split = Util::splitString(filename, ".");
    std::string ext = split.back();

    SDL_Surface *surface = nullptr;

    if (ext == "bmp") {
        surface = SDL_LoadBMP(filename.c_str());
    } else {
        surface = IMG_Load(filename.c_str());
    }

    if (!surface) {
        printf("image loader err: %s\n", SDL_GetError());
//        SDL_ClearError();
        return nullptr;
    }

    std::shared_ptr<Texture> texture = Graphics::createTexture(surface);
    loadedTextures[filename] = texture;

    SDL_FreeSurface(surface);
    return texture;
}

std::shared_ptr<Sound> ResourceManager::loadSound(const std::string &filename) {
    Mix_Chunk *chunk = Mix_LoadWAV(filename.c_str());

    if (!chunk) {
        printf("error loading sound: %s\n", SDL_GetError());
        return nullptr;
    }

    return std::make_shared<Sound>(chunk);
}
