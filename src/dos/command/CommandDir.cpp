#include <utility>

#include "CommandDir.h"

CommandDir::CommandDir(std::shared_ptr<Console> console) : Command(std::move(console)) {

}

CommandDir::~CommandDir() {

}

void CommandDir::exec(std::vector<std::string> flags, std::vector<std::string> args) {

}

Command::CommandStatus CommandDir::update() {
    return COMMAND_FINISHED;
}

std::string CommandDir::help() {
    return std::string();
}
