#include "CommandCurl.h"

#include <regex>

CommandCurl::CommandCurl(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem)
        : Command(std::move(console), std::move(filesystem)) {

}

CommandCurl::~CommandCurl() = default;

void CommandCurl::exec(std::vector<std::string> flags, std::vector<std::string> args) {
    if (args.empty()) {
        // invalid args
        console->write("No URL Specified\n\n");
        return;
    }

    if (!std::regex_match(args[0],
            std::regex(R"(^((http[s]?|ftp):\/)?\/?([^:\/\s]+)((\/\w+)*\/)([\w\-\.]+[^#?\s]+)(.*)?(#[\w\-]+)?$)"))
    ) {
        // invalid url
        console->write("Invalid URL\n\n");
        return;
    }

    status = COMMAND_RUNNING;

    std::function<void(Network::Response)> callback(std::bind(&CommandCurl::requestCallback, this, std::placeholders::_1));
    Network::Request request = Network::Request(args[0], Network::Method::GET, callback);
    request.execute();
}

Command::CommandStatus CommandCurl::update() {
    return status;
}

std::string CommandCurl::help() {
    return std::string();
}

void CommandCurl::requestCallback(Network::Response response) {
    if (response.status == Network::Status::SUCCESS) {
        printf("resp: %lld bytes\n", response.size);
        std::string dataStr;
        for (int i = 0; i < response.size; i++) {
            Network::byte b = response.data.get()->at(i);
//        printf("byte: %x\n", b);
            dataStr += b;
        }
        console->write(dataStr);
    } else {
        console->write("request failed: " + response.statusText + "\n");
    }

    console->write("\n\n");
    status = COMMAND_FINISHED;
}
