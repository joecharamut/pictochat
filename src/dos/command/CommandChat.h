#ifndef PROJECT_COMMANDCHAT_H
#define PROJECT_COMMANDCHAT_H

#include "Command.h"
#include "../../Main.h"
#include "../../types/Texture.h"
#include "../../types/Text.h"
#include "../../chat/MainMenuGui.h"
#include "../../types/Timer.h"

#include <SDL2/SDL.h>

class MainMenuGui;

namespace Commands {

    class CommandChat : public Command {
    public:
        CommandChat(std::shared_ptr<Console> console, std::shared_ptr <VirtualFS> filesystem);
        ~CommandChat() override;
        void exec(std::vector <std::string> flags, std::vector <std::string> args) override;
        CommandStatus update() override;
        std::string help() override;

        bool closeWindow = false;
    private:
        CommandStatus status = COMMAND_RUNNING;

        enum ChatState {
            MAIN_MENU
        };
        ChatState chatState = MAIN_MENU;

        enum InitState {
            SERVER_PING, SERVER_WAIT, SERVER_REPLY,
            INPUT_USERNAME, CHECK_USERNAME, WAIT_USERNAME, DONE_USERNAME
        };
        InitState initState = SERVER_PING;

        void drawWindow();
        void pingServer();

        Timer timer = Timer();

        bool windowEnabled = false;
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
    };

}

#endif //PROJECT_COMMANDCHAT_H
