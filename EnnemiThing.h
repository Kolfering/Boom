#ifndef ENNEMITHING_H
#define ENNEMITHING_H
#include "Ennemi.h"

class EnnemiThing : public Ennemi{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonMort;
    static Mix_Chunk* sonRandom;
public:
    EnnemiThing(Case*);
    ~EnnemiThing();
    void deplacerEnnemi();
    SDL_Texture* getImage();
    bool attaque();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonRandom();
    static void initialiserEnnemiThing();
    static void releaseEnnemiThing();

};


#endif // ENNEMITHING_H
