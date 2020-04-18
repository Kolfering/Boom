#ifndef ENNEMIGHOST_H
#define ENNEMIGHOST_H
#include "Ennemi.h"

class EnnemiGhost : public Ennemi{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonMort;
    bool spotted;
    bool frameAllowed;
    Direction directionSpotted;
    float vitesseAtk;
    float vitesseBase;
public:
    EnnemiGhost(Case*);
    ~EnnemiGhost();
    void deplacerEnnemi();
    SDL_Texture* getImage();
    bool attaque();
    bool goToCase();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonRandom();
    void accelerer();
    static void initialiserEnnemiGhost();
    static void releaseEnnemiGhost();

};

#endif // ENNEMIGHOST_H
