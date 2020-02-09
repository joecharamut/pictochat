#ifndef PROJECT_STATEMANAGER_H
#define PROJECT_STATEMANAGER_H


#include "State.h"

class StateManager {
private:
    State *currentState = nullptr;
    State *nextState = nullptr;

public:
    void setState(State *newState);
    State *getState();
    void load();
    void update();
};


#endif //PROJECT_STATEMANAGER_H
