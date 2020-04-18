#ifndef ENNEMILIZZY_H
#define ENNEMILIZZY_H
#include "Ennemi.h"

class EnnemiLizzy : public Ennemi{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonRandom;
    static Mix_Chunk* sonMort;
public:
    EnnemiLizzy(Case*);
    ~EnnemiLizzy();
    void deplacerEnnemi();
    SDL_Texture* getImage();
    bool attaque();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonRandom();
    static void initialiserEnnemiLizzy();
    static void releaseEnnemiLizzy();

};

#endif // ENNEMILIZZY_H
