#ifndef PROJECT_GUITEXT_H
#define PROJECT_GUITEXT_H

#include "GuiObject.h"
#include "../types/Text.h"

#include <memory>

class GuiText : public GuiObject {
public:
    GuiText(std::shared_ptr<Text> text, int x, int y) : text(std::move(text)), x(x), y(y) {};
    ~GuiText() override = default;
    void draw() override;
    void move(int x, int y);

    std::shared_ptr<Text> text;

private:
    int x;
    int y;
};


#endif //PROJECT_GUITEXT_H
