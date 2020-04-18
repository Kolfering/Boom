#ifndef PROJECTILEBOLT_H
#define PROJECTILEBOLT_H
#include "Projectile.h"

class ProjectileBolt : public Projectile{
private:
    static Mix_Chunk* sonHit;
    static SDL_Texture* image;
public:
    ProjectileBolt(Ennemi*,Case*,Position,Direction);
    ~ProjectileBolt();
    Mix_Chunk* getSonHit();
    static void initialiserProjectileBolt();
    static void releaseProjectileBolt();
    SDL_Texture* getImage();

};


#endif // PROJECTILEBOLT_H
