#ifndef ENNEMISMOULDER_H
#define ENNEMISMOULDER_H
#include "Ennemi.h"

class EnnemiSmoulder : public Ennemi{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonRandom;
    static Mix_Chunk* sonMort;
    int range;
public:
    EnnemiSmoulder(Case*);
    ~EnnemiSmoulder();
    void deplacerEnnemi();
    SDL_Texture* getImage();
    bool attaque();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonRandom();
    static void initialiserEnnemiSmoulder();
    static void releaseEnnemiSmoulder();
    void accelerer();

};

#endif // ENNEMISMOULDER_H
