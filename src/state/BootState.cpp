#include "BootState.h"
#include "../Main.h"
#include "../Input.h"
#include "../util/Version.h"
#include "../Graphics.h"
#include "../dos/command/CommandChat.h"
#include <SDL_mixer.h>

BootState::BootState() {
    text = std::make_shared<Text>("0", std::make_shared<Font>("res/vga.ttf"), 20, Text::Blended_Wrapped,
            COLOR(0xff, 0xbf, 0x00), COLOR_BLACK, Main::SCREEN_WIDTH);

    SDL_Surface *loadSurface = SDL_LoadBMP("res/scanlines3.bmp");
    if (!loadSurface) {
        printf("error loading scanline texture: %s\n", SDL_GetError());
        Main::quit();
    }

    SDL_Surface *scanlineSurface = SDL_CreateRGBSurfaceWithFormat(0, Main::SCREEN_WIDTH,
            Main::SCREEN_HEIGHT * 2, 32, SDL_PIXELFORMAT_RGBA32);
    if (!scanlineSurface) {
        printf("error creating scanline surface: %s\n", SDL_GetError());
        Main::quit();
    }

    for (int x = 0; x < Main::SCREEN_WIDTH; x += loadSurface->w) {
        for (int y = 0; y < Main::SCREEN_HEIGHT * 2; y += loadSurface->h) {
            SDL_Rect rect {x, y, x + loadSurface->w, y + loadSurface->h};
            SDL_BlitSurface(loadSurface, nullptr, scanlineSurface, &rect);
        }
    }

    scanlineTexture = Graphics::createTexture(scanlineSurface);
    SDL_FreeSurface(loadSurface);
    SDL_FreeSurface(scanlineSurface);
}

BootState::~BootState() {

}

void BootState::update() {
    frames++;

    console->update();

    std::string newText;
    newText = console->bufferString();

    if (frames % 15 == 0) {
        cursor = !cursor;
    }

    if (cursor) {
        newText[console->cursorToStringIndex()] = '_';
    }

    text->setText(newText);
    text->draw(0, 0);

    // update scanline positions
    scanlineOffset++;
    if (scanlineOffset > SCANLINE_LIMIT) scanlineOffset = 0;
    SDL_Rect rect {0, (scanlineOffset / 2) - Main::SCREEN_HEIGHT, Main::SCREEN_WIDTH, Main::SCREEN_HEIGHT * 2};
    // draw scanlines
    scanlineTexture->draw(nullptr, &rect);

    if (console->bufferEmpty()) {
        updateState();
    }
}

void BootState::updateState() {
    switch (state) {
        case BIOS_START: {
            if (frames == 60) {
                console->write("stupid BIOS (c) 2020 spaghetti.rocks Enterprises Ltd.\n");
            } else if (frames == 120) {
                console->write("Rev. " + Version::GIT_SHA1_SHORT + "\n\n");
            } else if (frames == 180) {
                state = RAM_CHECK;
            }
        } break;

        case RAM_CHECK: {
            ramCounter++;
            std::string str = "\r" + std::to_string(ramCounter) + " KB OK";
            console->write(str);
            console->flush();

            if (ramCounter > 127) {
                state = DOS_LOAD;

                console->write("\n\n");
                console->write("Starting...\n");
                save = frames;
            }
        } break;

        case DOS_LOAD: {
            if (frames - save > 90) {
                state = COMMAND;
            }
        } break;

        case COMMAND: {
            std::string path = shell.filesystem->getcwd() + "\\";
            if (!prompt) {
                console->write("\r" + path + "> ");
                Input::enableKeyBuffer();
                prompt = true;
            }

            std::string str = Input::popBuffer();
            if (!str.empty()) {
                commandStr += str;
                console->write("\r" + path + "> ");
                console->write(commandStr);
                console->flush();
            }

            if (Input::getKeyDown(SDLK_RETURN)) {
                console->write("\n");
                prompt = false;
                Input::disableKeyBuffer();

                commandStr.erase(commandStr.find_last_not_of(' ') + 1);
                if (!commandStr.empty()) {
                    runningCommand = shell.processCommand(commandStr).get();
                    if (runningCommand) {
                        state = RUNNING;
                    }
                }
                commandStr = "";
            }

            if (Input::getKeyDown(SDLK_BACKSPACE)) {
                if (!commandStr.empty()) {
                    console->write("\b \b");
                }

                commandStr = commandStr.substr(0, commandStr.size() - 1);
            }
        } break;

        case RUNNING: {
            if (runningCommand->update() == Command::COMMAND_FINISHED) {
                state = COMMAND;
                runningCommand = nullptr;
            }
        } break;
    }
}
