#ifndef PROJECT_COMMANDCOMMENT_H
#define PROJECT_COMMANDCOMMENT_H

#include "Command.h"
#include "../../net/Request.h"
#include <nlohmann/json.hpp>

class CommandComment : public Command {
public:
    CommandComment(std::shared_ptr<Console> console, std::shared_ptr<VirtualFS> filesystem);
    ~CommandComment() override;
    void exec(std::vector<std::string> flags, std::vector<std::string> args) override;
    CommandStatus update() override;
    std::string help() override;

private:
    const std::string BASE_URL = "http://www.spaghetti.rocks/form.php";
    const std::string VIEW_URL = BASE_URL + "?view=1";


    CommandStatus status = COMMAND_FINISHED;
    bool loaded = false;
    bool printed = false;
    nlohmann::json loadedComments;

    void networkCallback(Network::Response response);
};


#endif //PROJECT_COMMANDCOMMENT_H
