#include "BoutonKiss_GatherStart.h"
#include <Windows.h>
#include "Jeu.h"
#include "MenuPrepareNetworkGame.h"


BoutonKiss_GatherStart::BoutonKiss_GatherStart(kiss_button* kiss_b,kiss_entry* kiss_e) : BoutonKiss(kiss_b)
{
    kiss_entry_NameGame = kiss_e;
}

BoutonKiss_GatherStart::~BoutonKiss_GatherStart()
{

}

void BoutonKiss_GatherStart::onClick()
{
    Network_Server* network_server = new Network_Server();
    if(network_server->start())
    {
       if(!Jeu::getMenu()->getTransition())
       {
            Menu* m = nullptr;
            for(unsigned int i = 0;i<Jeu::getMenu()->getListeSousMenu().size();i++)
            {
                if((m = dynamic_cast<MenuPrepareNetworkGame*>(Jeu::getMenu()->getListeSousMenu().at(i))))
                {
                    Jeu::setMenu(m);
                    break;
                }
            }

            if(!m)
                Jeu::setMenu(new MenuPrepareNetworkGame(Jeu::getMenu()->getInput(),Jeu::getMenu()->getInputMouse(),Jeu::getMenu()->getScreen(), Jeu::getMenu(),network_server));

            Jeu::getMenu()->getInputMouse()->isMouseClicked = false;
       }
    }

}
