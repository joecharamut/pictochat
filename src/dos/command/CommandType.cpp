#include "CommandType.h"

Commands::CommandType::CommandType(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem)
        : Command(std::move(console), std::move(filesystem)) {

}

Commands::CommandType::~CommandType() = default;

void Commands::CommandType::exec(std::vector<std::string> flags, std::vector<std::string> args) {
    
}

Command::CommandStatus Commands::CommandType::update() {
    return COMMAND_FINISHED;
}

std::string Commands::CommandType::help() {
    return std::string();
}