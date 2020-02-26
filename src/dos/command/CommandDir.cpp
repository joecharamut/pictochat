#include <utility>

#include "CommandDir.h"
#include "../../Version.h"

CommandDir::CommandDir(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem)
    : Command(std::move(console), std::move(filesystem)) {

}

CommandDir::~CommandDir() = default;

void CommandDir::exec(std::vector<std::string> flags, std::vector<std::string> args) {
    console->write("\n");
    console->write(" Volume in drive C is PISS_ASS\n");
    console->write(" Volume Serial Number is " + Version::GIT_SHA1_SHORT + "\n");
    console->write(" Directory of C:\\\n");
    console->write("\n\n");

    std::shared_ptr<FileNode> node = filesystem->pathToNode("C:\\");
    int files = 0;
    int bytes = 0;
    if (node) {
        for (const auto &child : node->children) {
            files++;
            bytes += child->value.bytes;
            console->write(child->value.displayName + "\t\t" + child->value.displayExtension+ "\n");
        }
    }

    console->write("\t\t" + std::to_string(files) + " File(s)\t\t" + std::to_string(bytes) + " bytes\n\n");
}

Command::CommandStatus CommandDir::update() {
    return COMMAND_FINISHED;
}

std::string CommandDir::help() {
    return std::string();
}
