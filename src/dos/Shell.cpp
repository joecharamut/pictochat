#include "Shell.h"
#include "../Util.h"
#include "command/CommandDir.h"

#include <vector>
#include <iostream>
#include <utility>

#define PL() printf("line %d\n", __LINE__)

Shell::Shell(std::shared_ptr<Console> p_console) {
    console = std::move(p_console);
    filesystem = std::make_shared<VirtualFS>();

    registerCommand("dir", std::static_pointer_cast<Command>(std::make_shared<CommandDir>(console, filesystem)));
}

Shell::~Shell() {

}

void Shell::registerCommand(const std::string &name, std::shared_ptr<Command> command) {
    if (commands.count(name) == 0) {
        commands[name] = std::move(command);
        return;
    }

    printf("command `%s` already registered", name.c_str());
}

std::shared_ptr<Command> Shell::processCommand(const std::string &commandStr) {
    printf("input string: `%s`\n", commandStr.c_str());

    std::string command;
    std::vector<std::string> flags;
    std::vector<std::string> args;
    int pos = 0;

    for (char c : commandStr) {
        if (c != '/' && c != ' ') {
            command += c;
            pos++;
        } else {
            break;
        }
    }

    auto split = Util::splitString(commandStr.substr(pos), " ");

    int skip = 0;
    for (const auto &part : split) {
        skip++;
        if (part.empty()) {
            continue;
        }

        if (part[0] == '/') {
            std::string tmp;
            for (char c : part.substr(1)) {
                if (c != '/') {
                    tmp += c;
                } else {
                    flags.push_back(tmp);
                    tmp = "";
                    continue;
                }
            }

            flags.push_back(tmp);
        } else {
            break;
        }
    }

    for (int i = skip-1; i < split.size(); i++) {
        if (split[i][0] != '/') {
            args.push_back(split[i]);
        }
    }

    if (commands.count(command) > 0) {
        std::shared_ptr<Command> commandPtr = commands[command];
        commandPtr->exec(flags, args);
        return commandPtr;
    } else {
        console->write("Bad command or file name\n\n");
        return nullptr;
    }
}
