#ifndef PROJECT_FONT_H
#define PROJECT_FONT_H

#include <SDL_ttf.h>
#include <string>
#include <map>

class Font {
public:
    explicit Font(std::string filename);
    static void unload();

    TTF_Font *requestFont(int size);
    int getCharWidth(char c, int size);

private:
    std::string filename;
    std::map<int, std::map<char, int>> charSizes;

    static std::map<std::string, std::map<int, TTF_Font *>> fonts;
};


#endif //PROJECT_FONT_H
