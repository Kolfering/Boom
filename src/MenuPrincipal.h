#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H
#include "MenuTemplate.h"


class MenuPrincipal : public MenuTemplate {
public:
    MenuPrincipal(Input*,InputMouse*,SDL_Window*);
    ~MenuPrincipal();

};

#endif // MENUPRINCIPAL_H
