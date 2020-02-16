#ifndef PROJECT_BOOTSTATE_H
#define PROJECT_BOOTSTATE_H

#include "State.h"
#include "../types/Text.h"

#include <memory>

class BootState : public State {
public:
    BootState();
    ~BootState() override;
    string getName() override;
    void update() override;

private:
    enum InternalState {
        RAM_CHECK, DOS_LOAD
    };

    InternalState state = RAM_CHECK;

    std::shared_ptr<Text> text;
    std::string oldText;
    int save;
    int ramCounter = 0;
    int frames = 0;
    bool cursor = false;

    int bootWait = 0;
};


#endif //PROJECT_BOOTSTATE_H
