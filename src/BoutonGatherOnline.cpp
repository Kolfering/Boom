#include "BoutonGatherOnline.h"
#include <SDL_ttf.h>
#include "Menu.h"
#include "Jeu.h"
#include "MenuGatherNetwork.h"

BoutonGatherOnline::BoutonGatherOnline(Menu* menu) : Bouton(menu)
{
    SDL_Surface* imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Gather network game",COLOR_BOUTON_POLICE);
    image[0] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
    int w,h;
    SDL_QueryTexture(image[0],nullptr,nullptr,&w,&h);
    position.w = w;
    position.h = h;
    imageTmp = TTF_RenderText_Blended(menu->getPolice(),"Gather network game",COLOR_BOUTON_POLICE_FOCUS);
    image[1] = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageTmp);
    SDL_FreeSurface(imageTmp);
}

void BoutonGatherOnline::onClic()
{
   if(!menu->getTransition())
   {
        Menu* m = nullptr;
        for(unsigned int i = 0;i<menu->getListeSousMenu().size();i++)
        {
            if((m = dynamic_cast<MenuGatherNetwork*>(menu->getListeSousMenu().at(i))))
            {
                Jeu::setMenu(m);
                break;
            }
        }

        if(!m)
            Jeu::setMenu(new MenuGatherNetwork(menu->getInput(),menu->getInputMouse(),menu->getScreen(),menu));

        menu->getInputMouse()->isMouseClicked = false;
   }
}

BoutonGatherOnline::~BoutonGatherOnline()
{

}
