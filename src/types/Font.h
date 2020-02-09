#ifndef PROJECT_FONT_H
#define PROJECT_FONT_H

#include <SDL_ttf.h>
#include <string>
#include <map>

class Font {
private:
    std::string filename;
    std::map<int, TTF_Font *> fonts;

public:
    explicit Font(std::string filename);
    ~Font();
    TTF_Font *requestFont(int size);

};


#endif //PROJECT_FONT_H
