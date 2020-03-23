#include "CommandComment.h"
#include "../../util/Util.h"
#include "../../ResourceManager.h"

CommandComment::CommandComment(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem)
        : Command(std::move(console), std::move(filesystem)) {
    std::string separator;
    for (int i = 0; i < Console::WIDTH; i++) {
        separator += "=";
    }
    separator += "\n";
    SEPARATOR = separator;

    DTMF_TONES[0] = ResourceManager::loadSound("res/dtmf/0.ogg");
    DTMF_TONES[1] = ResourceManager::loadSound("res/dtmf/1.ogg");
    DTMF_TONES[2] = ResourceManager::loadSound("res/dtmf/2.ogg");
    DTMF_TONES[3] = ResourceManager::loadSound("res/dtmf/3.ogg");
    DTMF_TONES[4] = ResourceManager::loadSound("res/dtmf/4.ogg");
    DTMF_TONES[5] = ResourceManager::loadSound("res/dtmf/5.ogg");
    DTMF_TONES[6] = ResourceManager::loadSound("res/dtmf/6.ogg");
    DTMF_TONES[7] = ResourceManager::loadSound("res/dtmf/7.ogg");
    DTMF_TONES[8] = ResourceManager::loadSound("res/dtmf/8.ogg");
    DTMF_TONES[9] = ResourceManager::loadSound("res/dtmf/9.ogg");
}

CommandComment::~CommandComment() = default;

void CommandComment::exec(std::vector<std::string> flags, std::vector<std::string> args) {
    status = COMMAND_RUNNING;
}

Command::CommandStatus CommandComment::update() {
    switch (state) {
        case STATE_INIT: {
            console->write(SEPARATOR);
            console->write(Util::centerString(console->WIDTH,
                    "spaghetti.rocks Interconnected Network Message Board Software"));
            console->write(SEPARATOR);
            console->write("\n");

            state = STATE_DIAL;
        } break;

        case STATE_DIAL: {
            if (console->bufferEmpty()) {
                if (dialProgress <= 10) {
                    console->write("\rDialing [");
                    console->flush();
                    for (int i = 0; i < 10; i++) {
                        if (i < dialProgress) {
                            console->write("*");
                        } else {
                            console->write("-");
                        }
                    }
                    console->write("]");
                } else {
                    if (console->bufferEmpty()) {
                        console->write("\nConnecting...\n\n");
                        dialProgress = 0;
                        state = STATE_DIAL_WAIT;
                    }
                }

                if (dialProgress <= 11) {
                    if (dialProgress != 1) {
                        DTMF_TONES[rand() % 10]->play();
                        SDL_Delay(250);
                    }
                }

                dialProgress++;
            }
        } break;

        case STATE_DIAL_WAIT: {
            dialProgress++;

            if (dialProgress == 60) {
                std::function<void(Network::Response)> callback(std::bind(&CommandComment::networkCallback,
                        this, std::placeholders::_1));
                Network::Request request = Network::Request(VIEW_URL, Network::Method::GET, callback);
                request.execute();
            }

            if (dialProgress > 240) {
                if (loaded) {
                    state = STATE_PRINT;
                }
            }
        } break;

        case STATE_PRINT: {
            // print data
            for (int i = loadedComments.size() - 1; i >= 0; i--) {
                auto entry = loadedComments[i];
                std::string name = entry["name"];
                std::string comment = entry["comment"];
                std::time_t time_t = entry["timestamp"];
                std::tm time = *std::localtime(&time_t);
                std::string strTime = std::to_string(time.tm_year + 1900) + "-" + std::to_string(time.tm_mon + 1) +
                                      "-" + std::to_string(time.tm_mday);


                console->write(SEPARATOR);
                console->write(name);
                console->write(" : ");
                console->write(strTime);
                console->write("\n");
                console->write(comment);
                console->write("\n");
            }

            printed = true;
            console->write("\n\n");
            status = COMMAND_FINISHED;
        }
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