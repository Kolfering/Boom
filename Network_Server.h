#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H


#include <SDL_net.h>
#include "Menu.h"

class Network_Server{
private:
    IPaddress ipAddress;
    UDPsocket udpSocket;
    TCPsocket tcpSocket;
    TCPsocket tcpSocketCommunication;
    SDLNet_SocketSet socketSet;
    bool started;
    bool connected;

public:
    Network_Server();
    ~Network_Server();
    bool start();
    bool stop();
    bool isStarted();
    void sendMessage(const char*);
    bool handleClientConnection(Menu*);

};

#endif // NETWORK_SERVER_H
