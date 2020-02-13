#include <utility>

#include "Font.h"

Font::Font(std::string filename) {
    this->filename = std::move(filename);
}

Font::~Font() {
    printf("deleting font\n");
    if (TTF_WasInit()) {
        auto iter = fonts.begin();
        while (iter != fonts.end()) {
            TTF_CloseFont(iter->second);
            iter++;
        }
    } else {
        printf("ttf already closed oh gosh oh heck looks like were wasting memory now\n");
    }
}

TTF_Font *Font::requestFont(int size) {
    if (fonts.count(size) > 0) {
        return fonts[size];
    }

    TTF_Font *font = TTF_OpenFont(filename.c_str(), size);
    if (!font) {
        printf("font err: %s\n", SDL_GetError());
        SDL_ClearError();
        return nullptr;
    }
    fonts[size] = font;
    return font;
}
