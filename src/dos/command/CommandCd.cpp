#include "CommandCd.h"

#include <utility>

#include "../../util/Version.h"
#include "../../util/Util.h"
#include "../VirtualFS.h"

CommandCd::CommandCd(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem)
        : Command(std::move(console), std::move(filesystem)) {

}

CommandCd::~CommandCd() = default;

void CommandCd::exec(std::vector<std::string> flags, std::vector<std::string> args) {
    std::string subdir;
    if (!args.empty()) {
        subdir = Util::toUpperCase(args[0]);
    } else {
        console->write(filesystem->getcwd() + "\\");
        console->write("\n\n");
        return;
    }

    std::shared_ptr<FileNode> node = filesystem->pathToNode(filesystem->getcwd() + "\\" + subdir);
    if (node && node->value.extension == "<DIR>") {
        filesystem->chdir(node);
    } else {
        console->write("Invalid directory\n\n");
    }
}

Command::CommandStatus CommandCd::update() {
    return COMMAND_FINISHED;
}

std::string CommandCd::help() {
    return std::string();
}