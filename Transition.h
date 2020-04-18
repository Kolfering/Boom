#ifndef TRANSITION_H
#define TRANSITION_H
#include <SDL.h>

class Menu;

class Transition{
private:
    SDL_Texture* image;
    Menu* menu;
    bool in;
    int timeTransition;
    int timeAfter;
    int cps;
    int increment;
    bool finie;
    Uint8 a;
public:
    Transition(bool, unsigned int,Menu* = nullptr);
    ~Transition();
    SDL_Texture* getImage();
    Menu* getMenu();
    void execute();
    bool isFinie();
    bool isIn();

};

#endif // TRANSITION_H
