#include "Shell.h"
#include "../util/Util.h"

#include "command/CommandDir.h"
#include "command/CommandCd.h"
#include "command/CommandCurl.h"
#include "command/CommandComment.h"

#include <vector>
#include <iostream>
#include <utility>
#include <random>

#define PL() printf("%s:%d\n", __FILE__, __LINE__)

Shell::Shell(std::shared_ptr<Console> p_console) {
    console = std::move(p_console);
    filesystem = std::make_shared<VirtualFS>();

    // register builtins
    registerCommand("dir", std::static_pointer_cast<Command>(std::make_shared<CommandDir>(console, filesystem)));
    registerCommand("cd", std::static_pointer_cast<Command>(std::make_shared<CommandCd>(console, filesystem)));

    // register c:\dos global commands
    registerCommand("curl", std::static_pointer_cast<Command>(
            std::make_shared<CommandCurl>(console, filesystem)), GLOBAL);
    registerCommand("comment", std::static_pointer_cast<Command>(
            std::make_shared<CommandComment>(console, filesystem)), GLOBAL);
}

Shell::~Shell() {

}

void Shell::registerCommand(const std::string &name, const std::shared_ptr<Command> &command, CommandType type,
        const std::string &path) {

    switch (type) {
        case BUILTIN: {
            if (builtins.count(name) == 0) {
                builtins[name] = command;
            } else {
                printf("builtin `%s` already registered", name.c_str());
            }
        } break;

        case GLOBAL: {
            if (globalCommands.count(name) == 0) {
                globalCommands[name] = command;
                filesystem->addFile("C:\\DOS", StatData(name, "COM", rand() % 8192));
            } else {
                printf("global command `%s` already registered", name.c_str());
            }
        } break;

        case CONTEXT_REQUIRED: {
            std::string fullPath = path + "\\" + name + ".COM";
            if (contextCommands.count(fullPath) == 0) {
                contextCommands[fullPath] = command;
                filesystem->addFile(path, StatData(name, "COM", rand() % 8192));
            } else {
                printf("context command `%s` already registered", name.c_str());
            }
        } break;
    }
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

    std::string commandWithPath = filesystem->getcwd() + "\\" + command + (Util::stringEndsWith(command, ".COM") ? "" : ".COM");
    std::shared_ptr<Command> commandPtr = nullptr;

    if (builtins.count(command) > 0) {
        commandPtr = builtins[command];
    } else if (globalCommands.count(command) > 0) {
        commandPtr = globalCommands[command];
    } else if (contextCommands.count(commandWithPath) > 0) {
        commandPtr = globalCommands[command];
    }

    if (commandPtr) {
        commandPtr->exec(flags, args);
        return commandPtr;
    } else {
        console->write("Bad command or file name\n\n\n");
        return nullptr;
    }
}
