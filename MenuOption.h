#ifndef MENUOPTION_H
#define MENUOPTION_H

#include "MenuPrincipal.h"

class MenuOption : public MenuTemplate {
public:
    MenuOption(Input*,InputMouse*,SDL_Window*);
    ~MenuOption();
    void handleEvents();

};

#endif // MENUOPTION_H
