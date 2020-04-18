#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED
#include <SDL.h>
#include "Menu.h"

class Widget{
protected:
    SDL_Rect position;
    Menu* menu;
    bool focus;
    bool displayable;

public:
    Widget(Menu* menu);
    virtual ~Widget();
    virtual void onClic();
    virtual void onFocus();
    virtual void onDeFocus();
    SDL_Rect* getAdressePosition();
    virtual SDL_Texture* getImage() = 0;
    bool isDisplayable();
    bool isFocus();

};

#endif // WIDGET_H_INCLUDED
