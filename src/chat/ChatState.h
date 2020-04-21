#ifndef PROJECT_CHATSTATE_H
#define PROJECT_CHATSTATE_H


#include <nlohmann/json.hpp>
#include "../state/State.h"
#include "../Main.h"
#include "../types/Timer.h"
#include "../types/Texture.h"
#include "../net/Socket.h"
#include "MainMenuGui.h"
#include "TopScreenGui.h"
#include "KeyboardGui.h"


class MainMenuGui;
class TopScreenGui;
class KeyboardGui;

class ChatState : public State {
public:
    ChatState();
    ~ChatState() override;
    void update() override;
    void pingServer();
    void setRoom(const std::string &room);
    void exitRoom();

    std::shared_ptr<Socket> socket;
    bool online = false;
    std::string username;
    std::string room;

    std::shared_ptr<TopScreenGui> topScreenGui = std::make_shared<TopScreenGui>();
private:
    void pingResponse(const std::string &jsonData);
    void usernameResponse(bool valid);
    void socketHandler(Socket::EventData event);

    enum State {
        SETUP,
        SERVER_CONNECT, SERVER_PING, SERVER_WAIT, SERVER_REPLY,
        INPUT_USERNAME, CHECK_USERNAME, WAIT_USERNAME, DONE_USERNAME,
        ERROR, NOOP
    };
    State state = (State) 0;

    enum BottomScreenState {
        BLANK, USERNAME, MAIN_MENU, KEYBOARD
    };
    BottomScreenState bottomState = (BottomScreenState) 0;

    Timer timer = Timer();

    std::shared_ptr<GuiImage> blankTexture = nullptr;
    std::shared_ptr<GuiImage> usernameTexture = nullptr;
    std::shared_ptr<GuiText> usernameText = nullptr;

    std::shared_ptr<MainMenuGui> mainMenuGui = std::make_shared<MainMenuGui>();
    std::shared_ptr<KeyboardGui> keyboardGui = std::make_shared<KeyboardGui>();
};


#endif //PROJECT_CHATSTATE_H
