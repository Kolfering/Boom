#ifndef PROJECTILEHEADMISSILE_H
#define PROJECTILEHEADMISSILE_H
#include "Projectile.h"


class ProjectileHeadMissile : public Projectile{
private:
    static Mix_Chunk* sonHit;
    static SDL_Texture* image;
    Position cible;
    double angle;
public:
    void deplacer();
    ProjectileHeadMissile(Ennemi*,Case*,Position,Position);
    ProjectileHeadMissile(Position);
    ~ProjectileHeadMissile();
    Mix_Chunk* getSonHit();
    static void initialiserProjectileHeadMissile();
    static void releaseProjectileHeadMissile();
    SDL_Texture* getImage();

};


#endif // PROJECTILEHEADMISSILE_H
