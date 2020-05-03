#include "GuiButton.h"

void GuiButton::draw() {
    int mx;
    int my;
    int buttons = SDL_GetMouseState(&mx, &my);
    bool leftClick = buttons & SDL_BUTTON_LMASK;
    bool clickUp = false;

    if (leftClick && !lastClick) {
        lastClick = true;
    } else if (!leftClick && lastClick) {
        lastClick = false;
        clickUp = true;
    }

    if (mx >= x && mx < x + texture->w && my >= y && my < y + texture->h) {
        if (hoverColor.r != 0xff && hoverColor.g != 0xff && hoverColor.b != 0xff) {
            texture->setTint(hoverColor.r, hoverColor.g, hoverColor.b);
        }
        eventHandler(clickUp);
    } else {
        if (hoverColor.r != 0xff && hoverColor.g != 0xff && hoverColor.b != 0xff) {
            texture->setTint(0xff, 0xff, 0xff);
        }
    }

    texture->draw(x, y);
}

void GuiButton::move(int x, int y) {
    this->x = x;
    this->y = y;
}
