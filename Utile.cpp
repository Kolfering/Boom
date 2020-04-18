#include "Utile.h"
#include "MenuJeu.h"
#include <windows.h>
#include <Lmcons.h>

SDL_Texture* chargerSurface(string fichier)
{
    SDL_Surface* image = IMG_Load(fichier.c_str());
    SDL_Texture* newSurface = SDL_CreateTextureFromSurface(Jeu::getRenderer(),image);
    SDL_FreeSurface(image);
    return newSurface;
}


int genererNombreAleatoire(unsigned int nombre)
{
    std::uniform_int_distribution<int> distribution(1,nombre);
    return distribution(generator);
}

bool compareFrameManager(FrameManager fm1,FrameManager fm2)
{
    if(fm1.x == fm2.x && fm1.y == fm2.y && fm1.nbFrame == fm2.nbFrame && fm1.longueur == fm2.longueur && fm1.tempsFrame == fm2.tempsFrame && fm1.increment == fm2.increment)
        return true;
    return false;
}

string getUserName()
{
    char username[UNLEN+1];
    DWORD username_len = UNLEN+1;
    GetUserName(username, &username_len);
    std::string userName(username);
    return username;
}

int getTime()
{
    return SDL_GetTicks() - MenuJeu::getTempsPause();
}

/* ********************************************************************* */
/*obtenirPixel : permet de récupérer la couleur d'un pixel
Paramètres d'entrée/sortie :
SDL_Surface *surface : la surface sur laquelle on va récupérer la couleur d'un pixel
int x : la coordonnée en x du pixel à récupérer
int y : la coordonnée en y du pixel à récupérer

Uint32 resultat : la fonction renvoie le pixel aux coordonnées (x,y) dans la surface
*/
Uint32 obtenirPixel(SDL_Surface *surface, int x, int y)
{
    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    /* Ici p est l'adresse du pixel que l'on veut connaitre */
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
    switch(nbOctetsParPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            /*Suivant l'architecture de la machine*/
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;

        /*Ne devrait pas arriver, mais évite les erreurs*/
        default:
            return 0;
    }
}

/* ********************************************************************* */
/*definirPixel : permet de modifier la couleur d'un pixel
Paramètres d'entrée/sortie :
SDL_Surface *surface : la surface sur laquelle on va modifier la couleur d'un pixel
int x : la coordonnée en x du pixel à modifier
int y : la coordonnée en y du pixel à modifier
Uint32 pixel : le pixel à insérer
*/
void definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    /*Ici p est l'adresse du pixel que l'on veut modifier*/
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
    switch(nbOctetsParPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            /*Suivant l'architecture de la machine*/
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

int IsFullScreen(SDL_Window* window)
{
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) return true; // return true if surface is fullscreen
    return false; // Return false if surface is windowed
}


bool toggleScreenMode(SDL_Window* window)
{
    Uint32 flags = SDL_GetWindowFlags(window); // Get the video surface flags

    if (IsFullScreen(window))
    {
        // Swith to WINDOWED mode
        flags &= ~SDL_WINDOW_FULLSCREEN;
        if (SDL_SetWindowFullscreen(window,0) < 0) return false;
        return true;
    }

    // Swith to FULLSCREEN mode
    flags = flags|SDL_WINDOW_FULLSCREEN;
    if (SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN) < 0) return false;
    return true;
}

int UDPSendPacket(const void* data, UDPsocket udpSocket, int channel, int sizeofdata)
{
    UDPpacket* packet = SDLNet_AllocPacket(sizeofdata+1);
    memcpy(packet->data,data,sizeofdata+1);
    packet->len = sizeofdata+1;
    int sizeSend = SDLNet_UDP_Send(udpSocket,1,packet);
    SDLNet_FreePacket(packet);
    return sizeSend;
}

void trim(char* src)
{
    char *dst;

    for (dst=src; *src == ' '; src++) {;}

    if (dst==src) return;

    while ((*dst++ = *src++)) {;}

    return;
}



