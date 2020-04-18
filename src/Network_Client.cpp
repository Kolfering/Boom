#include "Network_Client.h"
#include <cstdio>
#include <iostream>
#include "Jeu.h"
#include "Utile.h"
#include "MenuPrepareNetworkGame.h"

bool Network_Client::initializeToServer(const char* ip)
{
    initialized = false;
    socketSet = nullptr;
    tcpSocket = nullptr;
    ipAddress = {0};
    connected = false;
    accepted = false;

    std::cout << "Network initialisation... ";
    int errorStatus  = SDLNet_Init();

    if(errorStatus == -1)
    {
        std::cout << "error" << std::endl;
        std::cout << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return false;
    }

    std::cout << "done" << std::endl;
    errorStatus = SDLNet_ResolveHost(&ipAddress,ip,2712);
    std::cout << "Resolving host ip... ";

    if(errorStatus == -1)
    {
        std::cout << "error" << std::endl;
        std::cout << "Cannot resolve host ip : " << ip << std::endl;
        return false;
    }

    std::cout << "done" << std::endl;

    std::cout << "Opening socket for connection to the server... ";
    udpSocket = SDLNet_UDP_Open(0);

    if(!udpSocket)
    {
        std::cout << "error" << std::endl;
        std::cout << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return false;
    }

    std::cout << "done" << std::endl;
    std::cout << "Binding UDP socket to channel... ";
    errorStatus = SDLNet_UDP_Bind(udpSocket,1,&ipAddress);


    if(errorStatus != 1)
    {
        std::cout << "error" << std::endl;
        std::cout << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        SDLNet_UDP_Close(udpSocket);
        return false;
    }

    std::cout << "done" <<std::endl;
    std::cout << "Contacting server... ";

    initialized = true;
    return true;
}

bool Network_Client::connectToServer()
{

        if(!initialized)
            return false;

        if(!connected)
        {

            UDPpacket* packetReturn = SDLNet_AllocPacket(sizeof(void*)+1);

             if (SDLNet_UDP_Recv(udpSocket,packetReturn) > 0)
             {
                if(strcmp("pong",(const char*)packetReturn->data) == 0)
                {
                    std::cout << "done" << std::endl;
                    std::cout << "Establishing connection with the server... ";
                    tcpSocket = SDLNet_TCP_Open(&ipAddress);

                    if(!tcpSocket)
                    {
                         std::cout << "error" << std::endl;
                         std::cout << SDLNet_GetError() << std::endl;
                         SDLNet_FreePacket(packetReturn);
                         return false;
                    }

                    std::cout << "done" << std::endl;
                    connected = true;
                }

             }
             else
             {
                 int n = UDPSendPacket("ping",udpSocket,1,4);

                    if (n == 0)
                    {
                        std::cout << "error" << std::endl;
                        std::cout << SDLNet_GetError() << std::endl;
                        return false;
                    }
             }

             SDLNet_FreePacket(packetReturn);



        }

    if(!accepted && connected)
    {
        if(!socketSet)
        {
            socketSet = SDLNet_AllocSocketSet(1);

            if(!socketSet)
            {
                std::cout << "error during allocation of socket set : " << SDLNet_GetError() << std::endl;
                return false;
            }

            int numused=SDLNet_TCP_AddSocket(socketSet, tcpSocket);

            if(numused==-1)
            {
                std::cout << "can't add socket for listener... : " << SDLNet_GetError() << std::endl;
                return false;
            }
        }

        int numready;
        int result;
        char msg[32];
        numready=SDLNet_CheckSockets(socketSet, 0);

        if(numready==-1)
        {
            std::cout << "can't check the socket set for response... : " << SDLNet_GetError() << std::endl;
            perror("SDLNet_CheckSockets");
            return false;
        }
        else if(numready)
        {
            if(SDLNet_SocketReady(tcpSocket))
            {
              result=SDLNet_TCP_Recv(tcpSocket,msg,32);
              if(result<=0)
              {
                    // An error may have occured, but sometimes you can just ignore it
                    // It may be good to disconnect sock because it is likely invalid now.
              }

              if(strcmp(msg,"You are connected dude !")==0)
              {
                accepted = true;
                return true;
              }
            }
        }

    }

    return false;

}

void Network_Client::unload()
{
    if(tcpSocket)
        SDLNet_TCP_Close(tcpSocket);
    if(socketSet)
        SDLNet_FreeSocketSet(socketSet);
    if(udpSocket)
        SDLNet_UDP_Close(udpSocket);


   initialized = false;
   socketSet = nullptr;
   tcpSocket = nullptr;
   udpSocket = nullptr;
   ipAddress = {0};
   connected = false;
   accepted = false;
   SDLNet_Quit();
}

void Network_Client::sendMessage(const char* message)
{
    if(tcpSocket && connected && accepted)
        SDLNet_TCP_Send(tcpSocket,message,128);
}

void Network_Client::handleServerConnection(Menu* menuCurrent)
{

    if(connected)
    {

        int numready;
        int result;
        char msg[128];
        numready=SDLNet_CheckSockets(socketSet, 0);

        if(numready==-1)
        {
            std::cout << "can't check the socket set for response... : " << SDLNet_GetError() << std::endl;
            perror("SDLNet_CheckSockets");
            return;
        }
        else if(numready)
        {
            if(SDLNet_SocketReady(tcpSocket))
            {
              result=SDLNet_TCP_Recv(tcpSocket,msg,128);
              if(result<=0)
              {
                    SDLNet_TCP_DelSocket(socketSet,tcpSocket);
                    SDLNet_TCP_Close(tcpSocket);
                    SDLNet_FreeSocketSet(socketSet);
                    socketSet = nullptr;
                    tcpSocket = nullptr;
                    connected = false;

                    MenuPrepareNetworkGame* menuPrepare;
                    if((menuPrepare = dynamic_cast<MenuPrepareNetworkGame*>(menuCurrent))!=nullptr)
                        menuPrepare->addPlayerDisconnected();
              }
              else
              {
                  MenuPrepareNetworkGame* menuPrepare;
                  if((menuPrepare = dynamic_cast<MenuPrepareNetworkGame*>(menuCurrent))!=nullptr)
                        kiss_array_appendstring(menuPrepare->getTextboxChat()->array,0,"Him:",msg);
              }

            }
        }
    }

}

bool Network_Client::isAccepted()
{
    return accepted;
}

bool Network_Client::isConnected()
{
    return connected;
}

bool Network_Client::isInitialized()
{
    return initialized;
}

Network_Client::Network_Client()
{
    initialized = false;
    socketSet = nullptr;
    tcpSocket = nullptr;
    ipAddress = {0};
    connected = false;
    accepted = false;
    udpSocket = nullptr;
}

Network_Client::~Network_Client()
{
    unload();
}
