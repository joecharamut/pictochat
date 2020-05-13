#ifndef PROJECT_MOBILETEXTINPUT_H
#define PROJECT_MOBILETEXTINPUT_H

#include <string>

class MobileTextInput {
public:
    static void showInput();
    static bool pollInput();
    static std::string getInput();
};


#endif //PROJECT_MOBILETEXTINPUT_H
