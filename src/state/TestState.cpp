#include "TestState.h"
#include "../ResourceManager.h"
#include "../types/Color.h"
#include "../Main.h"

TestState::TestState() {
    image = ResourceManager::loadTexture("res/test.bmp");
    text = make_shared<Text>("sand undertall", Font("res/bios.ttf"), 24, Text::Shaded,
            COLOR(0xff, 0x00, 0xff), COLOR(0x00, 0xff, 0x00));
}

TestState::~TestState() { }

string TestState::getName() {
    return "TestState";
}

void TestState::update() {
    Vector2 imageSize = image->getSize();
    SDL_Rect rect = {
            (Main::SCREEN_WIDTH / 2) - (imageSize.x / 2),
            (Main::SCREEN_HEIGHT / 2) - (imageSize.y / 2),
            imageSize.x,
            imageSize.y
    };

    Vector2 textSize = text->getSize();
    SDL_Rect rect2 = {0, 0, textSize.x, textSize.y};

    text->draw(&rect2);
    image->draw(nullptr, &rect);
}

