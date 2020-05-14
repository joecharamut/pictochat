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

    static const int line1Width = 168;
    static const int lineWidth = 224;

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
    bool thickPen = true;

    void setEraser(bool erase);
    bool eraserEnabled = false;

    void exitRoom();

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

    std::shared_ptr<GuiButton> penButtonOff;
    std::shared_ptr<GuiButton> eraserButtonOff;
    std::shared_ptr<GuiButton> penButtonOn;
    std::shared_ptr<GuiButton> eraserButtonOn;

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
