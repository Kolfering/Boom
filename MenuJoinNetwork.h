#ifndef MENUJOINNETWORK_H
#define MENUJOINNETWORK_H

#include "MenuTemplate.h"
#include "Network_Client.h"

class MenuJoinNetwork : public MenuTemplate {
private:
    Network_Client* network_client;
    kiss_array kiss_objects;
    kiss_button button;
    kiss_entry entryIP;
public:
    MenuJoinNetwork(Input*,InputMouse*,SDL_Window*, Menu*);
    ~MenuJoinNetwork();
    void handleEvents();

};


#endif // MENUJOINNETWORK_H
