#ifndef PROJECT_TEXTURE_H
#define PROJECT_TEXTURE_H

#include <SDL2/SDL.h>
#include "Vector2.h"
#include <memory>

class Texture {

public:
    int w = -1;
    int h = -1;

    explicit Texture(SDL_Texture *texture);
    ~Texture();
    void draw(SDL_Rect *srcRect, SDL_Rect *destRect);
    void draw(int x, int y);
    void setTint(int r, int g, int b);

    void clearTexture();
    std::shared_ptr<unsigned char> getPixels();
    void drawPixel(SDL_Color color, int x, int y, int thickness);
    void drawLine(SDL_Color color, int x1, int y1, int x2, int y2, int thickness);

    SDL_Texture *texture = nullptr;
};


#endif //PROJECT_TEXTURE_H
