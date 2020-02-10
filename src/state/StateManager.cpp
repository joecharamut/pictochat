#include <utility>

#include "StateManager.h"

std::shared_ptr<State> StateManager::currentState = nullptr;
std::shared_ptr<State> StateManager::nextState = nullptr;

bool StateManager::setState(std::shared_ptr<State> newState) {
    if (nextState) {
        printf("already trying to set a new state");
        return false;
    }

    nextState = std::move(newState);
    return true;
}

std::shared_ptr<State> StateManager::getState() {
    return currentState;
}


void StateManager::update() {
    if (nextState) {
        currentState = nextState;
        nextState = nullptr;
    }
    if (currentState) {
        currentState->update();
    }
}




