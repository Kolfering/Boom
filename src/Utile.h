#ifndef UTILE_H
#define UTILE_H
#include <iostream>
#include <SDL_image.h>
#include "Jeu.h"
#include "Case.h"
#include <random>
#include <chrono>
#include "Enumeration.h"
#include <SDL_net.h>


#if SDL_BYTEORDER == SDL_BIG_ENDIAN

    #define MASQUE_ROUGE 0xff000000
    #define MASQUE_VERT  0x00ff0000
    #define MASQUE_BLEU  0x0000ff00
    #define MASQUE_ALPHA 0x000000ff

#else

    #define MASQUE_ROUGE 0x000000ff
    #define MASQUE_VERT  0x0000ff00
    #define MASQUE_BLEU  0x00ff0000
    #define MASQUE_ALPHA 0xff000000

#endif

using std::string;

static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
SDL_Texture* chargerSurface(string);
string getUserName();
int genererNombreAleatoire(unsigned int);
bool compareFrameManager(FrameManager,FrameManager);
Uint32 obtenirPixel(SDL_Surface*, int, int);
void definirPixel(SDL_Surface*, int, int, Uint32);
int getTime();
bool toggleScreenMode(SDL_Window*);
bool isFullScreen(SDL_Window*);
int UDPSendPacket(const void* data, UDPsocket udpSocket, int channel, int sizeofdata);



#endif // UTILE_H
