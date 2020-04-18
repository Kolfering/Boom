#ifndef BOUTON_H
#define BOUTON_H
#include <SDL.h>
#include "Menu.h"
#include "Widget.h"
#define COLOR_BOUTON_POLICE {255,255,255}
#define COLOR_BOUTON_POLICE_FOCUS {235,45,23}


class Bouton : public Widget{
protected:
    SDL_Texture* image[2];
    unsigned int indiceImage;

public:
    Bouton(Menu*);
    virtual ~Bouton();
    virtual void onClic() = 0;
    void onFocus();
    void onDeFocus();
    SDL_Texture* getImage();

};



#endif // BOUTON_H
