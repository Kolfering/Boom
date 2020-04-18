#ifndef LETTRE_H
#define LETTRE_H
#include "Enumeration.h"

class Lettre{
private:
    static SDL_Texture* image;
    unsigned int indice;
    static unsigned int score;
    SDL_Rect position;
    SDL_Rect antiPosition;
    CounterFrame counterFrame;
    FrameManager* frameManager;
    FrameManager frameManagerE;
    FrameManager frameManagerEX;
    FrameManager frameManagerX;
    FrameManager frameManagerXT;
    FrameManager frameManagerT;
    FrameManager frameManagerTR;
    FrameManager frameManagerR;
    FrameManager frameManagerRA;
    FrameManager frameManagerA;
    FrameManager frameManagerAE;
    void calculNextFrame();

public:
    Lettre(SDL_Rect);
    ~Lettre();
    void execute();
    unsigned int getIndice();
    static void initialiserLettre();
    static void releaseLettre();
    static unsigned int getScore();
    SDL_Texture* getImage();
    SDL_Rect* getAdressePosition();
    SDL_Rect* getAdresseAntiPosition();

};

#endif // LETTRE_H
