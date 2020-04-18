#ifndef SCORE_H
#define SCORE_H
#define TEMPS_SCORE 1000
#include <SDL.h>
#include "Enumeration.h"
#include "Input.h"

class Score{
private:
    static SDL_Texture* surface;
    static SDL_Texture* surface5000;
    static SDL_Texture* upMegaVie;
    static SDL_Texture* surface100K;
    SDL_Rect position;
    SDL_Rect antiPosition;
    Position positionReal;
    bool fini;
    unsigned int valeur;
    unsigned int timer;
    unsigned int counter;

public:
    Score(int,int,unsigned int);
    bool isFini();
    void setFini(bool);
    void setPosition(float,float);
    SDL_Texture* getSurface();
    Position getPositionReal();
    SDL_Rect* getAdressePosition();
    SDL_Rect* getAdresseAntiPosition();
    static void initialiserScore();
    static void releaseScore();
    unsigned int getTimer();
    ~Score();

};

#endif // SCORE_H
