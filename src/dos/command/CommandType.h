#ifndef PROJECT_COMMANDTYPE_H
#define PROJECT_COMMANDTYPE_H

#include "Command.h"

namespace Commands {

    class CommandType : public Command {
    public:
        CommandType(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem);
        ~CommandType() override;
        void exec(std::vector<std::string> flags, std::vector<std::string> args) override;
        CommandStatus update() override;
        std::string help() override;
    };

}

#endif //PROJECT_COMMANDTYPE_H
