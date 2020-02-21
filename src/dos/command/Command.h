#include <utility>

#ifndef PROJECT_COMMAND_H
#define PROJECT_COMMAND_H

#include <string>
#include <vector>
#include <memory>
#include "../Console.h"

class Command {
public:
    explicit Command(std::shared_ptr<Console> console) : console(std::move(console)) {};
    virtual ~Command() = 0;
    virtual void exec(std::vector<std::string> flags, std::vector<std::string> args) = 0;
    virtual std::string help() = 0;

private:
    std::shared_ptr<Console> console;
};


#endif //PROJECT_COMMAND_H
