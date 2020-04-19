#include "MenuGatherNetwork.h"
#include "BoutonRetour.h"
#include "BoutonGatherOnline.h"
#include "BoutonKiss_GatherStart.h"
#include "Jeu.h"
#include "Libelle.h"
#include "Utile.h"


MenuGatherNetwork::MenuGatherNetwork(Input* input,InputMouse* inputMouse,SDL_Window* screen,Menu* menuPar) : MenuTemplate(input,inputMouse,screen,menuPar,false)
{

  idMenu = IDMenu::MENU_GATHERNETWORK;

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
   Libelle* libelleTitle = new Libelle("Gather Network Game",this,{255,255,255},40);
   SDL_QueryTexture(libelleTitle->getImage(),nullptr,nullptr,&w3,&h3);

   SDL_Rect pos = {(WINDOW_WIDTH-w3)/2 ,(WINDOW_HEIGHT-h3)/2,0,0};
   libelleTitle->setPosition(pos.x,pos.y);

   kiss_init(&kiss_objects,Jeu::getRenderer());
   button = {0};
   entryNameGame = {0};
   kiss_window window;
   kiss_window_new(&window, NULL, 1, 0, 0, 0,0);
   std::string userName = getUserName()+"'s Game";
   kiss_entry_new(&entryNameGame, &window, 1, userName.c_str(),(WINDOW_WIDTH/2)-125,(WINDOW_HEIGHT/2)+50,250);
   kiss_button_new(&button, &window, "Start",(WINDOW_WIDTH-62)/2,entryNameGame.texty+50);

   entryNameGame.visible = 1;
   listeWindow.push_back(&window);
   listeEntryBox.push_back(new EntryKiss(&entryNameGame));
   listeKissButton.push_back(new BoutonKiss_GatherStart(&button,&entryNameGame));
   listeWidget["TitreGather"] = libelleTitle;
}

MenuGatherNetwork::~MenuGatherNetwork()
{
    kiss_clean(&kiss_objects);
}

void MenuGatherNetwork::handleEvents()
{

}
