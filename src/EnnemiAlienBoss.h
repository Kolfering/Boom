#ifndef ENNEMIALIENBOSS_H
#define ENNEMIALIENBOSS_H
#include "Ennemi.h"
#include <vector>
#define ALIENBOSS_DEATH 5000
#define ALIENBOSS_BLINK 100
#define ALIENBOSS_BLANK 200
#define ALIENBOSS_EXPLOSION 125
#define ALIENBOSS_VIEMAX 160

using std::vector;

class EnnemiAlienBoss : public Ennemi{
private:
    static SDL_Texture* image;
    static SDL_Texture* imageHurt;
    static Mix_Chunk* sonAtk;
    static Mix_Chunk* sonRandom;
    static Mix_Chunk* sonMort;
    vector <Case*> listeCase;
    vector <unsigned int> listeExplosion;
    unsigned int timeBlink;
    unsigned int timeExplosion;
    unsigned int timeHit;
    bool hit;
    bool freeDirection;
    bool calculCleanArea(Direction);
    void collisionBomber();
    bool collision();
public:
    EnnemiAlienBoss(Case*);
    ~EnnemiAlienBoss();
    SDL_Texture* getImage();
    Mix_Chunk* getSonMort();
    Mix_Chunk* getSonAtk();
    Mix_Chunk* getSonRandom();
    std::vector<Case*> getCaseOnList();
    std::vector<Case*> getCaseToGoOnList();
    std::vector<Case*> getCasePrecedenteOnList();
    void deplacerEnnemi();
    void prendreDegat(Bomber*,unsigned int);
    void accelerer();
    bool attaque();
    void setCase(Case*);
    static void initialiserEnnemiAlienBoss();
    static void releaseEnnemiAlienBoss();


};

#endif // ENNEMIALIENBOSS_H
