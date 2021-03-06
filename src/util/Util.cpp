#include <utility>
#include <iostream>
#include <chrono>

#include "Util.h"

std::vector<std::string> Util::splitString(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> out = {};
    size_t start, end = 0;
    while ((start = str.find_first_not_of(delimiter, end)) != std::string::npos) {
        end = str.find(delimiter, start);
        out.push_back(str.substr(start, end-start));
    }
    return out;
}

std::string Util::replaceAll(const std::string &str, const std::string &from, const std::string &to) {
    std::string copy = str;
    size_t start_pos = 0;
    while((start_pos = copy.find(from, start_pos)) != std::string::npos) {
        copy.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return copy;
}

std::string Util::toUpperCase(const std::string &str) {
    std::string ret;
    for (char c : str) {
        ret += (char) toupper(c);
    }
    return ret;
}

std::string Util::toLowerCase(const std::string &str) {
    std::string ret;
    for (char c : str) {
        ret += (char) tolower(c);
    }
    return ret;
}

bool Util::stringEndsWith(const std::string &fullString, const std::string &ending) {
    if (fullString.length() >= ending.length()) {
        return (fullString.compare(fullString.length() - ending.length(), ending.length(), ending) == 0);
    } else {
        return false;
    }
}

bool Util::stringStartsWith(const std::string &fullString, const std::string &start) {
    if (fullString.length() >= start.length()) {
        return fullString.substr(0, start.size()) == start;
    } else {
        return false;
    }
}

long long int Util::getMilliTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
}

std::string Util::centerString(int width, const std::string &str, char padChar) {
    int len = str.length();
    if(width < len) { return str; }

    int diff = width - len;
    int pad1 = diff/2;
    int pad2 = diff - pad1;
    return std::string(pad1, padChar) + str + std::string(pad2, padChar);
}
