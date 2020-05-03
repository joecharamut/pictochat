#ifndef PROJECT_KEYBOARDGUI_H
#define PROJECT_KEYBOARDGUI_H

#include "ChatState.h"

#include <memory>
#include "../gui/GuiText.h"

class ChatState;

class KeyboardGui {
public:
    KeyboardGui();
    ~KeyboardGui();
    void setup(ChatState *instance);
    void draw();

    static const int lineLimit = 38;
    static const int line1Limit = 28;
    static const int line2Limit = line1Limit + lineLimit;
    static const int line3Limit = line2Limit + lineLimit;
    static const int line4Limit = line3Limit + lineLimit;
    static const int line5Limit = line4Limit + lineLimit;
    static const int messageLimit = line1Limit + (lineLimit * 4);

private:
    void updateTextLines();
    void pollBuffer();
    void updateDraw();
    void sendMessage();
    void clearDraw();
    std::string getSurfaceData();
    void loadSurfaceData(const std::string &data);
    void drawPixel(int x, int y, bool thick);
    void drawLine(int x1, int y1, int x2, int y2, bool thick);
    void stroke(int x1, int y1, int x2, int y2);

    void setThickPen(bool thick);
    bool thickPen;

    ChatState *instance = nullptr;

    bool showClick = false;
    bool surfaceClear = false;

    std::shared_ptr<Texture> drawTexture;
    const int drawWidth = 230;
    const int drawHeight = 80;
    int lastMouseX = -1;
    int lastMouseY = -1;

    std::shared_ptr<GuiImage> keyboardImage;
    std::shared_ptr<GuiButton> thickButtonOff;
    std::shared_ptr<GuiButton> thinButtonOff;
    std::shared_ptr<GuiButton> thickButtonOn;
    std::shared_ptr<GuiButton> thinButtonOn;

    std::string messageText;
    std::shared_ptr<GuiText> nameText;
    std::shared_ptr<GuiText> line1Text;
    std::shared_ptr<GuiText> line2Text;
    std::shared_ptr<GuiText> line3Text;
    std::shared_ptr<GuiText> line4Text;
    std::shared_ptr<GuiText> line5Text;
    std::shared_ptr<GuiButton> closeButton;
    std::shared_ptr<GuiButton> sendButton;
    std::shared_ptr<GuiButton> clearButton;
};


#endif //PROJECT_KEYBOARDGUI_H
