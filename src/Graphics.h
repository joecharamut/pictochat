#ifndef PROJECT_GRAPHICS_H
#define PROJECT_GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL_gpu.h>

#include <memory>
#include "types/Texture.h"

class Graphics {
public:
    static bool init();
    static void unload();
    static void drawTexture(GPU_Image *texture, GPU_Rect *srcrect, GPU_Rect *destrect);
    static std::shared_ptr<Texture> createTexture(SDL_Surface *surface);
    static void update();

private:
    static const int SCANLINE_LIMIT = 32;
    static int scanlineOffset;
};


#endif //PROJECT_GRAPHICS_H
