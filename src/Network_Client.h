#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <SDL_net.h>
#include "Menu.h"

class Network_Client{
private:
    IPaddress ipAddress;
    UDPsocket udpSocket;
    TCPsocket tcpSocket;
    SDLNet_SocketSet socketSet;
    bool connected;
    bool accepted;
    bool initialized;

public:
    Network_Client();
    ~Network_Client();
    void unload();
    bool connectToServer();
    bool isConnected();
    bool isAccepted();
    bool isInitialized();
    void reinitialize();
    void sendMessage(const char*);
    bool initializeToServer(const char*);
    void handleServerConnection(Menu*);

};

#endif // NETWORK_CLIENT_H
