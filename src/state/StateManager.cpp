#include <utility>

#include "StateManager.h"

std::shared_ptr<State> StateManager::currentState = nullptr;
std::shared_ptr<State> StateManager::nextState = nullptr;

void StateManager::setState(std::shared_ptr<State> newState) {
    if (nextState) {
        printf("setting state too fast!");
    } else {
        nextState = std::move(newState);
    }
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




