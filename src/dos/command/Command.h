#include <utility>

#ifndef PROJECT_COMMAND_H
#define PROJECT_COMMAND_H

#include <string>
#include <vector>
#include <memory>
#include "../Console.h"
#include "../VirtualFS.h"

class Command {
public:
    enum CommandStatus {
        COMMAND_RUNNING,
        COMMAND_FINISHED
    };

    Command(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem)
        : console(std::move(console)), filesystem(std::move(filesystem)) {};
    virtual ~Command() = 0;
    virtual void exec(std::vector<std::string> flags, std::vector<std::string> args) = 0;
    virtual CommandStatus update() = 0;
    virtual std::string help() = 0;

    std::shared_ptr<Console> console;
    std::shared_ptr<VirtualFS> filesystem;
};


#endif //PROJECT_COMMAND_H
