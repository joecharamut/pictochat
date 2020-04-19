#ifndef PROJECT_CHATSTATE_H
#define PROJECT_CHATSTATE_H


#include <SDL_rect.h>
#include "../state/State.h"
#include "../Main.h"
#include "../types/Timer.h"
#include "../types/Texture.h"
#include "MainMenuGui.h"

class MainMenuGui;

class ChatState : public State {
public:
    ChatState();
    ~ChatState() override = default;
    void update() override;

private:
    void drawWindow();
    void pingServer();

    enum BottomScreenState {
        MAIN_MENU
    };
    BottomScreenState state = MAIN_MENU;

    enum InitState {
        SERVER_PING, SERVER_WAIT, SERVER_REPLY,
        INPUT_USERNAME, CHECK_USERNAME, WAIT_USERNAME, DONE_USERNAME
    };
    InitState initState = SERVER_PING;

    Timer timer = Timer();

    std::string username;
    std::string userId;

    const int SCREEN_W = 256;
    const int SCREEN_H = 192;
    const int CENTER_X = (Main::SCREEN_WIDTH / 2) - (SCREEN_W / 2);
    const int CENTER_Y = (Main::SCREEN_HEIGHT / 2) - (SCREEN_H);
    const int BASE_X = CENTER_X;
    const int BASE_Y = CENTER_Y + SCREEN_H;

    SDL_Rect frameRect {};

    std::shared_ptr<Texture> frame;

    std::shared_ptr<MainMenuGui> mainMenuGui = std::make_shared<MainMenuGui>();

    bool online = false;
    bool windowEnabled = false;
};


#endif //PROJECT_CHATSTATE_H
