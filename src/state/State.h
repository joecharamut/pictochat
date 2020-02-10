#ifndef PROJECT_STATE_H
#define PROJECT_STATE_H

#include <string>

using namespace std;

class State {
public:
    State() { };
    virtual ~State() = 0;
    virtual string getName() = 0;
    virtual void update() = 0;
};


#endif //PROJECT_STATE_H
