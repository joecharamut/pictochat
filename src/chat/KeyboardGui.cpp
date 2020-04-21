#include "KeyboardGui.h"

#include "GuiMacros.h"
#include "../ResourceManager.h"
#include "../Input.h"

KeyboardGui::KeyboardGui() {
    handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    keyboardImage = GUI_IMAGE(LoadTexture("res/pictochat/keyboard/keyboard.png"), 0, 192);
    nameText = GUI_DS_TEXT("", COLOR(0x00, 0x00, 0x88), 28, 192+18);

    line1Text = GUI_DS_TEXT("", COLOR_BLACK, 83, 192+18);
    line2Text = GUI_DS_TEXT("", COLOR_BLACK, 24, 192+35);
    line3Text = GUI_DS_TEXT("", COLOR_BLACK, 24, 192+51);
    line4Text = GUI_DS_TEXT("", COLOR_BLACK, 24, 192+67);
    line5Text = GUI_DS_TEXT("", COLOR_BLACK, 24, 192+83);

    closeButton = GUI_BUTTON(LoadTexture("res/pictochat/closebutton.png"), 245, 192+2, COLOR(0xaa, 0xaa, 0xaa),
            [this](bool click) {
        showClick = true;
        messageText = "";
        instance->exitRoom();
    });

    sendButton = GUI_BUTTON(LoadTexture("res/pictochat/sendbutton.png"), 226, 192+105, COLOR(0xaa, 0xaa, 0xaa),
            [this](bool click) {
        showClick = true;
        if (click) {
            sendMessage();
        }
    });

    clearButton = GUI_BUTTON(LoadTexture("res/pictochat/clearbutton.png"), 226, 192+160, COLOR(0xaa, 0xaa, 0xaa),
            [this](bool click) {
        showClick = true;
        if (click) {
            messageText = "";
        }
    });
}

KeyboardGui::~KeyboardGui() {
    SDL_FreeCursor(handCursor);
}

void KeyboardGui::setup(ChatState *instance) {
    this->instance = instance;
    nameText->text->setText(instance->username);
}

void KeyboardGui::draw() {
    showClick = false;

    pollBuffer();
    updateTextLines();

    keyboardImage->draw();
    nameText->draw();
    line1Text->draw();
    line2Text->draw();
    line3Text->draw();
    line4Text->draw();
    line5Text->draw();

    sendButton->draw();
    clearButton->draw();
//    closeButton->draw();

    SDL_SetCursor(showClick ? handCursor : SDL_GetDefaultCursor());
}

void KeyboardGui::sendMessage() {
    if (messageText.empty()) return;

    instance->socket->send(((nlohmann::json) {
            {"action", "message"},
            {"room", instance->room},
            {"message", {{"type", (int) Message::USER_MESSAGE}, {"data", messageText}}}
    }).dump());

    messageText = "";
}

void KeyboardGui::pollBuffer() {
    Input::enableKeyBuffer();
    std::string str = Input::popBuffer();
    if (!str.empty()) {
        for (char c : str) {
            if (messageText.size() < messageLimit) {
                if (c >= 0x20 && c < 0x7f) {
                    messageText += c;
                }
            }
        }
    }

    if (Input::getKeyUp(SDLK_RETURN)) {
        sendMessage();
    }

    if (Input::getKeyDown(SDLK_BACKSPACE)) {
        if (!messageText.empty()) {
            messageText.pop_back();
        }
    }
}

void KeyboardGui::updateTextLines() {
    std::string copy = messageText;
    copy.append(messageLimit - copy.length(), ' ');

    line1Text->text->setText(copy.substr(0, line1Limit));
    line2Text->text->setText(copy.substr(line1Limit, lineLimit));
    line3Text->text->setText(copy.substr(line2Limit, lineLimit));
    line4Text->text->setText(copy.substr(line3Limit, lineLimit));
    line5Text->text->setText(copy.substr(line4Limit, lineLimit));
}
