#ifndef PROJECT_COMMANDDIR_H
#define PROJECT_COMMANDDIR_H


#include "Command.h"

class CommandDir : public Command {
public:
    CommandDir(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem);
    ~CommandDir() override;
    void exec(std::vector<std::string> flags, std::vector<std::string> args) override;
    CommandStatus update() override;
    std::string help() override;
};


#endif //PROJECT_COMMANDDIR_H