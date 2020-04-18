#ifndef ENNEMISARGE_H
#define ENNEMISARGE_H
#include "EnnemiHumain.h"

class EnnemiSarge : public EnnemiHumain{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonRandom;
    static Mix_Chunk* sonMort;
public:
    EnnemiSarge(Case*);
    ~EnnemiSarge();
    SDL_Texture* getImage();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonRandom();
    Mix_Chunk* getSonAtk();
    static void initialiserEnnemiSarge();
    static void releaseEnnemiSarge();

};

#endif // ENNEMISARGE_H
