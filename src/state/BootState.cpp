#include "BootState.h"
#include "../Main.h"
#include "../Input.h"
#include "../../cmake-build-debug/src/Version.h"

BootState::BootState() {
    text = make_shared<Text>("0", std::make_shared<Font>("res/vga.ttf"), 20, Text::Blended_Wrapped, COLOR_WHITE,
            COLOR_BLACK, Main::SCREEN_WIDTH);
}

BootState::~BootState() {

}

string BootState::getName() {
    return "BootState";
}

void BootState::update() {
    frames++;
    std::string newText;
    switch (state) {
        case BIOS_START: {
            if (frames == 60) {
                console.write("stupid BIOS (c) 2020 spaghetti.rocks\n");
            } else if (frames == 120) {
                console.write("Rev. " + Version::GIT_SHA1_SHORT + "\n\n");
            } else if (frames == 180) {
                state = RAM_CHECK;
            }
        } break;

        case RAM_CHECK: {
            ramCounter++;
            std::string str = "\r" + std::to_string(ramCounter) + " KB OK";
            console.write(str);

            if (ramCounter > 127) {
                state = DOS_LOAD;

                console.write("\n\n");
                console.write("Starting WebDOS...\n");
                save = frames;
            }
        } break;

        case DOS_LOAD: {
            if (frames - save > 90) {
                state = COMMAND;
                console.write("\n\nC:\\> ");
                Input::enableKeyBuffer();
            }
        } break;

        case COMMAND: {
            std::string str = Input::popBuffer();
            commandStr += str;

            if (Input::getKeyDown(SDLK_RETURN)) {
                commandStr.erase(commandStr.find_last_not_of(' ') + 1);
                if (!commandStr.empty()) {
                    printf("command submit: %s\n", commandStr.c_str());
                }

                console.write("\n");
                commandStr = "";
            }

            if (Input::getKeyDown(SDLK_BACKSPACE)) {
                commandStr = commandStr.substr(0, commandStr.size() - 1);
                console.write("\b \b");
            }

            console.write("\rC:\\> ");
            console.write(commandStr);
        } break;
    }

    newText = console.bufferString();

    if (frames % 15 == 0) {
        cursor = !cursor;
    }

    if (cursor) {
        newText[console.cursorToStringIndex()] = '_';
    }

    text->setText(newText);
    text->draw(0, 0);
}
