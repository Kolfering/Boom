#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Widget.h"
#include "Libelle.h"
#include <string>

class TextBox : public Widget{
private:
    SDL_Texture* image[2];
    SDL_Scancode value;
    Libelle* libelle;
    bool clicked;
public:
    TextBox(Menu*,SDL_Scancode = SDL_SCANCODE_UNKNOWN);
    ~TextBox();
    void setPosition(int,int);
    Libelle* getLibelle();
    SDL_Texture* getImage();
    void onClic();
    bool isClicked();
    void setClicked(bool);
    void setValue(SDL_Scancode);
    SDL_Scancode getValue();

};

#endif // TEXTBOX_H
