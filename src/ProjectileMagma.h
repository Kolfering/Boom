#ifndef PROJECTILEMAGMA_H
#define PROJECTILEMAGMA_H
#include "Projectile.h"

class ProjectileMagma: public Projectile{
private:
    static Mix_Chunk* sonHit;
    static SDL_Texture* image;
public:
    ProjectileMagma(Ennemi*,Case*,Position,Direction);
    ~ProjectileMagma();
    Mix_Chunk* getSonHit();
    static void initialiserProjectileMagma();
    static void releaseProjectileMagma();
    SDL_Texture* getImage();

};

#endif // PROJECTILEMAGMA_H
