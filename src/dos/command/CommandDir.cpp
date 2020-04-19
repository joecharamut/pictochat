#include <utility>

#include "CommandDir.h"
#include "../../util/Version.h"
#include "../../util/Util.h"
#include "../VirtualFS.h"

Commands::CommandDir::CommandDir(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem)
    : Command(std::move(console), std::move(filesystem)) {

}

Commands::CommandDir::~CommandDir() = default;

void Commands::CommandDir::exec(std::vector<std::string> flags, std::vector<std::string> args) {
    console->write("\n");
    console->write(" Volume in drive C is PISS\n");
    console->write(" Volume Serial Number is " + Version::GIT_SHA1_SHORT + "\n");

    std::string subdir;
    if (!args.empty()) subdir = Util::toUpperCase(args[0]);

    std::shared_ptr<FileNode> node = filesystem->pathToNode(filesystem->getcwd() + "\\" + subdir);
    if (node) {
        int files = 0;
        int bytes = 0;

        console->write(" Directory of " + filesystem->getcwd() + "\\" + subdir + "\n");
        console->write("\n\n");

        for (const auto &child : node->children) {
            files++;
            bytes += child->value.bytes;
            console->write(child->value.displayName + " " +
                (child->value.extension == "<DIR>" ? "   " : child->value.displayExtension)+ " " +
                (child->value.extension == "<DIR>" ? "<DIR>" : "     ") + "  " +
                std::to_string(child->value.bytes) + "\n");
        }

        console->write("\t\t" + std::to_string(files) + " File(s)\t\t" + std::to_string(bytes) + " bytes\n\n");
    } else {
        console->write("\n\nFile not found\n\n");
    }
}

Command::CommandStatus Commands::CommandDir::update() {
    return COMMAND_FINISHED;
}

std::string Commands::CommandDir::help() {
    return std::string();
}
