#include "Message.h"
#include "../ResourceManager.h"

Message::Message(Message::MessageType type, std::string messageData) : type(type), messageData(messageData) {
    bgTexture = (
            type == JOIN_A ? ResourceManager::loadTexture("res/pictochat/messages/joina.png") :
            type == JOIN_B ? ResourceManager::loadTexture("res/pictochat/messages/joinb.png") :
            type == JOIN_C ? ResourceManager::loadTexture("res/pictochat/messages/joinc.png") :
            type == JOIN_D ? ResourceManager::loadTexture("res/pictochat/messages/joind.png") :
            type == LEAVE_A ? ResourceManager::loadTexture("res/pictochat/messages/leavea.png") :
            type == LEAVE_B ? ResourceManager::loadTexture("res/pictochat/messages/leaveb.png") :
            type == LEAVE_C ? ResourceManager::loadTexture("res/pictochat/messages/leavec.png") :
            type == LEAVE_D ? ResourceManager::loadTexture("res/pictochat/messages/leaved.png") :
            type == WELCOME_MESSAGE ? ResourceManager::loadTexture("res/pictochat/messages/welcome.png") :
            type == SYSTEM_MESSAGE ? ResourceManager::loadTexture("res/pictochat/messages/sysmessage.png") :
            ResourceManager::loadTexture("res/pictochat/messages/1line.png")
    );

    if (type == SYSTEM_MESSAGE) {
        systemText = std::make_shared<Text>(messageData, std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16,
                Text::Blended, COLOR(0xd6, 0xce, 0xc5));
    }
}

Message::~Message() {

}

int Message::getHeight() {
    switch (type) {
        case JOIN_A:
        case JOIN_B:
        case JOIN_C:
        case JOIN_D:
        case LEAVE_A:
        case LEAVE_B:
        case LEAVE_C:
        case LEAVE_D:
        case WELCOME_MESSAGE:
        case SYSTEM_MESSAGE:
        {
            return 22;
        } break;

        case USER_MESSAGE: {
            return 22; // todo fix 1-5 height
        } break;
    }
}

void Message::draw(int x, int y) {
    bgTexture->draw(x, y);
    if (type == SYSTEM_MESSAGE) {
        systemText->draw(x+6, y+4);
    }
}
