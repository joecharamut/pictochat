#ifndef PROJECT_TESTSTATE_H
#define PROJECT_TESTSTATE_H

#include "State.h"
#include <memory>
#include "../types/Texture.h"
#include "../types/Text.h"
#include <SDL_mixer.h>

using namespace std;

class TestState : public State {
private:
    shared_ptr<Texture> image;
    shared_ptr<Text>    text;
    Mix_Chunk *music;

public:
    TestState();
    ~TestState() override;
    string getName() override;
    void update() override;
};


#endif //PROJECT_TESTSTATE_H
