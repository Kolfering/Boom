#ifndef PROJECTILESHOT_H
#define PROJECTILESHOT_H
#include "Projectile.h"

class ProjectileShot : public Projectile{
private:
    static Mix_Chunk* sonHit;
    static SDL_Texture* image;
public:
    ProjectileShot(Ennemi*,Case*,Position,Direction);
    ~ProjectileShot();
    Mix_Chunk* getSonHit();
    static void initialiserProjectileShot();
    static void releaseProjectileShot();
    SDL_Texture* getImage();

};

#endif // PROJECTILESHOT_H
