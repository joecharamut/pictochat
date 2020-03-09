#include "CommandComment.h"

CommandComment::CommandComment(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem)
        : Command(std::move(console), std::move(filesystem)) {

}

CommandComment::~CommandComment() = default;

void CommandComment::exec(std::vector<std::string> flags, std::vector<std::string> args) {
    std::function<void(Network::Response)> callback(std::bind(&CommandComment::networkCallback,this, std::placeholders::_1));
    Network::Request request = Network::Request(VIEW_URL, Network::Method::GET, callback);
    request.execute();
    status = COMMAND_RUNNING;
}

Command::CommandStatus CommandComment::update() {
    if (loaded && !printed) {
        // print data

        std::string separator;
        for (int i = 0; i < Console::WIDTH; i++) {
            separator += "=";
        }
        separator += "\n";

        for (int i = loadedComments.size() - 1; i >= 0; i--) {
            auto entry = loadedComments[i];
            std::string name = entry["name"];
            std::string comment = entry["comment"];
            std::time_t time_t = entry["timestamp"];
            std::tm time = *std::localtime(&time_t);
            std::string strTime = std::to_string(time.tm_year + 1900) + "-" + std::to_string(time.tm_mon + 1) +
                    "-" + std::to_string(time.tm_mday);


            std::string str = separator + name + " : " + strTime + "\n" + comment;
            console->write(str);
            console->write("\n");
        }

        printed = true;
        console->write("\n\n");
        status = COMMAND_FINISHED;
    }

    return status;
}

std::string CommandComment::help() {
    return std::string();
}

void CommandComment::networkCallback(Network::Response response) {
    if (response.status == Network::Status::SUCCESS) {
        std::string dataStr;
        for (int i = 0; i < response.size; i++) {
            Network::byte b = response.data.get()->at(i);
            dataStr += b;
        }
        loadedComments = nlohmann::json::parse(dataStr);
        loaded = true;
    } else {
        console->write("network error: " + response.statusText + "\n");
        console->write("\n\n");
        status = COMMAND_FINISHED;
    }

}