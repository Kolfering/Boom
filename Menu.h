#ifndef MENU_H
#define MENU_H
#include "Enumeration.h"
#include "Input.h"
#include "Transition.h"
#include "InputJoystick.h"
#include <vector>
#include <SDL_ttf.h>
#include <map>

class SousMenu;

class Menu{
public:
    Menu(Input*,InputMouse*,SDL_Window*);
    Menu(Input*,InputMouse*,SDL_Window*, Menu*);
    virtual ~Menu();
    virtual void executeMenu() = 0;
    virtual void afficheMenu() = 0;
    virtual void handleEvents();
    void setTransition(Transition*);
    Transition* getTransition();
    TTF_Font* getPolice();
    Input* getInput();
    InputMouse* getInputMouse();
    SDL_Window* getScreen();
    std::vector<Menu*> &getListeSousMenu();
    void addSousMenu(Menu*);
    Menu* getMenuParent();
    IDMenu getIDMenu();



protected:
    Menu* menuParent;
    Input* input;
    InputMouse* inputMouse;
    SDL_Window* screen;
    bool transitionFinie;
    TTF_Font* police;
    Transition* transition;
    void afficherTransition();
    std::vector<Menu*> listeSousMenu;
    IDMenu idMenu;

};

#endif // MENU_H
