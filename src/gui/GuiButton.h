#ifndef PROJECT_GUIBUTTON_H
#define PROJECT_GUIBUTTON_H

#include "GuiObject.h"
#include "../types/Texture.h"
#include <memory>
#include <functional>
#include <utility>

class GuiButton : public GuiObject {
public:
    GuiButton(std::shared_ptr<Texture> texture, int x, int y, SDL_Color hoverColor, std::function<void(bool)> eventHandler) :
        texture(std::move(texture)), x(x), y(y), hoverColor(hoverColor), eventHandler(std::move(eventHandler)) {
    }
    ~GuiButton() override = default;
    void draw() override;
    void move(int x, int y);

    std::shared_ptr<Texture> texture;

private:
    int x;
    int y;
    SDL_Color hoverColor;
    std::function<void(bool)> eventHandler;
};


#endif //PROJECT_GUIBUTTON_H
