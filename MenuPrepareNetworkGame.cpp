#include "MenuPrepareNetworkGame.h"
#include "BoutonRetour.h"
#include "BoutonGatherOnline.h"
#include "BoutonKiss_GatherStart.h"
#include "Jeu.h"
#include "Libelle.h"
#include "Utile.h"
#include "EntryKissNetworkChat.h"


MenuPrepareNetworkGame::MenuPrepareNetworkGame(Input* input,InputMouse* inputMouse,SDL_Window* screen, Menu* menuPar, Network_Server* network_server) : MenuTemplate(input,inputMouse,screen, menuPar,false)
{
    isHost = true;
    this->network_server = network_server;
    initMenu(menuPar);
}

MenuPrepareNetworkGame::MenuPrepareNetworkGame(Input* input,InputMouse* inputMouse,SDL_Window* screen, Menu* menuPar, Network_Client* network_client) : MenuTemplate(input,inputMouse,screen, menuPar,false)
{
    isHost = false;
    this->network_client = network_client;
    initMenu(menuPar);
}

void MenuPrepareNetworkGame::initMenu(Menu* menuPar)
{
   idMenu = IDMenu::MENU_PREPARENETWORKGAME;

   int w,h;
   BoutonRetour* boutonRetour = new BoutonRetour(this,menuPar->getIDMenu());

   SDL_QueryTexture(boutonRetour->getImage(),nullptr,nullptr,&w,&h);

   boutonRetour->getAdressePosition()->x = 0;
   boutonRetour->getAdressePosition()->y = WINDOW_HEIGHT - h;

   listeWidget["boutonRetour"] = boutonRetour;

   kiss_init(&kiss_objects,Jeu::getRenderer());
   kiss_textboxChat = {0};
   kiss_vscrollbarChat = {0};
   kiss_entryChat = {0};
   kiss_array_new(&kiss_arrayTextBoxChat);
   kiss_array_append(&kiss_objects, ARRAY_TYPE, &kiss_arrayTextBoxChat);

   kiss_window window;
   kiss_window_new(&window, NULL, 1, 0, 0, 0,0);
   window.visible = 1;

   kiss_textbox_new(&kiss_textboxChat,&window,1,&kiss_arrayTextBoxChat,WINDOW_WIDTH/2 - 250,150,500,200);
   kiss_vscrollbar_new(&kiss_vscrollbarChat, &window, kiss_textboxChat.rect.x + kiss_textboxChat.rect.w, kiss_textboxChat.rect.y, kiss_textboxChat.rect.h);
   kiss_vscrollbarChat.visible = 1;
   kiss_vscrollbarChat.textbox = &kiss_textboxChat;
   kiss_array_new(kiss_textboxChat.array);

   kiss_entry_new(&kiss_entryChat, &window, 1, "",kiss_textboxChat.rect.x,kiss_textboxChat.rect.y + kiss_textboxChat.rect.h + 5,500);

   int w2,h2;
   SDL_Texture* firstPlayerIcone = chargerSurface("Images/Player1Head.png");
   SDL_QueryTexture(firstPlayerIcone,nullptr,nullptr,&w2,&h2);
   SDL_Rect positionFirstPlayerIcone = {kiss_textboxChat.rect.x,kiss_textboxChat.rect.y - h2 ,w2,h2};
   listeTexture[firstPlayerIcone] = positionFirstPlayerIcone;

   int w4,h4;
   playerConnectedTexture = chargerSurface("Images/Player2Head.png");
   SDL_QueryTexture(playerConnectedTexture,nullptr,nullptr,&w4,&h4);
   positionPlayerConnectedTexture = {positionFirstPlayerIcone.x + positionFirstPlayerIcone.w ,positionFirstPlayerIcone.y,w4,h4};

   if(!isHost) listeTexture[playerConnectedTexture] = positionPlayerConnectedTexture;

   int w3,h3;
   Libelle* libelleTitle = new Libelle("Gather Network Game",this,{255,255,255},40);
   SDL_QueryTexture(libelleTitle->getImage(),nullptr,nullptr,&w3,&h3);

   SDL_Rect pos = {(WINDOW_WIDTH-w3)/2 ,50,0,0};
   libelleTitle->setPosition(pos.x,pos.y);
   listeWidget["TitreGather"] = libelleTitle;

   kiss_entryChat.visible = 1;

   if(isHost)
        listeEntryBox.push_back(new EntryKissNetworkChat(&kiss_entryChat,&kiss_textboxChat,network_server));
   else
        listeEntryBox.push_back(new EntryKissNetworkChat(&kiss_entryChat,&kiss_textboxChat,network_client));

   listeTextBox.push_back(&kiss_textboxChat);
   listeVScrollbar.push_back(&kiss_vscrollbarChat);
   listeWindow.push_back(&window);

}

void MenuPrepareNetworkGame::addPlayerConnected()
{
    std::map<SDL_Texture*,SDL_Rect>::iterator it = listeTexture.find(playerConnectedTexture);
    if (it == listeTexture.end())
        listeTexture[playerConnectedTexture] = positionPlayerConnectedTexture;
}

void MenuPrepareNetworkGame::addPlayerDisconnected()
{
    std::map<SDL_Texture*,SDL_Rect>::iterator it = listeTexture.find(playerConnectedTexture);
    if (it != listeTexture.end())
        listeTexture.erase(it);
}

kiss_textbox* MenuPrepareNetworkGame::getTextboxChat()
{
    return &kiss_textboxChat;
}

MenuPrepareNetworkGame::~MenuPrepareNetworkGame()
{
    kiss_clean(&kiss_objects);

    addPlayerConnected();

    if(isHost)
        delete network_server;
    else
        network_client->unload();

}

void MenuPrepareNetworkGame::handleEvents()
{
    if(isHost)
        network_server->handleClientConnection(this);
    else
        network_client->handleServerConnection(this);
}
