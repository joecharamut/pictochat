#ifndef PROJECT_TESTSTATE_H
#define PROJECT_TESTSTATE_H

#include "State.h"
#include <memory>
#include "../types/Texture.h"
#include "../types/Text.h"
#include <SDL_mixer.h>

class TestState : public State {
public:
    TestState();
    ~TestState() override;
    void update() override;

    std::shared_ptr<Text> text;

private:
    std::shared_ptr<Texture> image;
    Mix_Chunk *music;
};


#endif //PROJECT_TESTSTATE_H
