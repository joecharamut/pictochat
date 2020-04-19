#include "GuiButton.h"

void GuiButton::draw() {
    int mx;
    int my;
    int buttons = SDL_GetMouseState(&mx, &my);
    bool leftClick = buttons & SDL_BUTTON_LMASK;

    if (mx >= x && mx < x + texture->w && my >= y && my < y + texture->h) {
        texture->setTint(hoverColor.r, hoverColor.g, hoverColor.b);
        eventHandler(leftClick);
    } else {
        texture->setTint(0xff, 0xff, 0xff);
    }

    texture->draw(x, y);
}

void GuiButton::move(int x, int y) {
    this->x = x;
    this->y = y;
}
