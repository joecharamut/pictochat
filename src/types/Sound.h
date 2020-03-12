#ifndef PROJECT_SOUND_H
#define PROJECT_SOUND_H

#include <SDL_mixer.h>

class Sound {
public:
    explicit Sound(Mix_Chunk *chunk);
    ~Sound();
    void play();
    void playSync();

private:
    Mix_Chunk *chunk = nullptr;
};


#endif //PROJECT_SOUND_H
