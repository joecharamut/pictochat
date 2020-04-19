#ifndef PROJECT_GUIOBJECT_H
#define PROJECT_GUIOBJECT_H


class GuiObject {
public:
    GuiObject() = default;
    virtual ~GuiObject() = 0;
    virtual void draw() = 0;
};


#endif //PROJECT_GUIOBJECT_H
