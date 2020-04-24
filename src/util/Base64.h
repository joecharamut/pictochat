#ifndef PROJECT_BASE64_H
#define PROJECT_BASE64_H

#include <string>

class Base64 {
private:
    static const std::string base64_chars;

    static inline bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

public:
    static std::string base64_encode(unsigned char const*, unsigned int len);
    static std::string base64_decode(std::string const& s);
};


#endif //PROJECT_BASE64_H
