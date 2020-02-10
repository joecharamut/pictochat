#ifndef PROJECT_COLOR_H
#define PROJECT_COLOR_H

#include <SDL2/SDL.h>

#define COLOR(r, g, b) ((SDL_Color) {r, g, b})

#define COLOR_BLACK COLOR(0x00, 0x00, 0x00)
#define COLOR_WHITE COLOR(0xff, 0xff, 0xff)

#define COLOR_RED   COLOR(0xff, 0x00, 0x00)
#define COLOR_GREEN COLOR(0x00, 0xff, 0x00)
#define COLOR_BLUE  COLOR(0x00, 0x00, 0xff)

#endif //PROJECT_COLOR_H
