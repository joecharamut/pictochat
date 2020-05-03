#ifndef PROJECT_GRAPHICS_H
#define PROJECT_GRAPHICS_H

#include <SDL2/SDL.h>
#include <memory>
#include "types/Texture.h"
#include <map>

class Graphics {
public:
    static bool init();
    static void unload();
    static void drawTexture(SDL_Texture *texture, SDL_Rect *srcrect, SDL_Rect *destrect);
    static std::shared_ptr<Texture> createTexture(SDL_Surface *surface);
    static void update();
    static void setClearColor(SDL_Color color);
    static void setWindowSize(int w, int h);

    static SDL_Texture *createTexture(int w, int h);
    static void textureDrawLine(SDL_Texture *tex, SDL_Color color, int x1, int y1, int x2, int y2, int thickness);
    static void textureDrawPoint(SDL_Texture *tex, SDL_Color color, int x, int y);
    static void textureDrawRect(SDL_Texture *tex, SDL_Color color, SDL_Rect rect);
    static std::shared_ptr<unsigned char> getTexturePixels(SDL_Texture *tex);
    static void clearTexture(SDL_Texture *tex);

    enum CursorType {
        Default, Hand, Crosshair
    };
    static void setCurosr(CursorType type);

private:
    static constexpr double pi() { return 3.141592653589793238463; };

    static SDL_Window *window;
    static SDL_Renderer *renderer;

    static SDL_Color clearColor;

    static std::map<CursorType, SDL_Cursor *> cursors;

    static SDL_Texture *onePixel;
};


#endif //PROJECT_GRAPHICS_H
