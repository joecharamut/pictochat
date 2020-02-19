#ifndef PROJECT_BOOTSTATE_H
#define PROJECT_BOOTSTATE_H

#include "State.h"
#include "../types/Text.h"
#include "../dos/Console.h"

#include <memory>

class BootState : public State {
public:
    BootState();
    ~BootState() override;
    string getName() override;
    void update() override;

private:
    enum InternalState {
        TEST,
        RAM_CHECK,
        DOS_LOAD,
        COMMAND
    };

    InternalState state = RAM_CHECK;

    std::shared_ptr<Text> text;
    int save = 0;
    int ramCounter = 0;
    int frames = 0;
    bool cursor = false;

    Console console = Console();
    std::string commandStr;

};


#endif //PROJECT_BOOTSTATE_H
