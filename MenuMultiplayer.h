#ifndef MENUMULTIPLAYER_H
#define MENUMULTIPLAYER_H

#include "MenuPrincipal.h"
#include "kiss_sdl.h"

class MenuMultiplayer : public MenuTemplate {
public:
    MenuMultiplayer(Input*,InputMouse*,SDL_Window*);
    ~MenuMultiplayer();
    void handleEvents();

};

#endif // MENUOPTION_H
