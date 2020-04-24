#include "KeyboardGui.h"

#include "GuiMacros.h"
#include "../ResourceManager.h"
#include "../Input.h"
#include "../util/Base64.h"
#include "../Graphics.h"

KeyboardGui::KeyboardGui() {
    handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    drawSurface = Graphics::createTexture(drawWidth, drawHeight);

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
    SDL_FreeCursor(handCursor);
    SDL_DestroyTexture(drawSurface);
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

    SDL_Rect rect {23, 17+192, drawWidth, drawHeight};
    Graphics::drawTexture(drawSurface, nullptr, &rect);

    SDL_SetCursor(showClick ? handCursor : SDL_GetDefaultCursor());
}

void KeyboardGui::sendMessage() {
    if (messageText.empty() && surfaceClear) return;

    instance->socket->send(((nlohmann::json) {
            {"action", "message"},
            {"room", instance->room},
            {"message", {{"type", (int) Message::USER_MESSAGE}, {"data", messageText}, {"image", getSurfaceData()}}}
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

    if (insideElement && !insideName && !topRightCorner && !bottomLeftCorner && !bottomRightCorner) {
        if (leftClick) {
            int thisMouseX = mx - baseX;
            int thisMouseY = my - baseY;
            if (hasLastMouse && !(thisMouseX == lastMouseX && thisMouseY == lastMouseY)) {
                drawLine(lastMouseX, lastMouseY, thisMouseX, thisMouseY);
            } else {
                drawPixel(thisMouseX, thisMouseY);
            }

            lastMouseX = thisMouseX;
            lastMouseY = thisMouseY;
        }

        if (!leftClick && hasLastMouse) {
            lastMouseX = -1;
            lastMouseY = -1;
        }
    }
}

void KeyboardGui::clearDraw() {
    surfaceClear = true;
    Graphics::clearTexture(drawSurface);
}

std::string KeyboardGui::getSurfaceData() {
    // RGBA32 = 4 bytes / pxl
    // only need to check alpha
    // alpha should be 4th byte
    auto pixels = Graphics::getTexturePixels(drawSurface, drawWidth, drawHeight);

    bool bitmap[drawWidth * drawHeight];
    for (int i = 0; i < drawWidth * drawHeight; i++) {
        bitmap[i] = pixels.get()[(i * 4) + 3] == 0xff;
    }

    // pack into byte
    unsigned char bitmapPacked[(drawWidth * drawHeight) / 8];
    for (int i = 0; i < (drawWidth * drawHeight) / 8; i++) {
        unsigned char cons = 0;

        for (int j = 0; j < 8; j++) {
            cons |= (bitmap[(i * 8) + j] << j);
        }

        bitmapPacked[i] = cons;
    }

    return Base64::base64_encode(bitmapPacked, sizeof(bitmapPacked));
}

void KeyboardGui::loadSurfaceData(const std::string &data) {
    clearDraw();

    std::string decoded = Base64::base64_decode(data);
    const char *bytes = decoded.c_str();
    int numBytes = decoded.size();

    int x = 0;
    int y = 0;

    for (int i = 0; i < numBytes; i++) {
        int workingByte = bytes[i];

        for (int j = 0; j < 8; j++) {
            int pixelValue = (workingByte & (1 << j)) != 0 ? 0xff : 0x00;

            if (pixelValue) {
                Graphics::textureDrawPoint(drawSurface, x, y);
            }
            x++;
            if (x >= drawWidth) {
                x = 0;
                y++;
            }
        }
    }
}

void KeyboardGui::drawPixel(int x, int y) {
    surfaceClear = false;
    Graphics::textureDrawPoint(drawSurface, x, y);
}

void KeyboardGui::drawLine(int x1, int y1, int x2, int y2) {
    surfaceClear = false;
    Graphics::textureDrawLine(drawSurface, x1, y1, x2, y2);
}
