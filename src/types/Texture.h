#ifndef PROJECT_TEXTURE_H
#define PROJECT_TEXTURE_H

#include <SDL2/SDL.h>
#include "Vector2.h"

class Texture {
private:
    SDL_Texture *texture = nullptr;

public:
    int w = -1;
    int h = -1;

    explicit Texture(SDL_Texture *texture);
    ~Texture();
    void draw(SDL_Rect *srcRect, SDL_Rect *destRect);
};


#endif //PROJECT_TEXTURE_H
