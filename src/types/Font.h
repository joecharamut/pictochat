#ifndef PROJECT_FONT_H
#define PROJECT_FONT_H

#include <SDL_ttf.h>
#include <string>
#include <map>

class Font {
public:
    explicit Font(std::string filename);
    TTF_Font *requestFont(int size);

    static void unload();

private:
    std::string filename;
    static std::map<std::string, std::map<int, TTF_Font *>> fonts;
};


#endif //PROJECT_FONT_H
