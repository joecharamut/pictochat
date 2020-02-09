#include "StateManager.h"

void StateManager::setState(State *newState) {
    if (nextState) {
        printf("setting state too fast!");
    } else {
        nextState = newState;
    }
}

State *StateManager::getState() {
    return currentState;
}

void StateManager::load() {

}

void StateManager::update() {
    if (nextState) {
        currentState = nextState;
        nextState = nullptr;
    }
    currentState->update();
}




