#include "GuiImage.h"

void GuiImage::draw() {
    texture->draw(x, y);
}

void GuiImage::move(int x, int y) {
    this->x = x;
    this->y = y;
}
