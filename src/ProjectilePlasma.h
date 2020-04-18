#ifndef PROJECTILEPLASMA_H
#define PROJECTILEPLASMA_H
#include "Projectile.h"

class ProjectilePlasma: public Projectile{
private:
    static Mix_Chunk* sonHit;
    static SDL_Texture* image;
public:
    ProjectilePlasma(Ennemi*,Case*,Position,Direction);
    ~ProjectilePlasma();
    Mix_Chunk* getSonHit();
    static void initialiserProjectilePlasma();
    static void releaseProjectilePlasma();
    SDL_Texture* getImage();

};


#endif // PROJECTILEPLASMA_H
