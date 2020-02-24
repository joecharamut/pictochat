#ifndef PROJECT_CONSOLE_H
#define PROJECT_CONSOLE_H

#include <string>
#include <array>
#include "../types/Vector2.h"

class Console {
public:
    Console();
    ~Console() = default;

    void update();
    void flush(int chars = -1);
    void write(const std::string &str);
    void scrollBuffer(int lines);
    std::string bufferString();
    int cursorToStringIndex();

private:
    void internalWrite(const std::string &str);

    std::string writeBuffer;
    std::string cachedBuffer;
    bool dirty = true;
    std::array<std::array<char, 90>, 30> textBuffer {};
    Vector2 cursorPos = {0, 0};
};


#endif //PROJECT_CONSOLE_H
