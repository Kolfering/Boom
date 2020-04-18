#ifndef CHECKBOX_H
#define CHECKBOX_H
#include "Menu.h"
#include "Widget.h"
#include "GroupCheckBox.h"


class CheckBox : public Widget{
private:
    SDL_Texture* image[2];
    bool value;
    unsigned int calmTime;
    GroupCheckBox* groupCheckBox;

public:
    CheckBox(Menu*,bool = false);
    ~CheckBox();
    void onClic();
    bool getValue();
    void setValue(bool);
    SDL_Texture* getImage();
    void setGroupCheckBox(GroupCheckBox*);

};

#endif // CHECKBOX_H
