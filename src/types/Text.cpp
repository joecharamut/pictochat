#include <utility>

#include "Text.h"
#include "../Graphics.h"

Text::Text(const std::string &text, std::shared_ptr<Font> font, int size, FontMode mode, SDL_Color fgColor,
        SDL_Color bgColor, int wrap)
        : text(text), font(std::move(font)), size(size), mode(mode), fgColor(fgColor), bgColor(bgColor), wrap(wrap){
    setText(text);
}

Text::~Text() {

}

void Text::setText(const std::string &newText) {
    text = newText;

    SDL_Surface *surface = nullptr;
    switch (mode) {
        case Solid:
            surface = TTF_RenderText_Solid(font->requestFont(size), text.c_str(), fgColor);
            break;
        case Shaded:
            surface = TTF_RenderText_Shaded(font->requestFont(size), text.c_str(), fgColor, bgColor);
            break;
        case Blended:
            surface = TTF_RenderText_Blended(font->requestFont(size), text.c_str(), fgColor);
            break;
        case Blended_Wrapped:
            surface = TTF_RenderText_Blended_Wrapped(font->requestFont(size), text.c_str(), fgColor, wrap);
            break;
    }
//    texture.reset();
    texture = Graphics::createTexture(surface);
    w = texture->w;
    h = texture->h;
    SDL_FreeSurface(surface);
}

void Text::draw(GPU_Rect destRect) {
    texture->draw(nullptr, &destRect);
}

void Text::draw(int x, int y) {
    this->draw((GPU_Rect) {(float) x, (float) y, (float) w, (float) h});
}
