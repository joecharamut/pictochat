#ifndef PROJECT_TESTSTATE_H
#define PROJECT_TESTSTATE_H

#include "State.h"
#include <memory>
#include "../types/Texture.h"
#include "../types/Text.h"
#include <SDL_mixer.h>

using namespace std;

class TestState : public State {
public:
    TestState();
    ~TestState() override;
    string getName() override;
    void update() override;

    shared_ptr<Text> text;

private:
    shared_ptr<Texture> image;
    Mix_Chunk *music;
};


#endif //PROJECT_TESTSTATE_H
