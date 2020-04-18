#ifndef JEU_H
#define JEU_H
#include "Menu.h"
#include <iostream>
#include <cstring>
#define CPS 60.
#define CPS_RATE (1/CPS)*1000
#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640

class Jeu{
private:
    static std::map<SDL_JoystickID, JoystickKeys*> joystickMap;
    Input* input;
    InputMouse* inputMouse;
    SDL_Window* screen;
    static SDL_Renderer* renderer;
    static SDL_Rect positionSouris;
    static Menu* menu;
    static unsigned int volumeMusic;
    static unsigned int volumeSound;
    unsigned int timeBeforeCPS;
    unsigned int timeAfterCPS;
    void calculCPS();
    void UpdateEvents();


public:
    Jeu(SDL_Window*,SDL_Renderer*,Input*,InputMouse*);
    ~Jeu();
    void start();
    static void initJoystick(SDL_JoystickID);
    static void cleanJoystick();
    static void setMenu(Menu*);
    static std::map<SDL_JoystickID, JoystickKeys*>& getJoystick();
    static Menu* getMenu();
    static SDL_Rect getPositionSouris();
    static SDL_Renderer* getRenderer();
    static void setVolumeMusic(float);
    static void setVolumeSound(float);
};


#endif // JEU_H
