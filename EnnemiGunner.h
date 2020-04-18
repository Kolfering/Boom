#ifndef ENNEMIGUNNER_H
#define ENNEMIGUNNER_H
#include "Ennemi.h"

class EnnemiGunner : public Ennemi{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonRandom;
    static Mix_Chunk* sonMort;
public:
    EnnemiGunner(Case*);
    ~EnnemiGunner();
    void deplacerEnnemi();
    SDL_Texture* getImage();
    bool attaque();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonRandom();
    static void initialiserEnnemiGunner();
    static void releaseEnnemiGunner();
    void accelerer();

};

#endif // ENNEMIGUNNER_H
