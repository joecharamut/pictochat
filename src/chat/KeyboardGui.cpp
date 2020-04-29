#include "KeyboardGui.h"

#include "GuiMacros.h"
#include "../ResourceManager.h"
#include "../Input.h"
#include "../util/Base64.h"
#include "../Graphics.h"
#include "ImageUtil.h"

KeyboardGui::KeyboardGui() {
    drawTexture = std::make_shared<Texture>(Graphics::createTexture(drawWidth, drawHeight));

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
            clearDraw();
        }
    });
}

KeyboardGui::~KeyboardGui() {

}

void KeyboardGui::setup(ChatState *instance) {
    this->instance = instance;
    nameText->text->setText(instance->username);

    clearDraw();
}

void KeyboardGui::draw() {
    showClick = false;

    pollBuffer();
    updateTextLines();
    updateDraw();

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

    drawTexture->draw(23, 17+192);

    if (showClick) {
        Graphics::setCurosr(Graphics::Hand);
    } else {
        Graphics::setCurosr(Graphics::Default);
    }
}

void KeyboardGui::sendMessage() {
    if (messageText.empty() && surfaceClear) return;

    instance->socket->send(((nlohmann::json) {
            {"action", "message"},
            {"room", instance->room},
            {"message", {
                {"type", (int) Message::USER_MESSAGE},
                {"data", messageText},
                {"image", (surfaceClear ? "" : getSurfaceData())}
            }}
    }).dump());

    messageText = "";
    clearDraw();
}

void KeyboardGui::pollBuffer() {
    Input::enableKeyBuffer();
    std::string str = Input::popBuffer();
    if (!str.empty()) {
        for (char c : str) {
            if (messageText.size() < messageLimit) {
                // printable ascii
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

void KeyboardGui::updateDraw() {
    int mx, my;
    int buttons = SDL_GetMouseState(&mx, &my);
    bool leftClick = (buttons & SDL_BUTTON_LMASK) != 0;

    int baseX = 23;
    int baseY = 17+192;

    bool insideElement =
            mx > baseX &&
            my > baseY &&
            mx < baseX + 230 &&
            my < baseY + 80;

    bool insideName =
            mx > baseX + 0   &&
            my > baseY + 0   &&
            mx < baseX + 60  &&
            my < baseY + 19;

    bool topRightCorner =
            mx > baseX + 230 &&
            my > baseY + 15  &&
            mx < baseX + 234 &&
            my < baseY + 19;

    bool bottomLeftCorner =
            mx > baseX + 0   &&
            my > baseY + 81  &&
            mx < baseX + 4   &&
            my < baseY + 85;

    bool bottomRightCorner =
            mx > baseX + 230 &&
            my > baseY + 81  &&
            mx < baseX + 234 &&
            my < baseY + 85;

    bool hasLastMouse = (lastMouseX != -1 && lastMouseY != -1);

    if (insideElement) {
        showClick = true;
    }

    bool canDraw = insideElement && !insideName && !topRightCorner && !bottomLeftCorner && !bottomRightCorner;

    if (canDraw && leftClick) {
        int thisMouseX = mx - baseX;
        int thisMouseY = my - baseY;

        if (hasLastMouse && !(thisMouseX == lastMouseX && thisMouseY == lastMouseY)) {
//            drawLine(lastMouseX, lastMouseY, thisMouseX, thisMouseY);
            stroke(lastMouseX, lastMouseY, thisMouseX, thisMouseY);
        } else {
//            drawPixel(thisMouseX, thisMouseY);
            stroke(thisMouseX, thisMouseY);
        }

        lastMouseX = thisMouseX;
        lastMouseY = thisMouseY;
    }

    if ((!canDraw || !leftClick) && hasLastMouse) {
        // finish stroke

        // clip pos
        mx = std::max(mx, baseX);
        mx = std::min(mx, baseX + 230);

        my = std::max(my, baseY);
        my = std::min(my, baseY + 80);

        // todo fix name clipping

        int thisMouseX = mx - baseX;
        int thisMouseY = my - baseY;

//        drawLine(lastMouseX, lastMouseY, thisMouseX, thisMouseY);
        stroke(lastMouseX, lastMouseY, thisMouseX, thisMouseY);

        lastMouseX = -1;
        lastMouseY = -1;
    }

}

void KeyboardGui::stroke(int x1, int y1, int x2, int y2) {
    if (x2 == -1 && y2 == -1) {
        // point
        drawPixel(x1, y1);
    } else {
        // line
        drawLine(x1, y1, x2, y2);
    }
}

void KeyboardGui::drawPixel(int x, int y) {
    surfaceClear = false;
    drawTexture->drawPixel(SDL_Color {0x00, 0x00, 0x00, 0xff}, x, y, 3);
}

void KeyboardGui::drawLine(int x1, int y1, int x2, int y2) {
    surfaceClear = false;
    drawTexture->drawLine(SDL_Color {0x00, 0x00, 0x00, 0xff}, x1, y1, x2, y2, 3);
}

void KeyboardGui::clearDraw() {
    surfaceClear = true;
    drawTexture->clearTexture();
}

std::string KeyboardGui::getSurfaceData() {
    return ImageUtil::encode(drawTexture);
}

void KeyboardGui::loadSurfaceData(const std::string &data) {
    drawTexture = ImageUtil::decode(data, drawWidth, drawHeight);
}
