#ifndef PROJECTILEMGSHOT_H
#define PROJECTILEMGSHOT_H
#include "Projectile.h"

class ProjectileMGShot: public Projectile{
private:
    static Mix_Chunk* sonHit;
    static SDL_Texture* image;
public:
    ProjectileMGShot(Ennemi*,Case*,Position,Direction);
    ~ProjectileMGShot();
    Mix_Chunk* getSonHit();
    static void initialiserProjectileMGShot();
    static void releaseProjectileMGShot();
    SDL_Texture* getImage();

};


#endif // PROJECTILEMGSHOT_H
