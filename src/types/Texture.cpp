#include "Texture.h"
#include "../Graphics.h"

Texture::Texture(SDL_Texture *texture) {
    this->texture = texture;
}

Texture::~Texture() {
    printf("destroying texture\n");
    SDL_DestroyTexture(texture);
}

// srcRect = source texture (null = whole)
// destRect = screen (null = stretch to whole)
void Texture::draw(SDL_Rect *srcRect, SDL_Rect *destRect) {
    Graphics::drawTexture(texture, srcRect, destRect);
}

Vector2 Texture::getSize() {
    unsigned int format;
    int access;
    int w;
    int h;
    SDL_QueryTexture(texture, &format, &access, &w, &h);
    return {w, h};
}
