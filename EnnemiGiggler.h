#ifndef ENNEMIGIGGLER_H
#define ENNEMIGIGGLER_H
#include "Ennemi.h"

class EnnemiGiggler : public Ennemi{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonMort;
    static Mix_Chunk* sonRandom;
public:
    EnnemiGiggler(Case*);
    ~EnnemiGiggler();
    void deplacerEnnemi();
    SDL_Texture* getImage();
    bool attaque();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonRandom();
    static void initialiserEnnemiGiggler();
    static void releaseEnnemiGiggler();

};



#endif // ENNEMIGIGGLER_H
