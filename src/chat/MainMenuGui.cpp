#include "MainMenuGui.h"

#include "../ResourceManager.h"
#include "../types/Color.h"
#include "../util/Util.h"

#define LoadTexture(PATH) ResourceManager::loadTexture(PATH)
#define GUI_IMAGE(tex, x, y) std::make_shared<GuiImage>(tex, x, y)
#define GUI_BUTTON(t, x, y, c, f) std::make_shared<GuiButton>(t, x, y, c, f)
#define GUI_DS_TEXT(t, x, y) std::make_shared<GuiText>(std::make_shared<Text>(t, \
    std::make_shared<Font>("res/pictochat/ds_bios.ttf"), 16, Text::Blended, COLOR_BLACK), x, y)

MainMenuGui::MainMenuGui() {
    defaultCursor = SDL_GetDefaultCursor();
    handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    top = GUI_IMAGE(LoadTexture("res/pictochat/tsmenu.png"), CENTER_X, CENTER_Y);
    bottom = GUI_IMAGE(LoadTexture("res/pictochat/bsmenu.png"), CENTER_X, CENTER_Y + SCREEN_H);

    signalRedA = GUI_IMAGE(LoadTexture("res/pictochat/signalred.png"), BASE_X + 201, BASE_Y + 40 + 32*0);
    signalRedB = GUI_IMAGE(LoadTexture("res/pictochat/signalred.png"), BASE_X + 201, BASE_Y + 40 + 32*1);
    signalRedC = GUI_IMAGE(LoadTexture("res/pictochat/signalred.png"), BASE_X + 201, BASE_Y + 40 + 32*2);
    signalRedD = GUI_IMAGE(LoadTexture("res/pictochat/signalred.png"), BASE_X + 201, BASE_Y + 40 + 32*3);
    signalGreenA = GUI_IMAGE(LoadTexture("res/pictochat/signalgreen.png"), BASE_X + 201, BASE_Y + 40 + 32*0);
    signalGreenB = GUI_IMAGE(LoadTexture("res/pictochat/signalgreen.png"), BASE_X + 201, BASE_Y + 40 + 32*1);
    signalGreenC = GUI_IMAGE(LoadTexture("res/pictochat/signalgreen.png"), BASE_X + 201, BASE_Y + 40 + 32*2);
    signalGreenD = GUI_IMAGE(LoadTexture("res/pictochat/signalgreen.png"), BASE_X + 201, BASE_Y + 40 + 32*3);

    backButton = GUI_BUTTON(LoadTexture("res/pictochat/backButton.png"), BASE_X + 90, BASE_Y + 172,
            COLOR(0x80, 0x80, 0x80), [this](bool leftClick) {
        showClick = true;
        if (leftClick) {
//            instance->closeWindow = true;
        }
    });

    cursor = GUI_BUTTON(LoadTexture("res/pictochat/cursor.png"), cursorX, cursorY, COLOR_WHITE, [this](bool leftClick) {
        showClick = true;
    });

    onlineTextA = GUI_DS_TEXT("0/16", BASE_X + 166, BASE_Y + 42 + 32 * 0);
    onlineTextB = GUI_DS_TEXT("0/16", BASE_X + 166, BASE_Y + 42 + 32 * 1);
    onlineTextC = GUI_DS_TEXT("0/16", BASE_X + 166, BASE_Y + 42 + 32 * 2);
    onlineTextD = GUI_DS_TEXT("0/16", BASE_X + 166, BASE_Y + 42 + 32 * 3);
}

MainMenuGui::~MainMenuGui() {
    SDL_FreeCursor(defaultCursor);
    SDL_FreeCursor(handCursor);
}

void MainMenuGui::setup(std::string userId, ChatState *instance) {
    this->userId = std::move(userId);
    this->instance = instance;
    pingStatus();
}

void MainMenuGui::draw() {
    timer.tick();
    showClick = false;

    updateCursorPos();
    cursor->move(cursorX, cursorY);

    if (timer.get() > 180) {
        timer.reset();
        pingStatus();
    }

    top->draw();
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
}

void MainMenuGui::updateCursorPos() {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    int bottomScreenX = mx - CENTER_X;
    int bottomScreenY = my - CENTER_Y - SCREEN_H;

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

    switch (room) {
        default:
            break;
        case 0:
            cursorX = BASE_X + 26;
            cursorY = BASE_Y + 28;
            break;
        case 1:
            cursorX = BASE_X + 26;
            cursorY = BASE_Y + 60;
            break;
        case 2:
            cursorX = BASE_X + 26;
            cursorY = BASE_Y + 92;
            break;
        case 3:
            cursorX = BASE_X + 26;
            cursorY = BASE_Y + 124;
            break;
    }
}

void MainMenuGui::pingStatus() {
    Network::Request("http://lab.spaghetti.rocks:8080/status", Network::GET, [this](Network::Response resp) {
        if (resp.status == Network::SUCCESS && resp.statusCode == 200) {
            std::string respStr;

            for (char b : *resp.data) {
                respStr += b;
            }
            auto split = Util::splitString(respStr, ",");
            users[0] = std::stoi(split[1]);
            users[1] = std::stoi(split[2]);
            users[2] = std::stoi(split[3]);
            users[3] = std::stoi(split[4]);
        } else {
            users[0] = -1;
            users[1] = -1;
            users[2] = -1;
            users[3] = -1;
        }

        onlineTextA->text->setText(std::to_string((users[0] != -1 ? users[0] : 0)) + "/16");
        onlineTextB->text->setText(std::to_string((users[1] != -1 ? users[1] : 0)) + "/16");
        onlineTextC->text->setText(std::to_string((users[2] != -1 ? users[2] : 0)) + "/16");
        onlineTextD->text->setText(std::to_string((users[3] != -1 ? users[3] : 0)) + "/16");
    }).execute();
}
