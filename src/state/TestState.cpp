#include "TestState.h"
#include "../ResourceManager.h"
#include "../types/Color.h"
#include "../Main.h"

TestState::TestState() {
    image = ResourceManager::loadTexture("res/test.bmp");
    text = make_shared<Text>("sand undertall", std::make_shared<Font>("res/bios.ttf"), 14, Text::Blended_Wrapped,
            COLOR(0xff, 0x00, 0xff), COLOR(0x00, 0xff, 0x00), Main::SCREEN_WIDTH);
    music = Mix_LoadWAV("res/output.ogg");
    if (!music) {
        printf("error loading music: %s\n", SDL_GetError());
    }
    Mix_ReserveChannels(1);

    if (Mix_PlayChannel(-1, music, -1) < 0) {
        printf("error playing music: %s\n", SDL_GetError());
    }
}

TestState::~TestState() {
    Mix_FreeChunk(music);
}

string TestState::getName() {
    return "TestState";
}

void TestState::update() {
    SDL_Rect rect = {
            (Main::SCREEN_WIDTH  / 2) - (image->w / 2),
            (Main::SCREEN_HEIGHT / 2) - (image->h / 2),
            image->w,
            image->h
    };

    text->draw(0, 0);
    image->draw(nullptr, &rect);
}

