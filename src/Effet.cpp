#include "Effet.h"
#include "Utile.h"

SDL_Texture* Effet::image = nullptr;
Mix_Chunk* Effet::sonEffet = nullptr;
Effet::Effet(unsigned int x, unsigned int y)
{
    counter = 0;
    fini = false;
    position.x = x;
    position.y = y;
    position.w = CASE_DIMENSION;
    position.h = CASE_DIMENSION;
    counterReverse = false;
    tempsBefore = getTime();
}

void Effet::initialiserEffet()
{
    image = chargerSurface("../Images/Flash.png");
    sonEffet = Mix_LoadWAV("../Sons/Teleport.wav");
}

void Effet::releaseEffet()
{
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
    if(sonEffet)
        Mix_FreeChunk(sonEffet);
    sonEffet = nullptr;
}
Effet::~Effet()
{

}

bool Effet::isFini()
{
    return fini;
}

void Effet::calculNextFrame()
{
    if(getTime() - tempsBefore > TEMPS_EFFET_FRAME)
    {
        if(!counterReverse)
            counter++;
        else
            counter--;

        if(counter==NB_FRAME_EFFET-1)
            counterReverse = true;

        if(counter<0)
              fini = true;


        tempsBefore = getTime();
    }
}

SDL_Texture* Effet::getImage()
{
    return image;
}

Mix_Chunk* Effet::getSon()
{
    return sonEffet;
}
SDL_Rect* Effet::getAdresseAntiPosition()
{

    return &antiPosition;
}

void Effet::execute()
{
    calculNextFrame();
    antiPosition.x = counter * TAILLE_FRAME_EFFET;
    antiPosition.y = 0;
    antiPosition.w = TAILLE_FRAME_EFFET;
    antiPosition.h = TAILLE_FRAME_EFFET;
}

SDL_Rect* Effet::getAdressePosition()
{
    return &position;
}
