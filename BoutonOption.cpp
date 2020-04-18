#include "BoutonOption.h"
#include <SDL_ttf.h>
#include "MenuOption.h"
#include "Menu.h"
#include "Jeu.h"

BoutonOption::BoutonOption(Menu* menu) : Bouton(menu)
{
    SDL_Surface* imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Options",COLOR_BOUTON_POLICE);
    image[0] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
    int w,h;
    SDL_QueryTexture(image[0],nullptr,nullptr,&w,&h);
    position.w = w;
    position.h = h;
    imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Options",COLOR_BOUTON_POLICE_FOCUS);
    image[1] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
}

void BoutonOption::onClic()
{
   if(!menu->getTransition())
   {
       SDL_ShowCursor(SDL_DISABLE);
       menu->setTransition(new Transition(true,500,new MenuOption(menu->getInput(),menu->getInputMouse(),menu->getScreen())));
   }
}

BoutonOption::~BoutonOption()
{

}
