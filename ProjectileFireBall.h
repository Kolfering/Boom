#ifndef PROJECTILEFIREBALL_H
#define PROJECTILEFIREBALL_H
#include "Projectile.h"

class ProjectileFireBall : public Projectile{
private:
    static Mix_Chunk* sonHit;
    static SDL_Texture* image;
public:
    ProjectileFireBall(Ennemi*,Case*,Position,Direction);
    ~ProjectileFireBall();
    Mix_Chunk* getSonHit();
    static void initialiserProjectileFireBall();
    static void releaseProjectileFireBall();
    SDL_Texture* getImage();

};

#endif // PROJECTILEFIREBALL_H
