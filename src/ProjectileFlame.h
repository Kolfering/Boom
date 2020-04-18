#ifndef PROJECTILEFLAME_H
#define PROJECTILEFLAME_H
#include "Projectile.h"

class ProjectileFlame: public Projectile{
private:
    static Mix_Chunk* sonHit;
    static SDL_Texture* image;
public:
    ProjectileFlame(Ennemi*,Case*,Position,Direction);
    ~ProjectileFlame();
    Mix_Chunk* getSonHit();
    static void initialiserProjectileFlame();
    static void releaseProjectileFlame();
    SDL_Texture* getImage();

};


#endif // PROJECTILEFLAME_H
