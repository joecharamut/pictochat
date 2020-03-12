#include "Sound.h"

Sound::Sound(Mix_Chunk *chunk) : chunk(chunk) {

}

Sound::~Sound() {
    Mix_FreeChunk(chunk);
}

void Sound::play() {
    Mix_ReserveChannels(1);
    if (Mix_PlayChannel(-1, chunk, 0) < 0) {
        printf("error playing music: %s\n", SDL_GetError());
    }
}

void Sound::playSync() {
    Mix_ReserveChannels(1);
    int channel;
    if ((channel = Mix_PlayChannel(-1, chunk, 0)) < 0) {
        printf("error playing music: %s\n", SDL_GetError());
    }

    while (Mix_Playing(channel));
}
