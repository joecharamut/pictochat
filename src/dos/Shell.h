#ifndef PROJECT_SHELL_H
#define PROJECT_SHELL_H

#include <string>
#include <map>
#include <memory>
#include "command/Command.h"
#include "Console.h"
#include "VirtualFS.h"

class Shell {
public:
    std::shared_ptr<Console> console;
    std::shared_ptr<VirtualFS> filesystem;

    explicit Shell(std::shared_ptr<Console> p_console);
    ~Shell();
    std::shared_ptr<Command> processCommand(const std::string& commandStr);

private:
    enum CommandType {
        BUILTIN,
        GLOBAL,
        CONTEXT_REQUIRED
    };

    std::map<std::string, std::shared_ptr<Command>> builtins;
    std::map<std::string, std::shared_ptr<Command>> globalCommands;
    std::map<std::string, std::shared_ptr<Command>> contextCommands;

    void registerCommand(const std::string &name, const std::shared_ptr<Command> &command, CommandType type,
            const std::string &path);
};


#endif //PROJECT_SHELL_H
