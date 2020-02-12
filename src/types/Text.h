#ifndef PROJECT_TEXT_H
#define PROJECT_TEXT_H


#include <string>
#include <SDL_ttf.h>
#include <memory>
#include "Font.h"
#include "Texture.h"

class Text {


public:
    enum FontMode {
        Solid, Shaded, Blended, Blended_Wrapped
    };

    Text(const std::string &text, Font font, int size, FontMode mode, SDL_Color fgColor, SDL_Color bgColor, int wrap);
    ~Text() = default;
    Vector2 getSize();
    void draw(SDL_Rect *destRect);

private:
    std::shared_ptr<Texture> texture;
};


#endif //PROJECT_TEXT_H
