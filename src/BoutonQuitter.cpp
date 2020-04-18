#include "BoutonQuitter.h"
#include <SDL_ttf.h>
#include "Menu.h"
#include "Jeu.h"
#include "Bouton.h"

BoutonQuitter::BoutonQuitter(Menu* menu) : Bouton(menu)
{
    SDL_Surface* imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Quit",COLOR_BOUTON_POLICE);
    image[0] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
    int w,h;
    SDL_QueryTexture(image[0],nullptr,nullptr,&w,&h);
    position.w = w;
    position.h = h;
    imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Quit",COLOR_BOUTON_POLICE_FOCUS);
    image[1] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
}

void BoutonQuitter::onClic()
{
   if(!menu->getTransition())
       menu->getInput()->quit = 1;
}

BoutonQuitter::~BoutonQuitter()
{

}
