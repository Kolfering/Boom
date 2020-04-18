#ifndef ENNEMISOLDIER_H
#define ENNEMISOLDIER_H
#include "EnnemiHumain.h"

class EnnemiSoldier : public EnnemiHumain{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonRandom;
    static Mix_Chunk* sonMort;
public:
    EnnemiSoldier(Case*);
    ~EnnemiSoldier();
    SDL_Texture* getImage();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonAtk();
    Mix_Chunk* getSonRandom();
    static void initialiserEnnemiSoldier();
    static void releaseEnnemiSoldier();


};

#endif // ENNEMISOLDIER_H
