#include "MenuMultiplayer.h"
#include "BoutonRetour.h"
#include "BoutonGatherOnline.h"
#include "BoutonJoinOnline.h"
#include "Jeu.h"
#include "Utile.h"


MenuMultiplayer::MenuMultiplayer(Input* input,InputMouse* inputMouse,SDL_Window* screen) : MenuTemplate(input,inputMouse,screen)
{
   idMenu = IDMenu::MENU_MULTIPLAYER;

   int w,h,w2,h2,w3,h3,w4,h4;
   BoutonRetour* boutonRetour = new BoutonRetour(this,IDMenu::MENU_PRINCIPAL);
   BoutonGatherOnline* boutonGatherOnline = new BoutonGatherOnline(this);
   BoutonJoinOnline* boutonJoinOnline = new BoutonJoinOnline(this);

   SDL_QueryTexture(boutonRetour->getImage(),nullptr,nullptr,&w,&h);
   SDL_QueryTexture(boutonGatherOnline->getImage(),nullptr,nullptr,&w2,&h2);
   SDL_QueryTexture(boutonJoinOnline->getImage(),nullptr,nullptr,&w3,&h3);

   boutonRetour->getAdressePosition()->x = 0;
   boutonRetour->getAdressePosition()->y = WINDOW_HEIGHT - h;

   boutonGatherOnline->getAdressePosition()->x = WINDOW_WIDTH/2 - w2/2;
   boutonGatherOnline->getAdressePosition()->y = WINDOW_HEIGHT/2 - h2/2;

   boutonJoinOnline->getAdressePosition()->x = WINDOW_WIDTH/2 - w3/2;
   boutonJoinOnline->getAdressePosition()->y = boutonGatherOnline->getAdressePosition()->y + h2/2 + 30;

   SDL_Texture* logo = chargerSurface("../Images/BOOMLogo.png");
   SDL_QueryTexture(logo,nullptr,nullptr,&w4,&h4);
   SDL_Rect positionLogo = {WINDOW_WIDTH/2 - w4/2,h4/5,w4,h4};
   listeTexture[logo] = positionLogo;

   listeWidget["boutonRetour"] = boutonRetour;
   listeWidget["boutonGatherNetwork"] = boutonGatherOnline;
   listeWidget["boutonJoinNetwork"] = boutonJoinOnline;



}

MenuMultiplayer::~MenuMultiplayer()
{

}

void MenuMultiplayer::handleEvents()
{

}
