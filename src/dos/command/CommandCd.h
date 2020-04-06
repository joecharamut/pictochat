#ifndef PROJECT_COMMANDCD_H
#define PROJECT_COMMANDCD_H

#include "Command.h"

namespace Commands {

    class CommandCd : public Command {
    public:
        CommandCd(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem);
        ~CommandCd() override;
        void exec(std::vector<std::string> flags, std::vector<std::string> args) override;
        CommandStatus update() override;
        std::string help() override;
    };

}

#endif //PROJECT_COMMANDCD_H
