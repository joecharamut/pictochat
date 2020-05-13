#include "ChatState.h"
#include "../util/Util.h"
#include "../ResourceManager.h"
#include "../Input.h"
#include "GuiMacros.h"
#include "../util/Base64.h"
#include "../MobileTextInput.h"

ChatState::ChatState() {
    blankTexture = GUI_IMAGE(LoadTexture("res/pictochat/blankscreen.png"), 0, 192);
    usernameTexture = GUI_IMAGE(LoadTexture("res/pictochat/usernameprompt.png"), 11, 192+72);
    usernameText = GUI_DS_TEXT("", COLOR_WHITE, 106, 192+83);
}

ChatState::~ChatState() {
    socket->close();
}

void ChatState::update() {
    timer.tick();

    switch (state) {
        default: break;

        case ERROR: {
            topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "Could not connect to server"));
            state = NOOP;
        } break;

        case CLOSED: {
            topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "Server closed connection"));
            state = NOOP;
        } break;

        case SETUP: {
            state = SERVER_CONNECT;
            topScreenGui->setup(this);
            topScreenGui->pushMessage(Message(Message::WELCOME_MESSAGE, ""));
            topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "Connecting..."));

            socket = std::make_shared<Socket>("ws://lab.spaghetti.rocks:8069",
                    std::bind(&ChatState::socketHandler, this, std::placeholders::_1));
        } break;

        case SERVER_CONNECT: {

        } break;

        case SERVER_PING: {
            pingServer();
            state = SERVER_WAIT;
        } break;

        case SERVER_WAIT: {

        } break;

        case SERVER_REPLY: {
            if (!online) {
                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "Server not online"));
                state = NOOP;
            } else {
                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "Connection Successful"));
                bottomState = USERNAME;
                state = INPUT_USERNAME;
            }
        } break;

        case INPUT_USERNAME: {
            if (Main::IS_MOBILE) {
                MobileTextInput::showInput();
                state = INPUT_USERNAME_MOBILE;
                break;
            }

            Input::enableKeyBuffer();
            std::string str = Input::popBuffer();
            if (!str.empty()) {
                for (char c : str) {
                    if (username.size() < 10) {
                        if (isalnum(c)) {
                            username += c;
                            usernameText->text->setText(username);
                        }
                    }
                }
            }

            if (Input::getKeyUp(SDLK_RETURN)) {
                Input::disableKeyBuffer();
                state = CHECK_USERNAME;
            }

            if (Input::getKeyDown(SDLK_BACKSPACE)) {
                if (!username.empty()) {
                    username.pop_back();
                    usernameText->text->setText(username);
                }
            }
        } break;

        case INPUT_USERNAME_MOBILE: {
            if (!MobileTextInput::pollInput()) {
                username = MobileTextInput::getInput();
                usernameText->text->setText(username);
                state = CHECK_USERNAME;
            }
        } break;

        case CHECK_USERNAME: {
            state = WAIT_USERNAME;

            socket->send(((nlohmann::json) {
                    {"action", "username"},
                    {"username", username}
            }).dump());
        } break;

        case WAIT_USERNAME: {

        } break;

        case DONE_USERNAME: {
            mainMenuGui->setup(this);
            keyboardGui->setup(this);
            bottomState = MAIN_MENU;
            state = NOOP;
        } break;
    }

    topScreenGui->draw();

    switch (bottomState) {
        default: break;

        case BLANK: {
            blankTexture->draw();
        } break;

        case USERNAME: {
            blankTexture->draw();
            usernameTexture->draw();
            usernameText->draw();
        } break;

        case MAIN_MENU: {
            mainMenuGui->draw();
        } break;

        case KEYBOARD: {
            keyboardGui->draw();
        } break;
    }
}

void ChatState::socketHandler(Socket::EventData event) {
    switch (event.type) {
        default: break;

        case Socket::Open: {
            printf("connected\n");
            state = SERVER_PING;
        } break;

        case Socket::Close: {
            printf("socket closed\n");
            online = false;
            state = CLOSED;
        } break;

        case Socket::Error: {
            printf("socket error\n");
            online = false;
            state = ERROR;
        } break;

        case Socket::Message: {
            std::string msgString = (const char *) event.messageData;
            auto parsed = nlohmann::json::parse(msgString);
            auto type = parsed["type"].get<std::string>();

            if (type == "status" || type == "join") {
                mainMenuGui->socketMessage(msgString);
            }

            if (type == "status") {
                pingResponse(msgString);
            } else if (type == "username") {
                usernameResponse(parsed["valid"].get<bool>());
            } else if (type == "message") {
//                printf("%s\n", msgString.c_str());
                try {
                    auto messageType = (Message::MessageType) parsed["message"]["type"].get<int>();
                    auto messageData = parsed["message"]["data"].get<std::string>();
                    auto messageUser = parsed["message"]["user"].get<std::string>();
                    auto messageImage = parsed["message"]["image"].is_null() ?
                            "" : parsed["message"]["image"].get<std::string>();

                    topScreenGui->pushMessage(Message(messageType, messageData, messageUser, messageImage));
                } catch (std::exception &e) {
                    printf("error: %s\n", e.what());
                }
            }
        } break;
    }
}

void ChatState::pingServer() {
    socket->send(R"({"action":"status"})");
}

void ChatState::pingResponse(const std::string &jsonData) {
    online = true;
    if (state == SERVER_WAIT) state = SERVER_REPLY;
}

void ChatState::setRoom(const std::string &room) {
    this->room = room;
    bottomState = KEYBOARD;
}

void ChatState::exitRoom() {

}

void ChatState::usernameResponse(bool valid) {
    if (!valid) {
        username = "";
        usernameText->text->setText(username);
        topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "Invalid Username"));
        state = INPUT_USERNAME;
    } else {
        state = DONE_USERNAME;
    }
}
