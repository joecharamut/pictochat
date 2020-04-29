#ifndef PROJECT_MESSAGE_H
#define PROJECT_MESSAGE_H

#include <string>
#include <memory>
#include "../types/Texture.h"
#include "../types/Text.h"

class Message {
public:
    enum MessageType {
        JOIN_A, LEAVE_A,
        JOIN_B, LEAVE_B,
        JOIN_C, LEAVE_C,
        JOIN_D, LEAVE_D,
        USER_MESSAGE,
        WELCOME_MESSAGE,
        SYSTEM_MESSAGE
    };

    Message(MessageType type, std::string messageData, std::string messageUser = "", std::string messsageImage = "");
    ~Message();
    int getHeight();
    void draw(int x, int y);

private:
    MessageType type;
    std::string messageData;
    std::string messageUser;
    std::string messageImage;

    std::shared_ptr<Texture> bgTexture;
    std::shared_ptr<Text> systemText;
    std::shared_ptr<Text> joinText;
    std::shared_ptr<Text> leaveText;

    int userLines;
    std::shared_ptr<Text> userName;
    std::shared_ptr<Text> userLine1;
    std::shared_ptr<Text> userLine2;
    std::shared_ptr<Text> userLine3;
    std::shared_ptr<Text> userLine4;
    std::shared_ptr<Text> userLine5;

    std::shared_ptr<Texture> imageTexture;
};


#endif //PROJECT_MESSAGE_H
