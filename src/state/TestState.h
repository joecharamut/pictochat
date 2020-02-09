#ifndef PROJECT_TESTSTATE_H
#define PROJECT_TESTSTATE_H

#include "State.h"

class TestState : State {
public:
    string getName() override;
    void load() override;
    void update() override;
};


#endif //PROJECT_TESTSTATE_H
