#ifndef PROJECT_UTIL_H
#define PROJECT_UTIL_H

#include <string>
#include <vector>

class Util {
public:
    static std::vector<std::string> splitString(const std::string &str, const std::string &delimiter);

    template <typename T>
    static bool vectorContains(std::vector<T> vector, T object) {
        for (const auto &item : vector) {
            if (item == object) {
                return true;
            }
        }
        return false;
    }

    static std::string replaceAll(const std::string &str, const std::string &from, const std::string &to);

    static std::string toUpperCase(const std::string &str);
    static std::string toLowerCase(const std::string &str);

    static bool stringEndsWith(const std::string &fullString, const std::string &ending);
    static bool stringStartsWith(const std::string &fullString, const std::string &start);

    static long long int getMilliTime();

    static std::string centerString(int width, const std::string &str, char padChar = ' ');
};


#endif //PROJECT_UTIL_H
