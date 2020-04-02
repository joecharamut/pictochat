#include "CommandType.h"

CommandType::CommandType(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem)
        : Command(std::move(console), std::move(filesystem)) {

}

CommandType::~CommandType() = default;

void CommandType::exec(std::vector<std::string> flags, std::vector<std::string> args) {
    
}

Command::CommandStatus CommandType::update() {
    return COMMAND_FINISHED;
}

std::string CommandType::help() {
    return std::string();
}