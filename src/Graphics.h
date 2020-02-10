#ifndef PROJECT_GRAPHICS_H
#define PROJECT_GRAPHICS_H

#include <SDL2/SDL.h>
#include <memory>
#include "types/Texture.h"

class Graphics {
private:
    static SDL_Window *window;
    static SDL_Renderer *renderer;

public:
    static bool init();
    static void unload();
    static void drawTexture(SDL_Texture *texture, SDL_Rect *srcrect, SDL_Rect *destrect);
    static std::shared_ptr<Texture> createTexture(SDL_Surface *surface);
    static void update();
};


#endif //PROJECT_GRAPHICS_H
