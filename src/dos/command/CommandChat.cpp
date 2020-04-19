#include "CommandChat.h"
#include "../../ResourceManager.h"
#include "../../util/Util.h"
#include "../../Input.h"
#include "../../Graphics.h"
#include "../Shell.h"
#include "../../gui/GuiImage.h"
#include "../../gui/GuiButton.h"

Commands::CommandChat::CommandChat(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem)
        : Command(std::move(console), std::move(filesystem)) {

    frame = ResourceManager::loadTexture("res/pictochat/frame.png");

    frameRect = {
            (Main::SCREEN_WIDTH / 2) - (frame->w / 2),
            (Main::SCREEN_HEIGHT / 2) - (frame->h / 2) - 8,
            frame->w,
            frame->h
    };
}

Commands::CommandChat::~CommandChat() { }

void Commands::CommandChat::exec(std::vector<std::string> flags, std::vector<std::string> args) {
    status = COMMAND_RUNNING;
    initState = (InitState) 0;
    windowEnabled = false;
    username = "";
    timer.reset();
}

Command::CommandStatus Commands::CommandChat::update() {
    timer.tick();

    if (!windowEnabled) {
        switch (initState) {
            default: break;

            case SERVER_PING: {
                console->write("\rConnecting...");
                initState = SERVER_WAIT;
                pingServer();
            } break;

            case SERVER_WAIT: {

            } break;

            case SERVER_REPLY: {
                if (!online) {
                    console->write("Server not online.\n\n");
                    status = COMMAND_FINISHED;
                } else {
                    console->write("OK\n");
                    console->write("Enter a username: ");
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
                            console->write(std::string(1, c));
                        }
                    }
                }

                if (Input::getKeyUp(SDLK_RETURN)) {
                    console->write("\n");
                    Input::disableKeyBuffer();
                    initState = CHECK_USERNAME;
                }

                if (Input::getKeyUp(SDLK_BACKSPACE)) {
                    if (!username.empty()) {
                        console->write("\b \b");
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
                        console->write("Username unavailable\n");
                        status = COMMAND_FINISHED;
                    }
                });
                req.setPostData(username);
                req.execute();
            } break;

            case WAIT_USERNAME: {

            } break;

            case DONE_USERNAME: {
                console->write("Connected\n");
                mainMenuGui->setup(userId, this);
                windowEnabled = true;
            } break;
        }
    } else {
        drawWindow();
        if (closeWindow) {
            closeWindow = false;
            console->write("\n");
            SDL_SetCursor(SDL_GetDefaultCursor());
            status = COMMAND_FINISHED;
        }
    }

    return status;
}

std::string Commands::CommandChat::help() {
    return std::string();
}


void Commands::CommandChat::pingServer() {
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

void Commands::CommandChat::drawWindow() {
    frame->draw(nullptr, &frameRect);
    switch (chatState) {
        case MAIN_MENU: {
            mainMenuGui->draw();
        } break;
    }
}
