#include "Network_Server.h"
#include <cstdio>
#include <iostream>
#include "Jeu.h"
#include "Utile.h"
#include "MenuPrepareNetworkGame.h"



Network_Server::Network_Server()
{
    tcpSocket = nullptr;
    tcpSocketCommunication = nullptr;
    socketSet = nullptr;
    udpSocket = nullptr;
    started = false;
    ipAddress = {0};
    connected = false;
}

bool Network_Server::stop()
{
    if(started)
    {
        if(tcpSocket)
            SDLNet_TCP_Close(tcpSocket);

        if(tcpSocketCommunication)
            SDLNet_TCP_Close(tcpSocketCommunication);

        if(udpSocket)
            SDLNet_UDP_Close(udpSocket);

        if(socketSet)
            SDLNet_FreeSocketSet(socketSet);

        SDLNet_Quit();
    }

    started = false;
    return true;
}


bool Network_Server::start()
{
    if(started)
        return started;

    std::cout << "Server initialisation... ";
    int errorStatus = SDLNet_Init();

    if(errorStatus == -1)
    {
        std::cout << "error" << std::endl;
        std::cout << SDLNet_GetError() << std::endl;
        started = false;
        return false;
    }

    std::cout << "done" <<std::endl;

    std::cout << "Resolving host IP... ";
    errorStatus = SDLNet_ResolveHost(&ipAddress,NULL,2712);

    if(errorStatus == -1)
    {
        std::cout << "error" << std::endl;
        std::cout << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        started = false;
        return false;
    }

     std::cout << "done" <<std::endl;
     std::cout << "Opening sockets for listening... ";
     tcpSocket = SDLNet_TCP_Open(&ipAddress);
     udpSocket = SDLNet_UDP_Open(2712);

     if(!tcpSocket || !udpSocket)
     {
        std::cout << "error" << std::endl;
        std::cout << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        started = false;

        if(tcpSocket)
            SDLNet_TCP_Close(tcpSocket);

        if(udpSocket)
            SDLNet_UDP_Close(udpSocket);

        return false;
     }

     std::cout << "done" <<std::endl;
     std::cout << "Allocate socket set... ";
     socketSet = SDLNet_AllocSocketSet(1);
     if(!socketSet)
     {
        std::cout << "error" << std::endl;
        std::cout << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        started = false;
        SDLNet_TCP_Close(tcpSocket);
        SDLNet_UDP_Close(udpSocket);
        return false;
     }

     std::cout << "done" <<std::endl;
     std::cout << "Server started..." << std::endl;



     started = true;


     return true;
}

bool Network_Server::handleClientConnection(Menu* menuCurrent)
{
    if(started && !connected) //Connect the guy
    {
        if(!tcpSocketCommunication)
        {
            UDPpacket* packetReturn = SDLNet_AllocPacket(5);

             if (SDLNet_UDP_Recv(udpSocket,packetReturn) > 0)
             {
                if(strcmp("ping",(const char*)packetReturn->data) == 0)
                {
                    SDLNet_UDP_Unbind(udpSocket,1);
                    SDLNet_UDP_Bind(udpSocket,1,&packetReturn->address);
                    int n = UDPSendPacket("pong",udpSocket,1,4);

                    if (n == 0)
                    {
                        std::cout << "error" << std::endl;
                        std::cout << SDLNet_GetError() << std::endl;
                    }
                }
             }

             SDLNet_FreePacket(packetReturn);


            tcpSocketCommunication = SDLNet_TCP_Accept(tcpSocket);
            if(tcpSocketCommunication)
            {
                int numused=SDLNet_TCP_AddSocket(socketSet, tcpSocketCommunication);

                if(numused==-1)
                {
                    std::cout << "can't add socket for listener... : " << SDLNet_GetError() << std::endl;
                    SDLNet_TCP_Close(tcpSocketCommunication);
                    return false;
                }
                    int len,result;
                    std::cout << "Player connected" << std::endl;
                    char *msg="You are connected dude !";
                    len=strlen(msg)+1;
                    SDLNet_TCP_Send(tcpSocketCommunication,msg,len);
                    connected = true;

                    MenuPrepareNetworkGame* menuPrepare;
                    if((menuPrepare = dynamic_cast<MenuPrepareNetworkGame*>(menuCurrent))!=nullptr)
                        menuPrepare->addPlayerConnected();
            }
        }

    }
    else //Check what's going on with the other guy
    {

        int numready = SDLNet_CheckSockets(socketSet, 0);

        if(numready==-1)
        {
            std::cout << "can't check the socket set for response... : " << SDLNet_GetError() << std::endl;
            perror("SDLNet_CheckSockets");
            return false;
        }
        else if(numready)
        {
            if(SDLNet_SocketReady(tcpSocketCommunication))
            {
              char data[128];
              int result=SDLNet_TCP_Recv(tcpSocketCommunication,data,128);
              if(result<=0)
              {
                    std::cout << "Player disconnected" << std::endl;
                    SDLNet_TCP_DelSocket(socketSet,tcpSocketCommunication);
                    SDLNet_TCP_Close(tcpSocketCommunication);
                    tcpSocketCommunication = nullptr;
                    connected = false;

                    MenuPrepareNetworkGame* menuPrepare;
                    if((menuPrepare = dynamic_cast<MenuPrepareNetworkGame*>(menuCurrent))!=nullptr)
                        menuPrepare->addPlayerDisconnected();
              }

              else
              {
                  MenuPrepareNetworkGame* menuPrepare;
                  if((menuPrepare = dynamic_cast<MenuPrepareNetworkGame*>(menuCurrent))!=nullptr)
                        kiss_array_appendstring(menuPrepare->getTextboxChat()->array,0,"Him:",data);
              }

            }
        }

    }

    return true;
}

void Network_Server::sendMessage(const char* message)
{
    if(tcpSocketCommunication && connected)
        SDLNet_TCP_Send(tcpSocketCommunication,message,128);
}

bool Network_Server::isStarted()
{
    return started;
}

Network_Server::~Network_Server()
{
    stop();
}
