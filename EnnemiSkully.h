#ifndef ENNEMISKULLY_H
#define ENNEMISKULLY_H
#include "Ennemi.h"

class EnnemiSkully : public Ennemi{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonRandom;
    static Mix_Chunk* sonMort;
public:
    EnnemiSkully(Case*);
    ~EnnemiSkully();
    void deplacerEnnemi();
    SDL_Texture* getImage();
    bool attaque();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonRandom();
    static void initialiserEnnemiSkully();
    static void releaseEnnemiSkully();
    void accelerer();

};

#endif // ENNEMISKULLY_H
