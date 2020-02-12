#include "Text.h"
#include "../Graphics.h"

Text::Text(const std::string &text, Font font, int size, FontMode mode, SDL_Color fgColor, SDL_Color bgColor, int wrap) {
    SDL_Surface *surface = nullptr;
    switch (mode) {
        case Solid:
            surface = TTF_RenderText_Solid(font.requestFont(size), text.c_str(), fgColor);
            break;
        case Shaded:
            surface = TTF_RenderText_Shaded(font.requestFont(size), text.c_str(), fgColor, bgColor);
            break;
        case Blended:
            surface = TTF_RenderText_Blended(font.requestFont(size), text.c_str(), fgColor);
            break;
        case Blended_Wrapped:
            surface = TTF_RenderText_Blended_Wrapped(font.requestFont(size), text.c_str(), fgColor, wrap);
            break;
    }
    texture = Graphics::createTexture(surface);
    SDL_FreeSurface(surface);
}



void Text::draw(SDL_Rect *destRect) {
    texture->draw(nullptr, destRect);
}

Vector2 Text::getSize() {
    return texture->getSize();
}
