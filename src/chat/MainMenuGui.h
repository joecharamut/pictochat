#ifndef PROJECT_MAINMENUGUI_H
#define PROJECT_MAINMENUGUI_H

#include <memory>
#include "../gui/GuiImage.h"
#include "../gui/GuiButton.h"
#include "../Main.h"
#include "../gui/GuiText.h"
#include "../types/Timer.h"
#include "ChatState.h"

class ChatState;

class MainMenuGui {
public:
    MainMenuGui();
    ~MainMenuGui();
    void setup(std::string userId, ChatState *instance);
    void draw();

private:
    void updateCursorPos();
    void pingStatus();

    const int SCREEN_W = 256;
    const int SCREEN_H = 192;
    const int CENTER_X = (Main::SCREEN_WIDTH / 2) - (SCREEN_W / 2);
    const int CENTER_Y = (Main::SCREEN_HEIGHT / 2) - (SCREEN_H);
    const int BASE_X = CENTER_X;
    const int BASE_Y = CENTER_Y + SCREEN_H;

    SDL_Cursor *handCursor;
    SDL_Cursor *defaultCursor;
    bool showClick;

    Timer timer = Timer();

    int users[4] = {-1, -1, -1, -1};

    std::string userId;
    ChatState *instance;

    std::shared_ptr<GuiImage> top;
    std::shared_ptr<GuiImage> bottom;

    std::shared_ptr<GuiImage> signalRedA;
    std::shared_ptr<GuiImage> signalRedB;
    std::shared_ptr<GuiImage> signalRedC;
    std::shared_ptr<GuiImage> signalRedD;
    std::shared_ptr<GuiImage> signalGreenA;
    std::shared_ptr<GuiImage> signalGreenB;
    std::shared_ptr<GuiImage> signalGreenC;
    std::shared_ptr<GuiImage> signalGreenD;

    std::shared_ptr<GuiButton> backButton;
    std::shared_ptr<GuiButton> cursor;

    std::shared_ptr<GuiText> onlineTextA;
    std::shared_ptr<GuiText> onlineTextB;
    std::shared_ptr<GuiText> onlineTextC;
    std::shared_ptr<GuiText> onlineTextD;

    int cursorX = BASE_X + 26, cursorY = BASE_Y + 28;
};


#endif //PROJECT_MAINMENUGUI_H
