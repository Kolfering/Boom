#ifndef MENUFIN_H
#define MENUFIN_H
#include "MenuTemplate.h"


class MenuFin : public MenuTemplate {
public:
    MenuFin(Input*,InputMouse*,SDL_Window*);
    ~MenuFin();
    void handleEvents();

};

#endif // MENUFIN_H
