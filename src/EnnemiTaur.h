#ifndef ENNEMITAUR_H
#define ENNEMITAUR_H
#include "Ennemi.h"

class EnnemiTaur : public Ennemi{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonMort;
    bool attacked;
public:
    EnnemiTaur(Case*);
    ~EnnemiTaur();
    void deplacerEnnemi();
    SDL_Texture* getImage();
    bool attaque();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonRandom();
    static void initialiserEnnemiTaur();
    static void releaseEnnemiTaur();

};



#endif // ENNEMITAUR_H
