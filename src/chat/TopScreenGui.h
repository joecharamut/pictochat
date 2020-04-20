#ifndef PROJECT_TOPSCREENGUI_H
#define PROJECT_TOPSCREENGUI_H

#include <memory>
#include <string>
#include "../gui/GuiImage.h"
#include "ChatState.h"
#include "Message.h"

class ChatState;

class TopScreenGui {
public:
    TopScreenGui();
    ~TopScreenGui();
    void setup(ChatState *instance);
    void draw();
    void pushMessage(const Message &message);

private:
    const int messageArea = 190;
    std::vector<Message> messages;

    std::shared_ptr<GuiImage> topScreenBackground;
    std::shared_ptr<GuiImage> signalRedBadge;
    std::shared_ptr<GuiImage> signalGreenBadge;

    ChatState *instance = nullptr;
};


#endif //PROJECT_TOPSCREENGUI_H
