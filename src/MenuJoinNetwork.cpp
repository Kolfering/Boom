#include "MenuJoinNetwork.h"
#include "BoutonRetour.h"
#include "BoutonJoinOnline.h"
#include "BoutonKiss_JoinStart.h"
#include "MenuPrepareNetworkGame.h"
#include "Jeu.h"
#include "Utile.h"
#include "Libelle.h"


MenuJoinNetwork::MenuJoinNetwork(Input* input,InputMouse* inputMouse,SDL_Window* screen,Menu* menuPar) : MenuTemplate(input,inputMouse,screen,menuPar,false)
{
   idMenu = IDMenu::MENU_JOINNETWORK;

   int w,h;
   BoutonRetour* boutonRetour = new BoutonRetour(this,MENU_MULTIPLAYER);

   SDL_QueryTexture(boutonRetour->getImage(),nullptr,nullptr,&w,&h);

   boutonRetour->getAdressePosition()->x = 0;
   boutonRetour->getAdressePosition()->y = WINDOW_HEIGHT - h;


   listeWidget["boutonRetour"] = boutonRetour;

   int w2,h2;
   SDL_Texture* logo = chargerSurface("../Images/BOOMLogo.png");
   SDL_QueryTexture(logo,nullptr,nullptr,&w2,&h2);
   SDL_Rect positionLogo = {WINDOW_WIDTH/2 - w2/2,h2/5,w2,h2};
   listeTexture[logo] = positionLogo;

   int w3,h3;
   Libelle* libelleTitle = new Libelle("Join Network Game",this,{255,255,255},40);
   SDL_QueryTexture(libelleTitle->getImage(),nullptr,nullptr,&w3,&h3);

   SDL_Rect pos = {(WINDOW_WIDTH-w3)/2 ,(WINDOW_HEIGHT-h3)/2,0,0};
   libelleTitle->setPosition(pos.x,pos.y);

   kiss_init(&kiss_objects,Jeu::getRenderer());
   button = {0};
   entryIP = {0};
   kiss_window window;
   kiss_window_new(&window, NULL, 1, 0, 0, 0,0);
   kiss_entry_new(&entryIP, &window, 1, "85.201.27.239",(WINDOW_WIDTH/2)-75,(WINDOW_HEIGHT/2)+50,150);
   kiss_button_new(&button, &window, "Join",(WINDOW_WIDTH-62)/2,entryIP.texty+50);

   network_client = new Network_Client();

   entryIP.visible = 1;
   listeWindow.push_back(&window);
   listeEntryBox.push_back(new EntryKiss(&entryIP));
   listeKissButton.push_back(new BoutonKiss_JoinStart(&button,&entryIP,network_client));
   listeWidget["TitreJoin"] = libelleTitle;



}

MenuJoinNetwork::~MenuJoinNetwork()
{
    kiss_clean(&kiss_objects);
    delete network_client;
}

void MenuJoinNetwork::handleEvents()
{
    if(network_client->isInitialized() && !network_client->isAccepted())
    {
       if(network_client->connectToServer())
       {
           if(!transition)
           {
                Menu* m = nullptr;
                for(unsigned int i = 0;i<listeSousMenu.size();i++)
                {
                    if((m = dynamic_cast<MenuPrepareNetworkGame*>(listeSousMenu.at(i))))
                    {
                        Jeu::setMenu(m);
                        break;
                    }
                }

            if(!m)
                Jeu::setMenu(new MenuPrepareNetworkGame(input,inputMouse,screen,this,network_client));
           }
       }
    }

}
