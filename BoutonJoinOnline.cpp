#include "BoutonJoinOnline.h"
#include <SDL_ttf.h>
#include "Menu.h"
#include "Jeu.h"
#include "MenuJoinNetwork.h"

BoutonJoinOnline::BoutonJoinOnline(Menu* menu) : Bouton(menu)
{
    SDL_Surface* imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Join network game",COLOR_BOUTON_POLICE);
    image[0] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
    int w,h;
    SDL_QueryTexture(image[0],nullptr,nullptr,&w,&h);
    position.w = w;
    position.h = h;
    imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Join network game",COLOR_BOUTON_POLICE_FOCUS);
    image[1] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
}

void BoutonJoinOnline::onClic()
{
   if(!menu->getTransition())
   {
        Menu* m = nullptr;
        for(unsigned int i = 0;i<menu->getListeSousMenu().size();i++)
        {
            if((m = dynamic_cast<MenuJoinNetwork*>(menu->getListeSousMenu().at(i))))
            {
                Jeu::setMenu(m);
                break;
            }
        }

        if(!m)
            Jeu::setMenu(new MenuJoinNetwork(menu->getInput(),menu->getInputMouse(),menu->getScreen(), menu));

        menu->getInputMouse()->isMouseClicked = false;
   }
}

BoutonJoinOnline::~BoutonJoinOnline()
{

}
