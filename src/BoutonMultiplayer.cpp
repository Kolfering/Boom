#include "BoutonMultiplayer.h"
#include <SDL_ttf.h>
#include "MenuMultiPlayer.h"
#include "Menu.h"
#include "Jeu.h"

BoutonMultiplayer::BoutonMultiplayer(Menu* menu) : Bouton(menu)
{
    SDL_Surface* imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Network",COLOR_BOUTON_POLICE);
    image[0] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
    int w,h;
    SDL_QueryTexture(image[0],nullptr,nullptr,&w,&h);
    position.w = w;
    position.h = h;
    imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Network",COLOR_BOUTON_POLICE_FOCUS);
    image[1] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
}

void BoutonMultiplayer::onClic()
{
   if(!menu->getTransition())
   {
       SDL_ShowCursor(SDL_DISABLE);
       menu->setTransition(new Transition(true,500,new MenuMultiplayer(menu->getInput(),menu->getInputMouse(),menu->getScreen())));
   }
}

BoutonMultiplayer::~BoutonMultiplayer()
{

}
