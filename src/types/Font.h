#ifndef PROJECT_FONT_H
#define PROJECT_FONT_H

#include <SDL_ttf.h>
#include <string>
#include <map>

class Font {
public:
    explicit Font(std::string filename);
    ~Font();
    TTF_Font *requestFont(int size);

private:
    std::string filename;
    std::map<int, TTF_Font *> fonts;
};


#endif //PROJECT_FONT_H
