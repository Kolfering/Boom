#ifndef PROJECTILEEGG_H
#define PROJECTILEEGG_H
#include "Projectile.h"

class ProjectileEgg : public Projectile{
private:
    static Mix_Chunk* sonHit;
    static SDL_Texture* image;
    static SDL_Texture* imageExplose;
    bool pondu;
    float vitessePonte;
    float vitesseDeplacement;
    FrameManager frameManagerCourant;
    void calculNextFrame();
    void spawnEnnemi();

public:
    ProjectileEgg(Ennemi*,Case*,Position,Direction);
    ~ProjectileEgg();
    Mix_Chunk* getSonHit();
    void deplacer();
    SDL_Rect* getAdresseAntiPosition();
    static void initialiserProjectileEgg();
    static void releaseProjectileEgg();
    SDL_Texture* getImage();

};


#endif // PROJECTILEEGG_H
