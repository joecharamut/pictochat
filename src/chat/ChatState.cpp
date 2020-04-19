#include "ChatState.h"
#include "../util/Util.h"
#include "../ResourceManager.h"
#include "../Input.h"

ChatState::ChatState() {
    frame = ResourceManager::loadTexture("res/pictochat/frame.png");

    frameRect = {
            (Main::SCREEN_WIDTH / 2) - (frame->w / 2),
            (Main::SCREEN_HEIGHT / 2) - (frame->h / 2) - 8,
            frame->w,
            frame->h
    };
}

void ChatState::update() {
    timer.tick();

    if (!windowEnabled) {
        switch (initState) {
            default: break;

            case SERVER_PING: {
                printf("\rConnecting...");
                initState = SERVER_WAIT;
                pingServer();
            } break;

            case SERVER_WAIT: {

            } break;

            case SERVER_REPLY: {
                if (!online) {
                    printf("Server not online.\n\n");
                    exit(0);
                } else {
                    printf("OK\n");
                    printf("Enter a username: ");
                    initState = INPUT_USERNAME;
                }
            } break;

            case INPUT_USERNAME: {
                Input::enableKeyBuffer();
                std::string str = Input::popBuffer();
                if (!str.empty()) {
                    for (char c : str) {
                        if (isalnum(c)) {
                            username += c;
                            printf(std::string(1, c).c_str());
                        }
                    }
                }

                if (Input::getKeyUp(SDLK_RETURN)) {
                    printf("\n");
                    Input::disableKeyBuffer();
                    initState = CHECK_USERNAME;
                }

                if (Input::getKeyUp(SDLK_BACKSPACE)) {
                    if (!username.empty()) {
                        printf("\b \b");
                        username.pop_back();
                    }
                }
            } break;

            case CHECK_USERNAME: {
                initState = WAIT_USERNAME;
                Network::Request req = Network::Request("http://lab.spaghetti.rocks:8080/register", Network::POST,
                        [this](Network::Response resp) {
                    if (resp.status == Network::SUCCESS && resp.statusCode == 200) {
                        std::string respStr;
                        for (char b : *resp.data) {
                            respStr += b;
                        }
                        userId = respStr;
                        initState = DONE_USERNAME;
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
    } else {
        drawWindow();
//        if (closeWindow) {
//            closeWindow = false;
//            printf("\n");
//            SDL_SetCursor(SDL_GetDefaultCursor());
//            status = COMMAND_FINISHED;
//        }
    }
}

void ChatState::pingServer() {
    Network::Request("http://lab.spaghetti.rocks:8080/status", Network::GET, [this](Network::Response resp) {
        initState = SERVER_REPLY;
        printf("reply\n");
        if (resp.status == Network::SUCCESS && resp.statusCode == 200) {
            std::string respStr;

            for (char b : *resp.data) {
                respStr += b;
            }

            auto split = Util::splitString(respStr, ",");
            online = split[0] == "online";
        } else {
            online = false;
        }
    }).execute();
}

void ChatState::drawWindow() {
    frame->draw(nullptr, &frameRect);
    switch (state) {
        case MAIN_MENU: {
            mainMenuGui->draw();
        } break;
    }
}