#include <vector>
#include "ImageUtil.h"
#include "../Graphics.h"
#include "../util/Base64.h"
#include <nlohmann/json.hpp>
#include <array>

// public stubs
std::string ImageUtil::encode(const std::shared_ptr<Texture> &texture) {
    return encodeBP(texture);
//    return encodeRLE(texture);
//    return encodeNC(texture);
}

std::shared_ptr<Texture> ImageUtil::decode(const std::string &base64String, int w, int h) {
    return decodeBP(base64String, w, h);
//    return decodeRLE(base64String, w, h);
//    return decodeNC(base64String, w, h);
}

// no compress
std::string ImageUtil::encodeNC(const std::shared_ptr<Texture> &texture) {
    // RGBA32 = 4 bytes / pxl
    // only need to check alpha
    // alpha should be 4th byte
    int width = texture->w;
    int height = texture->h;
    auto pixels = texture->getPixels();

    std::vector<bool> bitmap(width * height);
    for (int i = 0; i < width * height; i++) {
        bitmap[i] = pixels.get()[(i * 4) + 3] == 0xff;
    }

    std::string jsonStr = ((nlohmann::json) {{"data", bitmap}}).dump();

    std::string enc = Base64::base64_encode((unsigned char *) jsonStr.c_str(), jsonStr.size());
    return enc;
}

std::shared_ptr<Texture> ImageUtil::decodeNC(const std::string &base64, int w, int h) {
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(Graphics::createTexture(w, h));
    tex->clearTexture();

    auto json = nlohmann::json::parse(Base64::base64_decode(base64));
    auto data = json["data"].get<std::vector<bool>>();
    int x = 0;
    int y = 0;

    for (bool b : data) {
        if (b) {
            tex->drawPixel({0x00, 0x00, 0x00, 0xff}, x, y);
        }

        x++;
        if (x >= w) {
            x = 0;
            y++;
        }
    }

    return tex;
}

// run-length encoding
std::string ImageUtil::encodeRLE(const std::shared_ptr<Texture> &texture) {
    // RGBA32 = 4 bytes / pxl
    // only need to check alpha
    // alpha should be 4th byte
    int width = texture->w;
    int height = texture->h;
    auto pixels = texture->getPixels();

    bool bitmap[width * height];
    for (int i = 0; i < width * height; i++) {
        bitmap[i] = pixels.get()[(i * 4) + 3] == 0xff;
    }

    std::vector<int> packed;
    int counter = 0;
    bool last = bitmap[0];

    for (bool b : bitmap) {
        if (b == last) {
            counter++;
        } else {
            last = b;
            packed.push_back(counter);
            packed.push_back(b ? 0 : 1);
            counter = 1;
        }
    }

    std::string jsonStr = ((nlohmann::json) {{"data", packed}}).dump();

    std::string enc = Base64::base64_encode((unsigned char *) jsonStr.c_str(), jsonStr.size());
    return enc;
}

std::shared_ptr<Texture> ImageUtil::decodeRLE(const std::string &base64, int w, int h) {
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(Graphics::createTexture(w, h));
    tex->clearTexture();

    auto json = nlohmann::json::parse(Base64::base64_decode(base64));
    auto data = json["data"].get<std::vector<int>>();
    int x = 0;
    int y = 0;

    for (int i = 0; i < data.size(); i += 2) {
        int num = data[i];
        bool theValue = data[i+1] == 1;

        for (int j = 0; j < num; j++) {
            if (theValue) {
                tex->drawPixel({0x00, 0x00, 0x00, 0xff}, x, y);
            }

            x++;
            if (x >= w) {
                x = 0;
                y++;
            }
        }
    }

    return tex;
}

// pack bits into bytes
std::string ImageUtil::encodeBP(const std::shared_ptr<Texture> &texture) {
    // RGBA32 = 4 bytes / pxl
    // only need to check alpha
    // alpha should be 4th byte
    int width = texture->w;
    int height = texture->h;
    auto pixels = texture->getPixels();

    bool bitmap[width * height];
    for (int i = 0; i < width * height; i++) {
        bitmap[i] = pixels.get()[(i * 4) + 3] == 0xff;
    }

    // pack into byte
    unsigned char bitmapPacked[(width * height) / 8];
    for (int i = 0; i < (width * height) / 8; i++) {
        unsigned char cons = 0;

        for (int j = 0; j < 8; j++) {
            cons |= (bitmap[(i * 8) + j] << j);
        }

        bitmapPacked[i] = cons;
    }

    std::string enc = Base64::base64_encode(bitmapPacked, sizeof(bitmapPacked));
//    printf("%s\n", enc.c_str());
    return enc;
}

std::shared_ptr<Texture> ImageUtil::decodeBP(const std::string &base64String, int w, int h) {
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(Graphics::createTexture(w, h));
    tex->clearTexture();

    std::string decoded = Base64::base64_decode(base64String);
    const char *bytes = decoded.c_str();
    int numBytes = decoded.size();

    int x = 0;
    int y = 0;

    for (int i = 0; i < numBytes; i++) {
        int workingByte = bytes[i];

        for (int j = 0; j < 8; j++) {
            int pixelValue = (workingByte & (1 << j)) != 0 ? 0xff : 0x00;

            if (pixelValue) {
                tex->drawPixel(SDL_Color {0x00, 0x00, 0x00, 0xff}, x, y);
            }

            x++;
            if (x >= w) {
                x = 0;
                y++;
            }
        }
    }

    return tex;
}
