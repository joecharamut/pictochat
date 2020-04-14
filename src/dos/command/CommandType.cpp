#include "CommandType.h"
#include "../../fs/Filesystem.h"

Commands::CommandType::CommandType(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem)
        : Command(std::move(console), std::move(filesystem)) {

}

Commands::CommandType::~CommandType() = default;

void Commands::CommandType::exec(std::vector<std::string> flags, std::vector<std::string> args) {
    if (args.empty()) {
        console->write("Required parameter missing\n\n");
        return;
    }

    auto fileNode = filesystem->pathToNode(args[0]);
    if (!fileNode) {
        console->write("File not found\n\n");
        return;
    }

    if (!fileNode->value.realFile) {
        console->write("Invalid file type\n\n");
        return;
    }

    auto realFile = Filesystem::openFile(filesystem->nodeToPath(fileNode));
    if (!realFile) {
        console->write("unable to open file for some reason\n\n");
        return;
    }
    console->write(*realFile->read());
    console->write("\n");
}

Command::CommandStatus Commands::CommandType::update() {
    return COMMAND_FINISHED;
}

std::string Commands::CommandType::help() {
    return std::string();
}