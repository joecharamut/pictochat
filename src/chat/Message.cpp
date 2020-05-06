#include "Message.h"
#include "../ResourceManager.h"
#include "KeyboardGui.h"
#include "ImageUtil.h"

Message::Message(Message::MessageType type, std::string messageData, std::string messageUser, std::string messsageImage)
    : type(type), messageData(messageData), messageUser(messageUser), messageImage(messsageImage) {
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
            type == SYSTEM_MESSAGE ? ResourceManager::loadTexture("res/pictochat/messages/sysmessage.png") : nullptr);

    switch (type) {
        case WELCOME_MESSAGE: break;

        case SYSTEM_MESSAGE: {
            systemText = std::make_shared<Text>(messageData, std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16,
                    Text::Blended, COLOR(0xd6, 0xce, 0xc5));
        } break;

        case JOIN_A:
        case JOIN_B:
        case JOIN_C:
        case JOIN_D: {
            joinText = std::make_shared<Text>(messageData, std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16,
                    Text::Blended, COLOR(0xd6, 0xce, 0xc5));
        } break;

        case LEAVE_A:
        case LEAVE_B:
        case LEAVE_C:
        case LEAVE_D: {
            leaveText = std::make_shared<Text>(messageData, std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16,
                    Text::Blended, COLOR(0xd6, 0xce, 0xc5));
        } break;

        case USER_MESSAGE: {
            userName = std::make_shared<Text>(messageUser, std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16,
                    Text::Blended, COLOR(0x00, 0x00, 0x88));

            userLine1 = std::make_shared<Text>("", std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16,
                    Text::Blended, COLOR_BLACK);
            userLine2 = std::make_shared<Text>("", std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16,
                    Text::Blended, COLOR_BLACK);
            userLine3 = std::make_shared<Text>("", std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16,
                    Text::Blended, COLOR_BLACK);
            userLine4 = std::make_shared<Text>("", std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16,
                    Text::Blended, COLOR_BLACK);
            userLine5 = std::make_shared<Text>("", std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16,
                    Text::Blended, COLOR_BLACK);

            int line = 1;
            std::shared_ptr<Text> currentLine = userLine1;
            int currentWidth = 0;
            std::string currentLineText;

            for (char c : messageData) {
                currentWidth += currentLine->getCharWidth(c);

                if (currentWidth >= (line == 1 ? KeyboardGui::line1Width : KeyboardGui::lineWidth)) {
                    currentWidth = 0;
                    currentLine->setText(currentLineText);
                    currentLineText = "";

                    if (line == 1) currentLine = userLine2;
                    else if (line == 2) currentLine = userLine3;
                    else if (line == 3) currentLine = userLine4;
                    else currentLine = userLine5;

                    line++;

                    if (line == 6) break;
                }
                currentLineText += c;
            }
            currentLine->setText(currentLineText);

            int len = messageData.size();
            if (line == 1) {
                userLines = 1;
                bgTexture = ResourceManager::loadTexture("res/pictochat/messages/1line.png");
            } else if (line == 2) {
                userLines = 2;
                bgTexture = ResourceManager::loadTexture("res/pictochat/messages/2line.png");
            } else if (line == 3) {
                userLines = 3;
                bgTexture = ResourceManager::loadTexture("res/pictochat/messages/3line.png");
            } else if (line == 4) {
                userLines = 4;
                bgTexture = ResourceManager::loadTexture("res/pictochat/messages/4line.png");
            } else {
                userLines = 5;
                bgTexture = ResourceManager::loadTexture("res/pictochat/messages/5line.png");
            }

            if (!messsageImage.empty()) {
                userLines = 5;
                bgTexture = ResourceManager::loadTexture("res/pictochat/messages/5line.png");
                imageTexture = ImageUtil::decode(messageImage, 230, 80);
            }
        } break;
    }
}

Message::~Message() {

}

int Message::getHeight() {
    return bgTexture->h;
}

void Message::draw(int x, int y) {
    bgTexture->draw(x, y);

    if (systemText) {
        systemText->draw(x+6, y+4);
    }

    if (joinText) {
        joinText->draw(x+86, y+4);
    }

    if (leaveText) {
        leaveText->draw(x+79, y+4);
    }

    if (userName) {
        userName->draw(x+7, y+4);

        switch (userLines) {
            case 5:
                userLine5->draw(x+3, y+69);
            case 4:
                userLine4->draw(x+3, y+53);
            case 3:
                userLine3->draw(x+3, y+36);
            case 2:
                userLine2->draw(x+3, y+20);
            case 1:
                userLine1->draw(x+63, y+4);
        }
    }

    if (imageTexture) {
        imageTexture->draw(x+2, y+3);
    }
}
