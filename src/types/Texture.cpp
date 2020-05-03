#include "Texture.h"
#include "../Graphics.h"

Texture::Texture(SDL_Texture *texture) {
    this->texture = texture;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
}

#include <emscripten/emscripten.h>

Texture::~Texture() {
//    printf("destroying texture\n");
    SDL_DestroyTexture(texture);
}

// srcRect = source texture (null = whole)
// destRect = screen (null = stretch to whole)
void Texture::draw(SDL_Rect *srcRect, SDL_Rect *destRect) {
    Graphics::drawTexture(texture, srcRect, destRect);
}

void Texture::draw(int x, int y) {
    SDL_Rect rect {x, y, w, h};
    draw(nullptr, &rect);
}

void Texture::setTint(int r, int g, int b) {
    SDL_SetTextureColorMod(texture, r, g, b);
}

void Texture::clearTexture() {
    Graphics::clearTexture(texture);
}

std::shared_ptr<unsigned char> Texture::getPixels() {
    return Graphics::getTexturePixels(texture);
}

void Texture::drawPixel(SDL_Color color, int x, int y) {
    Graphics::textureDrawPoint(texture, color, x, y);
}

void Texture::drawLine(SDL_Color color, int x1, int y1, int x2, int y2, int thickness) {
    Graphics::textureDrawLine(texture, color, x1, y1, x2, y2, thickness);
}

void Texture::drawRect(SDL_Color color, SDL_Rect rect) {
    Graphics::textureDrawRect(texture, color, rect);
}
