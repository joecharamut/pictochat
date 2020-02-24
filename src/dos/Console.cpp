#include "Console.h"

Console::Console() {
    for (auto &line : textBuffer) {
        for (char &c : line) {
            c = 0;
        }
    }
}

void Console::update() {
    flush(1);
}

void Console::write(const std::string &str) {
    writeBuffer += str;
}

void Console::flush(int chars) {
    size_t pos = chars;

    if (writeBuffer.empty()) return;
    if (pos < 0) {
        pos = std::string::npos;
    }

    if (pos > writeBuffer.size()) {
        internalWrite(writeBuffer);
        writeBuffer = "";
    } else {
        internalWrite(writeBuffer.substr(0, pos));
        writeBuffer = writeBuffer.substr(pos);
    }
}

void Console::internalWrite(const std::string &str) {
    dirty = true;

    int cx = cursorPos.x;
    int cy = cursorPos.y;

    for (char c : str) {
        if (c == '\n') {
            cx = 0;
            cy++;
            continue;
        }

        if (c == '\b') {
            cx--;
            if (cx < 0) {
                cx = 0;
            }
            continue;
        }

        if (c == '\r') {
            cx = 0;
            continue;
        }

        if (cx > textBuffer[cy].size() - 1) {
            cx = 0;
            cy++;
        }

        if (cy > textBuffer.size() - 1) {
            scrollBuffer(1);
            cy = textBuffer.size() - 1;
        }

        textBuffer[cy][cx] = c;
        cx++;
    }

    cursorPos = {cx, cy};
}

void Console::scrollBuffer(int lines) {
    dirty = true;
    for (int l = 0; l < lines; l++) {
        for (int i = 1; i < textBuffer.size(); i++) {
            textBuffer[i - 1] = textBuffer[i];
        }

        for (char &c : textBuffer.back()) {
            c = 0;
        }

        cursorPos = {cursorPos.x, cursorPos.y - 1};
    }
}

std::string Console::bufferString() {
    if (!dirty) {
        return cachedBuffer;
    }

    std::string out;

    for (auto line : textBuffer) {
        for (char c : line) {
            if (c == 0) {
                break;
            }

            out += c;
        }
        // todo : newline prevents \r from working right
        out += "\n";
    }

    dirty = false;
    cachedBuffer = out;
    return out;
}

int Console::cursorToStringIndex() {
    int i = 0;
    int cx = 0;
    int cy = 0;

    std::string str = bufferString();

    for (char c : str) {
        i++;
        cx++;

        if (c == '\n') {
            cx = 0;
            cy++;
        }

        if (cx == cursorPos.x && cy == cursorPos.y) {
            return i;
        }
    }

    return i;
}
