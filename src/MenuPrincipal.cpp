#include "MenuPrincipal.h"
#include "Utile.h"
#include "BoutonNouvellePartie.h"
#include "BoutonContinue.h"
#include "BoutonOption.h"
#include "BoutonQuitter.h"
#include "BoutonMultiplayer.h"
#include "Libelle.h"

MenuPrincipal::MenuPrincipal(Input* input,InputMouse* inputMouse,SDL_Window* screen) : MenuTemplate(input,inputMouse,screen)
{
    idMenu = IDMenu::MENU_PRINCIPAL;

    int w,h,w2,h2,w3,h3,w4,h4,h5,w5,w6,h6,w7,h7;
    BoutonNouvellePartie* boutonNouvellePartie = new BoutonNouvellePartie(this);
    BoutonContinue* boutonContinue = new BoutonContinue(this);
    BoutonMultiplayer* boutonMultiplayer = new BoutonMultiplayer(this);
    BoutonOption* boutonOption = new BoutonOption(this);
    BoutonQuitter* boutonQuitter = new BoutonQuitter(this);
    Libelle* libelleRights = new Libelle("Made by Ben Corporation - Based on the original game",this,{255,255,255},10);

    SDL_Texture* logo = chargerSurface("../Images/BOOMLogo.png");

    SDL_QueryTexture(boutonNouvellePartie->getImage(),nullptr,nullptr,&w,&h);
    SDL_QueryTexture(boutonContinue->getImage(),nullptr,nullptr,&w2,&h2);
    SDL_QueryTexture(boutonMultiplayer->getImage(),nullptr,nullptr,&w7,&h7);
    SDL_QueryTexture(boutonOption->getImage(),nullptr,nullptr,&w3,&h3);
    SDL_QueryTexture(boutonQuitter->getImage(),nullptr,nullptr,&w4,&h4);
    SDL_QueryTexture(libelleRights->getImage(),nullptr,nullptr,&w5,&h5);
    SDL_QueryTexture(logo,nullptr,nullptr,&w6,&h6);

    SDL_Rect positionLogo = {WINDOW_WIDTH/2 - w6/2,h6/5,w6,h6};

    boutonNouvellePartie->getAdressePosition()->x = WINDOW_WIDTH/2 - w/2;
    boutonNouvellePartie->getAdressePosition()->y = WINDOW_HEIGHT/2 - h/2 - 40;

    boutonContinue->getAdressePosition()->x = WINDOW_WIDTH/2 - w2/2;
    boutonContinue->getAdressePosition()->y = boutonNouvellePartie->getAdressePosition()->y + h2/2 + 20;

    boutonMultiplayer->getAdressePosition()->x = WINDOW_WIDTH/2 - w2/2;
    boutonMultiplayer->getAdressePosition()->y = boutonContinue->getAdressePosition()->y + h2/2 + 20;

    boutonOption->getAdressePosition()->x = WINDOW_WIDTH/2 - w2/2;
    boutonOption->getAdressePosition()->y = boutonMultiplayer->getAdressePosition()->y + h2/2 + 20;

    boutonQuitter->getAdressePosition()->x = WINDOW_WIDTH/2 - w2/2;
    boutonQuitter->getAdressePosition()->y = boutonOption->getAdressePosition()->y + h2/2 + 20;

    libelleRights->setPosition(WINDOW_WIDTH/2 - libelleRights->getTaille().x/2,WINDOW_HEIGHT - libelleRights->getTaille().y);

    listeWidget["boutonNouvellePartie"] = boutonNouvellePartie;
    listeWidget["boutonContinue"] = boutonContinue;
    listeWidget["boutonMultiplayer"] = boutonMultiplayer;
    listeWidget["boutonOption"] = boutonOption;
    listeWidget["boutonQuit"] = boutonQuitter;
    listeWidget["libelleRights"] = libelleRights;

    listeTexture[logo] = positionLogo;
}

MenuPrincipal::~MenuPrincipal()
{

}



