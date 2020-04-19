#include "GuiText.h"

void GuiText::draw() {
    text->draw(x, y);
}

void GuiText::move(int x, int y) {
    this->x = x;
    this->y = y;
}
