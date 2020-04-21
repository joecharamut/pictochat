#include "MainMenuGui.h"

#include "../ResourceManager.h"
#include "../types/Color.h"
#include "../util/Util.h"

#include "GuiMacros.h"

MainMenuGui::MainMenuGui() {
    defaultCursor = SDL_GetDefaultCursor();
    handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    bottom = GUI_IMAGE(LoadTexture("res/pictochat/bsmenu.png"), 0, SCREEN_H);

    signalRedA = GUI_IMAGE(LoadTexture("res/pictochat/signalred.png"), 201, SCREEN_H + 40 + 32*0);
    signalRedB = GUI_IMAGE(LoadTexture("res/pictochat/signalred.png"), 201, SCREEN_H + 40 + 32*1);
    signalRedC = GUI_IMAGE(LoadTexture("res/pictochat/signalred.png"), 201, SCREEN_H + 40 + 32*2);
    signalRedD = GUI_IMAGE(LoadTexture("res/pictochat/signalred.png"), 201, SCREEN_H + 40 + 32*3);
    signalGreenA = GUI_IMAGE(LoadTexture("res/pictochat/signalgreen.png"), 201, SCREEN_H + 40 + 32*0);
    signalGreenB = GUI_IMAGE(LoadTexture("res/pictochat/signalgreen.png"), 201, SCREEN_H + 40 + 32*1);
    signalGreenC = GUI_IMAGE(LoadTexture("res/pictochat/signalgreen.png"), 201, SCREEN_H + 40 + 32*2);
    signalGreenD = GUI_IMAGE(LoadTexture("res/pictochat/signalgreen.png"), 201, SCREEN_H + 40 + 32*3);

    backButton = GUI_BUTTON(LoadTexture("res/pictochat/backButton.png"), 90, SCREEN_H + 172, COLOR(0x80, 0x80, 0x80),
            [this](bool leftClick) {
        showClick = true;
    });

    cursor = GUI_BUTTON(LoadTexture("res/pictochat/cursor.png"), cursorX, cursorY, COLOR_WHITE, ([this](bool leftClick) {
        showClick = true;
        if (!ready) return;
        if (leftClick && !pendingJoin) {
            pendingJoin = true;
            pendingRoom = cursorRoom;
            instance->socket->send(((nlohmann::json) {
                    {"action", "join"},
                    {"room", roomStrings[cursorRoom]}
            }).dump());
        }
    }));

    onlineTextA = GUI_DS_TEXT("0/16", COLOR_BLACK, 166, SCREEN_H + 42 + 32*0);
    onlineTextB = GUI_DS_TEXT("0/16", COLOR_BLACK, 166, SCREEN_H + 42 + 32*1);
    onlineTextC = GUI_DS_TEXT("0/16", COLOR_BLACK, 166, SCREEN_H + 42 + 32*2);
    onlineTextD = GUI_DS_TEXT("0/16", COLOR_BLACK, 166, SCREEN_H + 42 + 32*3);
}

MainMenuGui::~MainMenuGui() {
    SDL_FreeCursor(defaultCursor);
    SDL_FreeCursor(handCursor);
}

void MainMenuGui::setup(ChatState *instance) {
    this->instance = instance;
    this->instance->pingServer();
}

void MainMenuGui::draw() {
    timer.tick();
    showClick = false;

    updateCursorPos();
    cursor->move(cursorX, cursorY);

    if (timer.get() > 180) {
        timer.reset();
        instance->pingServer();
        if (!instance->online) {
            users[0] = -1;
            users[1] = -1;
            users[2] = -1;
            users[3] = -1;
        }
    }

    bottom->draw();

    onlineTextA->draw();
    onlineTextB->draw();
    onlineTextC->draw();
    onlineTextD->draw();

    cursor->draw();
    backButton->draw();

    (users[0] < 0 ? signalRedA : signalGreenA)->draw();
    (users[1] < 0 ? signalRedB : signalGreenB)->draw();
    (users[2] < 0 ? signalRedC : signalGreenC)->draw();
    (users[3] < 0 ? signalRedD : signalGreenD)->draw();

    SDL_SetCursor((showClick ? handCursor : defaultCursor));

    ready = true;
}

void MainMenuGui::updateCursorPos() {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    int bottomScreenX = mx;
    int bottomScreenY = my - SCREEN_H;

    int room = -1;
    if (bottomScreenX > 32 && bottomScreenX < 223) {
        if (bottomScreenY > 32 && bottomScreenY <= 64) {
            room = 0; // room a
        } else if (bottomScreenY > 64 && bottomScreenY <= 96) {
            room = 1; // room b
        } else if (bottomScreenY > 96 && bottomScreenY <= 128) {
            room = 2; // room c
        } else if (bottomScreenY > 128 && bottomScreenY <= 160) {
            room = 3; // room d
        }
    }

    if (room != -1) {
        cursorRoom = room;
    }

    switch (room) {
        default:
            break;
        case 0:
            cursorX = 26;
            cursorY = SCREEN_H + 28;
            break;
        case 1:
            cursorX = 26;
            cursorY = SCREEN_H + 60;
            break;
        case 2:
            cursorX = 26;
            cursorY = SCREEN_H + 92;
            break;
        case 3:
            cursorX = 26;
            cursorY = SCREEN_H + 124;
            break;
    }
}

void MainMenuGui::socketMessage(std::string message) {
    auto json = nlohmann::json::parse(message);
    auto type = json["type"].get<std::string>();
    if (type == "status") {
        users[0] = json["roomA"].get<int>();
        users[1] = json["roomB"].get<int>();
        users[2] = json["roomC"].get<int>();
        users[3] = json["roomD"].get<int>();

        onlineTextA->text->setText(std::to_string((users[0] != -1 ? users[0] : 0)) + "/16");
        onlineTextB->text->setText(std::to_string((users[1] != -1 ? users[1] : 0)) + "/16");
        onlineTextC->text->setText(std::to_string((users[2] != -1 ? users[2] : 0)) + "/16");
        onlineTextD->text->setText(std::to_string((users[3] != -1 ? users[3] : 0)) + "/16");
    } else if (type == "join") {
        pendingJoin = false;
        bool success = json["success"].get<bool>();
        if (!success) {
            instance->topScreenGui->pushMessage(Message(Message::SYSTEM_MESSAGE, "Error Joining Room"));
        } else {
            instance->setRoom(roomStrings[pendingRoom]);
            SDL_SetCursor(SDL_GetDefaultCursor());
        }
    }
}
