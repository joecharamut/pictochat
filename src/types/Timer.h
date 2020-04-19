#ifndef PROJECT_TIMER_H
#define PROJECT_TIMER_H


class Timer {
public:
    void tick();
    int get();
    int reset();

private:
    int ticks;
};


#endif //PROJECT_TIMER_H
