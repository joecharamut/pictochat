#ifndef PROJECT_TEXTURE_H
#define PROJECT_TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL_gpu.h>
#include "Vector2.h"

class Texture {
private:
    GPU_Image *texture = nullptr;

public:
    int w = -1;
    int h = -1;

    explicit Texture(GPU_Image *texture);
    ~Texture();
    void draw(GPU_Rect *srcRect, GPU_Rect *destRect);
};


#endif //PROJECT_TEXTURE_H
