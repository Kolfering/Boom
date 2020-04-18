#include "Score.h"
#include "Carte.h"
#include "Utile.h"
SDL_Texture* Score::upMegaVie = nullptr;
SDL_Texture* Score::surface = nullptr;
SDL_Texture* Score::surface5000 = nullptr;
SDL_Texture* Score::surface100K = nullptr;
Score::Score(int x,int y,unsigned int valeur)
{
    this->position.x = x;
    this->position.y = y;
    positionReal.x = position.x;
    positionReal.y = position.y;
    this->valeur = valeur;
    fini = false;
    timer = getTime();

    position.w = CASE_DIMENSION;

    switch(valeur)
    {
    case 1:
        counter = 0;
        position.h = 20;
        break;
    case 2:
        counter = 1;
        position.h = 20;
        break;
    case 10:
        counter = 0;
        position.h = CASE_DIMENSION/2;
        break;
    case 50:
        counter = 1;
        position.h = CASE_DIMENSION/2;
        break;
    case 100:
        counter = 2;
        position.h = CASE_DIMENSION/2;
        break;
    case 150:
        counter = 3;
        position.h = CASE_DIMENSION/2;
        break;
    case 200:
        counter = 4;
        position.h = CASE_DIMENSION/2;
        break;
    case 300:
        counter = 5;
        position.h = CASE_DIMENSION/2;
        break;
    case 400:
        counter = 6;
        position.h = CASE_DIMENSION/2;
        break;
    case 500:
        counter = 7;
        position.h = CASE_DIMENSION/2;
        break;
    case 600:
        counter = 8;
        position.h = CASE_DIMENSION/2;
        break;
    case 700:
        counter = 9;
        position.h = CASE_DIMENSION/2;
        break;
    case 800:
        counter = 10;
        position.h = CASE_DIMENSION/2;
        break;
    case 900:
        counter = 11;
        position.h = CASE_DIMENSION/2;
        break;
    case 1000:
        counter = 12;
        position.h = CASE_DIMENSION/2;
        break;
    case 5000:
        counter = 0;
        position.h = 19;
        position.w = 42;
        break;
    case 100000:
        counter = 0;
        position.h = 28;
        position.w = 75;
    default:
        counter = 0;
        break;
    }

}

Score::~Score()
{

}

Position Score::getPositionReal()
{
    return positionReal;
}

SDL_Rect* Score::getAdresseAntiPosition()
{
    if(valeur>1000)
        return nullptr;

    antiPosition.x = counter * CASE_DIMENSION;
    antiPosition.y = 0;
    antiPosition.w = CASE_DIMENSION;
    if(valeur>2)
        antiPosition.h = CASE_DIMENSION/2;
    else
        antiPosition.h = 20;
    return &antiPosition;
}

void Score::initialiserScore()
{
    surface = chargerSurface("Images/Points.png");
    surface5000 = chargerSurface("Images/Points5k.png");
    upMegaVie = chargerSurface("Images/1_2Up.png");
    surface100K = chargerSurface("Images/Points_100K.png");
}

void Score::releaseScore()
{
    if(surface)
        SDL_DestroyTexture(surface);
    surface = nullptr;
    if(surface5000)
        SDL_DestroyTexture(surface5000);
    surface5000 = nullptr;
    if(surface100K)
        SDL_DestroyTexture(surface100K);
    surface100K = nullptr;
    if(upMegaVie)
        SDL_DestroyTexture(upMegaVie);
    upMegaVie = nullptr;
}
void Score::setPosition(float x,float y)
{
    positionReal.x = x;
    positionReal.y = y;
}

bool Score::isFini()
{
    return fini;
}

void Score::setFini(bool fini)
{
    this->fini = fini;
}

unsigned int Score::getTimer()
{
    return timer;
}

SDL_Texture* Score::getSurface()
{
    if(valeur>5000)
        return surface100K;
    else if(valeur>1000)
        return surface5000;
    else if(valeur>2)
        return surface;
    else
        return upMegaVie;
}

SDL_Rect* Score::getAdressePosition()
{
    position.x = positionReal.x;
    position.y = positionReal.y;
    return &position;
}
