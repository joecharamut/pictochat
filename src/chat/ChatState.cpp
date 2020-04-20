#include "ChatState.h"
#include "../util/Util.h"
#include "../ResourceManager.h"
#include "../Input.h"

ChatState::ChatState() {
    connectingTexture = ResourceManager::loadTexture("res/pictochat/connecting.png");
    errorTexture = ResourceManager::loadTexture("res/pictochat/error.png");
}

ChatState::~ChatState() {
    socket->close();
}

void ChatState::update() {
    timer.tick();

    switch (state) {
        default: break;

        case ERROR: {
            errorTexture->draw(0, 192);
        } break;

        case SETUP: {
            state = SERVER_CONNECT;
            topScreenGui->setup(this);
            topScreenGui->pushMessage(Message(Message::WELCOME_MESSAGE, ""));
            topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "Connecting..."));

            socket = std::make_shared<Socket>("ws://localhost:1337",
                    std::bind(&ChatState::socketHandler, this, std::placeholders::_1));

            mainMenuGui->setup("", this);
        } break;

        case SERVER_CONNECT: {

        } break;

        case SERVER_PING: {
            pingServer();
            state = SERVER_WAIT;
        } break;

        case SERVER_WAIT: {
            connectingTexture->draw(0, 192);
        } break;

        case SERVER_REPLY: {
            if (!online) {
                printf("Server not online.\n\n");
                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "Server not online"));
                state = NOOP;
            } else {
                printf("OK\n");
                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "Connection Successful"));
                windowEnabled = true;

//                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "and"));
//                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "then"));
//                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "he"));
//                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "turned"));
//                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "himself"));
//                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "into"));
//                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "a"));
//                topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "pickle"));

                state = NOOP;
            }
        } break;

        case INPUT_USERNAME: {
            Input::enableKeyBuffer();
            std::string str = Input::popBuffer();
            if (!str.empty()) {
                for (char c : str) {
                    if (isalnum(c)) {
                        username += c;
                        printf("%s", std::string(1, c).c_str());
                    }
                }
            }

            if (Input::getKeyUp(SDLK_RETURN)) {
                printf("\n");
                Input::disableKeyBuffer();
                state = CHECK_USERNAME;
            }

            if (Input::getKeyUp(SDLK_BACKSPACE)) {
                if (!username.empty()) {
                    printf("\b \b");
                    username.pop_back();
                }
            }
        } break;

        case CHECK_USERNAME: {
            state = WAIT_USERNAME;
            Network::Request req = Network::Request("http://lab.spaghetti.rocks:8080/register", Network::POST,
                    [this](Network::Response resp) {
                if (resp.status == Network::SUCCESS && resp.statusCode == 200) {
                    std::string respStr;
                    for (char b : *resp.data) {
                        respStr += b;
                    }
                    userId = respStr;
                    state = DONE_USERNAME;
                } else {
                    printf("Username unavailable\n");
                    exit(0);
                }
            });
            req.setPostData(username);
            req.execute();
        } break;

        case WAIT_USERNAME: {

        } break;

        case DONE_USERNAME: {
            printf("Connected\n");
            mainMenuGui->setup(userId, this);
            windowEnabled = true;
        } break;
    }

    if (windowEnabled) {
        drawWindow();
    }
    topScreenGui->draw();
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
        } break;

        case Socket::Error: {
            printf("socket error\n");
            online = false;
            state = ERROR;
        } break;

        case Socket::Message: {
            std::string msgString = (const char *) event.messageData;
            if (!online) {
                auto parsed = nlohmann::json::parse(msgString);

                auto type = parsed["type"].get<std::string>();
                nlohmann::basic_json json = parsed;

                if (type == "status") {
                    pingResponse(msgString);
                } else {
                    printf("unknown json type: %s\n", type.c_str());
                }
            } else {
                mainMenuGui->socketMessage(msgString);
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

void ChatState::drawWindow() {
    mainMenuGui->draw();
}


