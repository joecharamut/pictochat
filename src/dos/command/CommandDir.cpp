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

    console->write("DOS\t\t<DIR>\n");
    console->write("\t\t1 File(s)\t\t0 bytes\n\n");
}

Command::CommandStatus CommandDir::update() {
    return COMMAND_FINISHED;
}

std::string CommandDir::help() {
    return std::string();
}
