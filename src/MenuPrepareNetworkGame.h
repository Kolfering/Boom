#ifndef MENUPREPARENETWORKGAME_H
#define MENUPREPARENETWORKGAME_H

#include "MenuTemplate.h"
#include "Network_Client.h"
#include "Network_Server.h"

class MenuPrepareNetworkGame : public MenuTemplate {
private:
    bool isHost;
    Network_Client* network_client;
    Network_Server* network_server;
    kiss_array kiss_objects;
    kiss_array kiss_arrayTextBoxChat;
    kiss_textbox kiss_textboxChat;
    kiss_entry kiss_entryChat;
    kiss_vscrollbar kiss_vscrollbarChat;
    SDL_Texture* playerConnectedTexture;
    SDL_Rect positionPlayerConnectedTexture;
    void initMenu(Menu*);
public:
    MenuPrepareNetworkGame(Input*,InputMouse*,SDL_Window*,Menu*,Network_Server*);
    MenuPrepareNetworkGame(Input*,InputMouse*,SDL_Window*,Menu*,Network_Client*);
    void addPlayerConnected();
    void addPlayerDisconnected();
    kiss_textbox* getTextboxChat();
    ~MenuPrepareNetworkGame();
    void handleEvents();

};



#endif // MENUPREPARENETWORKGAME_H
