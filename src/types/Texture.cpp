#include "Texture.h"
#include "../Graphics.h"

Texture::Texture(SDL_Texture *texture) {
    this->texture = texture;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
}

Texture::~Texture() {
//    printf("destroying texture\n");
    SDL_DestroyTexture(texture);
}

// srcRect = source texture (null = whole)
// destRect = screen (null = stretch to whole)
void Texture::draw(SDL_Rect *srcRect, SDL_Rect *destRect) {
    Graphics::drawTexture(texture, srcRect, destRect);
}
