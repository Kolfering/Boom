#ifndef ENNEMIHEADBOSS_H
#define ENNEMIHEADBOSS_H
#include "Ennemi.h"
#define HEADBOSS_BLANK 100
#define HEADBOSS_BLINK 100
#define HEADBOSS_DEATH 4000
#define HEADBOSS_EXPLOSION 125
#define HEADBOSS_MINIFIRE 400
#define HEADBOSS_NBFIRE 3
#include <vector>

using std::vector;
class EnnemiHeadBoss : public Ennemi{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonMort;
    static Mix_Chunk* sonHurt;
    static SDL_Texture* imageHurt;
    vector <Case*> listeCase;
    vector <unsigned int> listeExplosion;
    void collisionBomber();
    unsigned int timeHit;
    unsigned int timeBlink;
    unsigned int timeExplosion;
    unsigned int timeFire;
    unsigned int timeMiniFire;
    unsigned int nbFire;
    bool hit;
    unsigned int range;

public:
    EnnemiHeadBoss(Case*);
    ~EnnemiHeadBoss();
    void deplacerEnnemi();
    SDL_Texture* getImage();
    bool attaque();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonRandom();
    void prendreDegat(Bomber*,unsigned int);
    static void initialiserEnnemiHeadBoss();
    static void releaseEnnemiHeadBoss();
    SDL_Rect* getAdresseAntiPosition();

};


#endif // ENNEMIHEADBOSS_H
