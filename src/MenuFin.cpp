#include "MenuFin.h"
#include "Utile.h"
#include "Libelle.h"
#include "MenuPrincipal.h"
#include "Utile.h"



MenuFin::MenuFin(Input* input,InputMouse* inputMouse,SDL_Window* screen) : MenuTemplate(input,inputMouse,screen)
{

    idMenu = IDMenu::MENU_FIN;

    std::string userName(getUserName());

    int w,h,w2,h2,w3,h3,w4,h4,h5,w5,w6,h6,w7,h7,w8,h8,w9,h9,w10,h10;
    Libelle* libelleRights = new Libelle("Press a key or click to continue",this,{59,23,251},20,true);
    Libelle* libelleMessage = new Libelle("YOU HAVE WON "+userName+"!!",this,{182,30,3},32,true);
    Libelle* libelleMessage2 = new Libelle("You reached the last of the alien infested zones",this,{182,30,3},18,true);
    Libelle* libelleMessage3 = new Libelle("and defeated the Big Alien Boss himself.",this,{182,30,3},18,true);
    Libelle* libelleMessage4 = new Libelle("Not exactly like eating a piece of cake, eh?",this,{182,30,3},18,true);
    Libelle* libelleMessage5 = new Libelle("Well, now the Earth is safe, you are a hero,",this,{182,30,3},18,true);
    Libelle* libelleMessage6 = new Libelle("interviews, autographs and all the rest.",this,{182,30,3},18,true);

    Libelle* libelleMessage7 = new Libelle("But, wait a minute?",this,{182,30,3},18,true);
    Libelle* libelleMessage8 = new Libelle("What is that strange thing in the sky?",this,{182,30,3},18,true);
    Libelle* libelleMessage9 = new Libelle("OH NO! NOT AGAIN!!",this,{182,30,3},18,true);

    /*Libelle* libelleMessage7 = new Libelle("Well.. In fact you barely ended the demo.",this,{182,30,3},18,true);
    Libelle* libelleMessage8 = new Libelle("There are 70 levels more waiting for you.",this,{182,30,3},18,true);
    Libelle* libelleMessage9 = new Libelle("Ask the genius developer to play the full version.",this,{182,30,3},18,true);*/



    backGround = chargerSurface("Images/BGFin.png");


    SDL_QueryTexture(libelleRights->getImage(),nullptr,nullptr,&w5,&h5);
    SDL_QueryTexture(libelleMessage->getImage(),nullptr,nullptr,&w4,&h4);
    SDL_QueryTexture(libelleMessage->getImage(),nullptr,nullptr,&w3,&h3);
    SDL_QueryTexture(libelleMessage->getImage(),nullptr,nullptr,&w2,&h2);
    SDL_QueryTexture(libelleMessage->getImage(),nullptr,nullptr,&w,&h);
    SDL_QueryTexture(libelleMessage->getImage(),nullptr,nullptr,&w6,&h6);
    SDL_QueryTexture(libelleMessage->getImage(),nullptr,nullptr,&w7,&h7);
    SDL_QueryTexture(libelleMessage->getImage(),nullptr,nullptr,&w8,&h8);
    SDL_QueryTexture(libelleMessage->getImage(),nullptr,nullptr,&w9,&h9);
    SDL_QueryTexture(libelleMessage->getImage(),nullptr,nullptr,&w10,&h10);


    libelleRights->setPosition(WINDOW_WIDTH/2 - libelleRights->getTaille().x/2,WINDOW_HEIGHT - libelleRights->getTaille().y);
    libelleMessage->setPosition(WINDOW_WIDTH/2 - libelleMessage->getTaille().x/2,20);
    libelleMessage2->setPosition(WINDOW_WIDTH/2 - libelleMessage2->getTaille().x/2,libelleMessage->getPosition().y+libelleMessage->getTaille().y + 30);
    libelleMessage3->setPosition(WINDOW_WIDTH/2 - libelleMessage3->getTaille().x/2,libelleMessage2->getPosition().y+libelleMessage2->getTaille().y + 10);
    libelleMessage4->setPosition(WINDOW_WIDTH/2 - libelleMessage4->getTaille().x/2,libelleMessage3->getPosition().y+libelleMessage3->getTaille().y + 10);
    libelleMessage5->setPosition(WINDOW_WIDTH/2 - libelleMessage5->getTaille().x/2,libelleMessage4->getPosition().y+libelleMessage4->getTaille().y + 10);
    libelleMessage6->setPosition(WINDOW_WIDTH/2 - libelleMessage6->getTaille().x/2,libelleMessage5->getPosition().y+libelleMessage5->getTaille().y + 10);
    libelleMessage7->setPosition(WINDOW_WIDTH/2 - libelleMessage7->getTaille().x/2,libelleMessage6->getPosition().y+libelleMessage6->getTaille().y + 40);
    libelleMessage8->setPosition(WINDOW_WIDTH/2 - libelleMessage8->getTaille().x/2,libelleMessage7->getPosition().y+libelleMessage7->getTaille().y + 10);
    libelleMessage9->setPosition(WINDOW_WIDTH/2 - libelleMessage9->getTaille().x/2,libelleMessage8->getPosition().y+libelleMessage8->getTaille().y + 10);

    listeWidget["libelleRights"] = libelleRights;
    listeWidget["libelleMessageWin"] = libelleMessage;
    listeWidget["libelleMessage2"] = libelleMessage2;
    listeWidget["libelleMessage3"] = libelleMessage3;
    listeWidget["libelleMessage4"] = libelleMessage4;
    listeWidget["libelleMessage5"] = libelleMessage5;
    listeWidget["libelleMessage6"] = libelleMessage6;
    listeWidget["libelleMessage7"] = libelleMessage7;
    listeWidget["libelleMessage8"] = libelleMessage8;
    listeWidget["libelleMessage9"] = libelleMessage9;


}

void MenuFin::handleEvents()
{
    if(!transition)
    {
        if(input->isKeyDown || inputMouse->isMouseClicked)
                setTransition(new Transition(true,800,new MenuPrincipal(input,inputMouse,screen)));
    }

}


MenuFin::~MenuFin()
{

}



