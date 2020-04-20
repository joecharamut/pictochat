#include "TopScreenGui.h"

#include "GuiMacros.h"
#include "../ResourceManager.h"

TopScreenGui::TopScreenGui() {
    topScreenBackground = GUI_IMAGE(LoadTexture("res/pictochat/tsmenu.png"), 0, 0);
    signalRedBadge = GUI_IMAGE(LoadTexture("res/pictochat/signalred.png"), 1, 1);
    signalGreenBadge = GUI_IMAGE(LoadTexture("res/pictochat/signalgreen.png"), 1, 1);
}

TopScreenGui::~TopScreenGui() {

}

void TopScreenGui::setup(ChatState *instance) {
    this->instance = instance;
}

void TopScreenGui::pushMessage(const Message &message) {
    messages.insert(messages.begin(), message);
}

void TopScreenGui::draw() {
    topScreenBackground->draw();
    (instance->online ? signalGreenBadge : signalRedBadge)->draw();

    int messageY = 191;
    for (auto message : messages) {
        messageY -= message.getHeight();
        if (messageY > (-message.getHeight())) {
            message.draw(21, messageY);
        }
    }
}
