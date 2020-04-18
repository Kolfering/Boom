#include "Transition.h"
#include "Jeu.h"
#include "Utile.h"

Transition::~Transition()
{
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}

Transition::Transition(bool in, unsigned int temps,Menu* menu)
{
    this->in = in;
    this->timeTransition = temps;
    this->menu = menu;
    finie = false;
    cps = (timeTransition / (255./5));
    timeAfter = 0;
    SDL_Surface* imageTmp = SDL_CreateRGBSurface(SDL_SWSURFACE,WINDOW_WIDTH,WINDOW_HEIGHT,32,MASQUE_ROUGE,MASQUE_VERT,MASQUE_BLEU,MASQUE_ALPHA);
    a = 255;
    increment = -5;

    if(in)
    {
        a = 0;
        increment = 5;
    }


    SDL_FillRect(imageTmp,nullptr,SDL_MapRGB(imageTmp->format,0,0,0));
    image = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);

}

Menu* Transition::getMenu()
{
    return menu;
}

bool Transition::isFinie()
{
    return finie;
}

SDL_Texture* Transition::getImage()
{
    return image;
}

bool Transition::isIn()
{
    return in;
}

void Transition::execute()
{
    if(getTime() - timeAfter > cps)
    {
        a += increment;
        if(a == 255 || a == 0)
            finie = true;

        timeAfter = getTime();
        SDL_SetTextureAlphaMod(image,a);
    }

}
