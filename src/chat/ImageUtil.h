#ifndef PROJECT_IMAGEUTIL_H
#define PROJECT_IMAGEUTIL_H

#include <memory>
#include "../types/Texture.h"

class ImageUtil {
public:
    static std::string encode(const std::shared_ptr<Texture>& texture);
    static std::shared_ptr<Texture> decode(const std::string &base64String, int w, int h);

private:
    static std::string encodeNC(const std::shared_ptr<Texture> &texture);
    static std::shared_ptr<Texture> decodeNC(const std::string &base64, int w, int h);

    static std::string encodeRLE(const std::shared_ptr<Texture> &texture);
    static std::shared_ptr<Texture> decodeRLE(const std::string &base64, int w, int h);

    static std::string encodeBP(const std::shared_ptr<Texture> &texture);
    static std::shared_ptr<Texture> decodeBP(const std::string &base64, int w, int h);
};


#endif //PROJECT_IMAGEUTIL_H
