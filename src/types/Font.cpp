#include <utility>

#include "Font.h"

std::map<std::string, std::map<int, TTF_Font *>> Font::fonts;

Font::Font(std::string filename) {
    this->filename = std::move(filename);
}

void Font::unload() {
    printf("unloading fonts\n");
    int total = 0;
    if (TTF_WasInit()) {
        auto iterFonts = fonts.begin();
        while (iterFonts != fonts.end()) {
            auto iterOpen = iterFonts->second.begin();
            while (iterOpen != iterFonts->second.end()) {
                TTF_CloseFont(iterOpen->second);
                total++;
                iterOpen++;
            }
            iterFonts++;
        }
        printf("unloaded %d fonts\n", total);
    } else {
        printf("ttf already closed (this is probably bad)\n");
    }
}

TTF_Font *Font::requestFont(int size) {
    if (fonts.count(filename) > 0) {
        if (fonts[filename].count(size) > 0) {
            return fonts[filename][size];
        }
    }

    TTF_Font *font = TTF_OpenFont(filename.c_str(), size);
    if (!font) {
        printf("font err: %s\n", SDL_GetError());
        SDL_ClearError();
        return nullptr;
    }
    fonts[filename][size] = font;
    return font;
}

int Font::getCharWidth(char c, int size) {
    if (charSizes.count(size) > 0) {
        if (charSizes[size].count(c) > 0) {
            return charSizes[size][c];
        }
    }

    TTF_Font *fnt = requestFont(size);
    char str[] = {0, 0};
    int w;

    for (char iter = 0x20; iter < 0x7f; iter++) {
        str[0] = iter;
        if (TTF_SizeText(fnt, str, &w, nullptr) < 0) {
            printf("error scanning text size (char: %c, font: %s, size: %d, err: %s)\n", iter, filename.c_str(), size,
                    SDL_GetError());
        }
        charSizes[size][iter] = w;
//        printf("charsize(%c) = %d\n", iter, w);
    }

    return charSizes[size][c];
}
