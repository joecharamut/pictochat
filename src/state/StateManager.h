#ifndef PROJECT_STATEMANAGER_H
#define PROJECT_STATEMANAGER_H


#include "State.h"
#include <memory>

class StateManager {
private:
    static std::shared_ptr<State> currentState;
    static std::shared_ptr<State> nextState;

public:
    static bool setState(std::shared_ptr<State> newState);
    static std::shared_ptr<State> getState();
    static void update();
};


#endif //PROJECT_STATEMANAGER_H
