#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Enumeration.h"
#include "Input.h"
#include "Case.h"

class Projectile{
protected:
    Ennemi* ennemi;
    Case* caseCollision;
    SDL_Rect position;
    SDL_Rect antiPosition;
    Position positionReal;
    Position dimensionProjectile;
    Position dimensionExplosion;
    Position dimension;
    bool fini;
    bool explose;
    float vitesse;
    unsigned int degat;
    Direction direction;
    int range;
    int distanceRange;
    FrameManager frameManagerExplosion;
    FrameManager frameManagerAtkHaut;
    FrameManager frameManagerAtkBas;
    FrameManager frameManagerAtkDroite;
    FrameManager frameManagerAtkGauche;
    FrameManager* frameManager;
    CounterFrame counterFrame;
    virtual void calculNextFrame();

public:
    Projectile(Position);
    Projectile(Ennemi*,Case*,Position,Direction);
    virtual ~Projectile();
    virtual void deplacer();
    bool isFini();
    virtual Mix_Chunk* getSonHit() = 0;
    SDL_Rect* getAdressePosition();
    virtual SDL_Rect* getAdresseAntiPosition();
    virtual SDL_Texture* getImage() = 0;

};


#endif // PROJECTILE_H
