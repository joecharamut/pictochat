#ifndef PROJECT_CHATSTATE_H
#define PROJECT_CHATSTATE_H


#include <SDL_rect.h>
#include "../state/State.h"
#include "../Main.h"
#include "../types/Timer.h"
#include "../types/Texture.h"
#include "MainMenuGui.h"
#include "TopScreenGui.h"
#include "../net/Socket.h"
#include <nlohmann/json.hpp>


class MainMenuGui;
class TopScreenGui;

class ChatState : public State {
public:
    ChatState();
    ~ChatState() override;
    void update() override;
    void pingServer();

    std::shared_ptr<Socket> socket;
    bool online = false;

private:
    void drawWindow();
    void pingResponse(const std::string &jsonData);
    void socketHandler(Socket::EventData event);

    enum State {
        SETUP,
        SERVER_CONNECT, SERVER_PING, SERVER_WAIT, SERVER_REPLY,
        INPUT_USERNAME, CHECK_USERNAME, WAIT_USERNAME, DONE_USERNAME,
        ERROR, NOOP
    };
    State state = (State) 0;

    Timer timer = Timer();

    std::string username;
    std::string userId;

    std::shared_ptr<Texture> connectingTexture = nullptr;
    std::shared_ptr<Texture> errorTexture = nullptr;

    std::shared_ptr<MainMenuGui> mainMenuGui = std::make_shared<MainMenuGui>();
    std::shared_ptr<TopScreenGui> topScreenGui = std::make_shared<TopScreenGui>();

    bool windowEnabled = false;
};


#endif //PROJECT_CHATSTATE_H
