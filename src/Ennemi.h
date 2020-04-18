#ifndef ENNEMI_H
#define ENNEMI_H
#include "Personnage.h"
#include "Case.h"
#include <SDL_mixer.h>
#include <vector>
#define PROBABILITY_ENNEMI_SON_RANDOM 1

class Ennemi : public Personnage{
protected:
    static SDL_Texture* imageAlien;
    static Mix_Chunk* sonAlienMort;
    FrameManager frameManagerAlienHaut;
    FrameManager frameManagerAlienBas;
    FrameManager frameManagerAlienDroite;
    FrameManager frameManagerAlienGauche;
    FrameManager frameManagerAlienMort;
    Direction directionInverse;
    IDEnnemi idEnnemi;
    float degat;
    bool frameAtkActif;
    bool apparaitre;
    bool alienable;
    bool frameAtk;
    bool standWhileAtk;
    static bool alienMode;
    unsigned int score;
    unsigned int intervalAtk;
    unsigned int tempsDerniereAtk;
    unsigned int tempsFrameAtk;
    unsigned int tempsSonRandom;
    Direction directionAtk;
    Case* casePrecedente;
    FrameManager frameManagerHaut;
    FrameManager frameManagerBas;
    FrameManager frameManagerDroite;
    FrameManager frameManagerGauche;
    FrameManager frameManagerDead;
    FrameManager frameManagerAtkHaut;
    FrameManager frameManagerAtkBas;
    FrameManager frameManagerAtkGauche;
    FrameManager frameManagerAtkDroite;
    void gestionSonRandom();
    virtual bool collision();
    virtual void collisionBomber();
    virtual bool checkCase(Case*,std::vector<Case*>);

public:
    Ennemi(Case*);
    virtual void prendreDegat(Bomber*, unsigned int ID);
    virtual void deplacerEnnemi() = 0;
    virtual bool attaque() = 0;
    virtual Mix_Chunk* getSonRandom() = 0;
    virtual bool goToCase();
    virtual std::vector<Case*> getCaseOnList();
    virtual std::vector<Case*> getCaseToGoOnList();
    virtual std::vector<Case*> getCasePrecedenteOnList();
    bool isApparaitre();
    virtual Case* getCasePrecedente();
    void setCase(Case*);
    void faitDemiTour();
    IDEnnemi getIDEnnemi();
    static bool getAlienMode();
    static void setAlienMode(bool);
    static void initialiserEnnemi();
    static void releaseEnnemi();
    float getDegat();
    bool isMort();
    virtual void accelerer();
    virtual Mix_Chunk* getSonMort() = 0;
    virtual ~Ennemi();


};


#endif // ENNEMI_H
