#ifndef PROJECT_GUIMACROS_H
#define PROJECT_GUIMACROS_H

#define LoadTexture(PATH) ResourceManager::loadTexture(PATH)
#define GUI_IMAGE(tex, x, y) std::make_shared<GuiImage>(tex, x, y)
#define GUI_BUTTON(t, x, y, c, f) std::make_shared<GuiButton>(t, x, y, c, f)
#define GUI_DS_TEXT(t, c, x, y) std::make_shared<GuiText>(std::make_shared<Text>(t, \
    std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16, Text::Blended, c), x, y)

#endif //PROJECT_GUIMACROS_H
