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

    thickButtonOff = GUI_BUTTON(LoadTexture("res/pictochat/keyboard/thickbutton.png"), 2, 192+71, COLOR(0xff, 0xff, 0xff),
            [this](bool click) {
        showClick = true;
        if (click) {
            setThickPen(true);
        }
    });

    thinButtonOff = GUI_BUTTON(LoadTexture("res/pictochat/keyboard/thinbutton.png"), 2, 192+86, COLOR(0xff, 0xff, 0xff),
            [this](bool click) {
        showClick = true;
        if (click) {
            setThickPen(false);
        }
    });

    thickButtonOn = GUI_BUTTON(LoadTexture("res/pictochat/keyboard/thickbuttonon.png"), 2, 192+71, COLOR(0xff, 0xff, 0xff),
            [this](bool click) {
        showClick = true;
        if (click) {
            setThickPen(true);
        }
    });

    thinButtonOn = GUI_BUTTON(LoadTexture("res/pictochat/keyboard/thinbuttonon.png"), 2, 192+86, COLOR(0xff, 0xff, 0xff),
            [this](bool click) {
        showClick = true;
        if (click) {
            setThickPen(false);
        }
    });

    penButtonOff = GUI_BUTTON(LoadTexture("res/pictochat/keyboard/penciloff.png"), 2, 192+38, COLOR(0xff, 0xff, 0xff),
            [this](bool click) {
        showClick = true;
        if (click) {
            setEraser(false);
        }
    });

    penButtonOn = GUI_BUTTON(LoadTexture("res/pictochat/keyboard/pencilon.png"), 2, 192+38, COLOR(0xff, 0xff, 0xff),
            [this](bool click) {
        showClick = true;
        if (click) {
            setEraser(false);
        }
    });

    eraserButtonOff = GUI_BUTTON(LoadTexture("res/pictochat/keyboard/eraseroff.png"), 2, 192+52, COLOR(0xff, 0xff, 0xff),
            [this](bool click) {
        showClick = true;
        if (click) {
            setEraser(true);
        }
    });

    eraserButtonOn = GUI_BUTTON(LoadTexture("res/pictochat/keyboard/eraseron.png"), 2, 192+52, COLOR(0xff, 0xff, 0xff),
            [this](bool click) {
        showClick = true;
        if (click) {
            setEraser(true);
        }
    });
}

KeyboardGui::~KeyboardGui() {

}

void KeyboardGui::setup(ChatState *instance) {
    this->instance = instance;
    nameText->text->setText(instance->username);

    clearDraw();
    setThickPen(true);
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

    if (thickPen) {
        thickButtonOn->draw();
        thinButtonOff->draw();
    } else {
        thickButtonOff->draw();
        thinButtonOn->draw();
    }

    if (eraserEnabled) {
        eraserButtonOn->draw();
        penButtonOff->draw();
    } else {
        eraserButtonOff->draw();
        penButtonOn->draw();
    }

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
            stroke(lastMouseX, lastMouseY, thisMouseX, thisMouseY);
        } else {
            stroke(thisMouseX, thisMouseY, thisMouseX, thisMouseY);
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

        stroke(lastMouseX, lastMouseY, thisMouseX, thisMouseY);

        lastMouseX = -1;
        lastMouseY = -1;
    }

    // clear name area (works?)
    drawTexture->drawRect(SDL_Color {0x00, 0x00, 0x00, 0x00}, SDL_Rect {0, 0, 60, 19});
}

void KeyboardGui::stroke(int x1, int y1, int x2, int y2) {
    if (x1 == x2 && y1 == y2) {
        // point
        drawPixel(x1, y1, thickPen);
    } else {
        // line
        drawLine(x1, y1, x2, y2, thickPen);
    }
}

void KeyboardGui::drawPixel(int x, int y, bool thick) {
    surfaceClear = false;

    SDL_Color drawColor;
    if (eraserEnabled) {
        drawColor = {0xff, 0x00, 0x00, 0x00};
    } else {
        drawColor = {0x00, 0x00, 0x00, 0xff};
    }

    if (!thick) {
        drawTexture->drawPixel(drawColor, x, y);
    } else {
        drawTexture->drawLine(drawColor, x, y, x, y, 2);
    }
}

void KeyboardGui::drawLine(int x1, int y1, int x2, int y2, bool thick) {
    surfaceClear = false;

    SDL_Color drawColor;
    if (eraserEnabled) {
        drawColor = {0xff, 0x00, 0x00, 0x00};
    } else {
        drawColor = {0x00, 0x00, 0x00, 0xff};
    }

    drawTexture->drawLine(drawColor, x1, y1, x2, y2, (thick ? 3 : 1));
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

void KeyboardGui::setThickPen(bool thick) {
    thickPen = thick;
}

void KeyboardGui::setEraser(bool erase) {
    eraserEnabled = erase;
}
