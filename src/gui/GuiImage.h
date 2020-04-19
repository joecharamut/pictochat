#include <utility>

#ifndef PROJECT_GUIIMAGE_H
#define PROJECT_GUIIMAGE_H

#include "GuiObject.h"
#include "../types/Texture.h"
#include <memory>

class GuiImage : public GuiObject {
public:
    GuiImage(std::shared_ptr<Texture> texture, int x, int y) : texture(std::move(texture)), x(x), y(y) {};
    ~GuiImage() override = default;
    void draw() override;
    void move(int x, int y);

    std::shared_ptr<Texture> texture;

private:
    int x;
    int y;
};


#endif //PROJECT_GUIIMAGE_H
