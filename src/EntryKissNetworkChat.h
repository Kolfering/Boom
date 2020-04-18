#ifndef ENTRYKISSNETWORKCHAT_H
#define ENTRYKISSNETWORKCHAT_H

#include "EntryKiss.h"
#include "Input.h"
#include "Network_Client.h"
#include "Network_Server.h"

class EntryKissNetworkChat : public EntryKiss{
private :
    kiss_textbox* textbox;
    Network_Client* network_client;
    Network_Server* network_server;

public :
    EntryKissNetworkChat(kiss_entry*,kiss_textbox*,Network_Client*);
    EntryKissNetworkChat(kiss_entry*,kiss_textbox*,Network_Server*);
    ~EntryKissNetworkChat();
    void event(Input*, InputMouse*);

};

#endif // ENTRYKISSNETWORKCHAT_H
