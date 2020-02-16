#include "BootState.h"
#include "../Main.h"

BootState::BootState() {
    text = make_shared<Text>("0", std::make_shared<Font>("res/vga.ttf"), 20, Text::Blended_Wrapped, COLOR_WHITE, COLOR_BLACK, Main::SCREEN_WIDTH);
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
        case RAM_CHECK: {
            ramCounter++;
            newText = std::to_string(ramCounter) + " KB OK";

            if (ramCounter > 255) {
                save = frames;
                state = DOS_LOAD;
            }
        } break;

        case DOS_LOAD: {
            newText = std::to_string(ramCounter) + " KB OK\n\n";
            newText += "Starting MS-DOS...\n\n";

            if (frames - save > 90) {
                newText += "\n\nC:\\> ";
            }
        } break;
    }

    if (frames % 15 == 0) {
        cursor = !cursor;
    }
    newText += (cursor ? "_" : "");

    text->setText(newText);
    text->draw(0, 0);
}
