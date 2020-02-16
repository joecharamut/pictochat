#ifndef PROJECT_TEXT_H
#define PROJECT_TEXT_H


#include <string>
#include <SDL_ttf.h>
#include <memory>
#include "Font.h"
#include "Texture.h"
#include "Color.h"

class Text {
public:
    enum FontMode {
        Solid, Shaded, Blended, Blended_Wrapped
    };

    Text(const std::string &text, std::shared_ptr<Font> font, int size, FontMode mode, SDL_Color fgColor,
            SDL_Color bgColor = COLOR_BLACK, int wrap = -1);
    ~Text();
    void draw(SDL_Rect destRect);
    void draw(int x, int y);
    void setText(const std::string& newText);

    int w = -1;
    int h = -1;

private:
    std::shared_ptr<Texture> texture;

    std::string text;
    std::shared_ptr<Font> font;
    int size;
    FontMode mode;
    SDL_Color fgColor;
    SDL_Color bgColor;
    int wrap;
};


#endif //PROJECT_TEXT_H
