#ifndef PROJECT_BOOTSTATE_H
#define PROJECT_BOOTSTATE_H

#include "State.h"
#include "../types/Text.h"
#include "../dos/Console.h"
#include "../dos/Shell.h"

#include <memory>

class BootState : public State {
public:
    BootState();
    ~BootState() override;
    void update() override;

private:
    enum InternalState {
        BIOS_START,
        RAM_CHECK,
        DOS_LOAD,
        COMMAND,
        RUNNING
    };

    InternalState state = BIOS_START;

    std::shared_ptr<Text> text;
    int save = 0;
    int ramCounter = 0;
    int frames = 0;
    bool cursor = false;
    bool prompt = false;

    std::shared_ptr<Console> console = std::make_shared<Console>();
    Shell shell = Shell(console);
    std::string commandStr;

    Command *runningCommand;
};


#endif //PROJECT_BOOTSTATE_H
