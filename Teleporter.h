#ifndef TELEPORTER_H
#define TELEPORTER_H
#include "Case.h"
#include "Personnage.h"
#define TEMPS_TELEPORTER_REPOS 2000
#define NB_FRAME_TELEPORTER 8
#define TAILLE_FRAME_TELEPORTER 32
#define TEMPS_FRAME_TELEPORTER 70

class Teleporter{
private:
    Case* caseTeleporter;
    static SDL_Texture* image;
    SDL_Rect antiPosition;
    bool accessible;
    unsigned int tempsDernierAcces;
    unsigned int tempsBefore;
    unsigned int counter;
    unsigned int indice;
    unsigned int indiceFrere;

public:
    Teleporter(Case*,unsigned int,unsigned int);
    ~Teleporter();
    void execute();
    bool teleporter(Personnage*);
    static void initialiserTeleporter();
    static void releaseTeleporter();
    unsigned int getIndice();
    unsigned int getIndiceFrere();
    SDL_Texture* getImage();
    Case* getCase();
    void setCase(Case*);
    void setAccessible(bool);
    bool isAccessible();
    void setTempsDernierAcces(unsigned int);
    SDL_Rect* getAntiPosition();
};

#endif // TELEPORTER_H
