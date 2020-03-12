#ifndef PROJECT_COMMANDCOMMENT_H
#define PROJECT_COMMANDCOMMENT_H

#include "Command.h"
#include "../../net/Request.h"
#include "../../types/Sound.h"
#include <nlohmann/json.hpp>
#include <array>

class CommandComment : public Command {
public:
    CommandComment(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem);
    ~CommandComment() override;
    void exec(std::vector<std::string> flags, std::vector<std::string> args) override;
    CommandStatus update() override;
    std::string help() override;

private:
    enum LocalState {
        STATE_INIT,
        STATE_DIAL,
        STATE_DIAL_WAIT,
        STATE_PRINT
    };

    const std::string BASE_URL = "http://www.spaghetti.rocks/form.php";
    const std::string VIEW_URL = BASE_URL + "?view=1";
    std::string SEPARATOR;

    LocalState state = STATE_INIT;

    CommandStatus status = COMMAND_FINISHED;
    bool loaded = false;
    bool printed = false;
    nlohmann::json loadedComments;

    int dialProgress = 0;

    std::array<std::shared_ptr<Sound>, 10> DTMF_TONES;

    void networkCallback(Network::Response response);
};


#endif //PROJECT_COMMANDCOMMENT_H
