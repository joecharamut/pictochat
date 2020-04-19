#include "Timer.h"

void Timer::tick() {
    ticks++;
}

int Timer::get() {
    return ticks;
}

int Timer::reset() {
    int tmp = ticks;
    ticks = 0;
    return tmp;
}
