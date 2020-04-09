#include "Texture.h"
#include "../Graphics.h"

Texture::Texture(GPU_Image *texture) {
    this->texture = texture;
    w = texture->w;
    h = texture->h;
}

Texture::~Texture() {
//    printf("destroying texture\n");
    GPU_FreeImage(texture);
}

// srcRect = source texture (null = whole)
// destRect = screen (null = stretch to whole)
void Texture::draw(GPU_Rect *srcRect, GPU_Rect *destRect) {
    Graphics::drawTexture(texture, srcRect, destRect);
}
