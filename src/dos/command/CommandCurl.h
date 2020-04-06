#ifndef PROJECT_COMMANDCURL_H
#define PROJECT_COMMANDCURL_H

#include "Command.h"
#include "../../net/Request.h"

namespace Commands {

    class CommandCurl : public Command {
    public:
        CommandCurl(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem);
        ~CommandCurl() override;
        void exec(std::vector<std::string> flags, std::vector<std::string> args) override;
        CommandStatus update() override;
        std::string help() override;
        void requestCallback(Network::Response response);

    private:
        CommandStatus status = COMMAND_FINISHED;
    };

}

#endif //PROJECT_COMMANDCURL_H
