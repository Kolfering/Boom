#include "ProjectileBolt.h"
#include "Utile.h"

Mix_Chunk* ProjectileBolt::sonHit;
SDL_Texture* ProjectileBolt::image;
ProjectileBolt::~ProjectileBolt()
{

}

ProjectileBolt::ProjectileBolt(Ennemi* ennemi,Case* caseCollision,Position position,Direction direction) : Projectile(ennemi, caseCollision,position,direction)
{
    frameManagerAtkBas = {0,0,1,5,0,0,1};
    frameManagerAtkHaut = {0,0,1,5,0,0,1};
    frameManagerAtkDroite = {0,0,1,5,0,0,1};
    frameManagerAtkGauche = {0,0,1,5,0,0,1};
    frameManagerExplosion = {1,0,4,5,50,0,1};

    switch(direction)
    {
    case HAUT:
        frameManager = &frameManagerAtkHaut;
        break;
    case BAS:
        frameManager = &frameManagerAtkBas;
        break;
    case DROITE:
        frameManager = &frameManagerAtkDroite;
        break;
    case GAUCHE:
        frameManager = &frameManagerAtkGauche;
        break;
    default:
        break;
    }

    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
    frameManager->tempsBefore = getTime();
    vitesse = 1.2;
    degat = 3;
    dimensionProjectile = {13,13};
    dimensionExplosion = {32,32};
    dimension = dimensionProjectile;
    positionReal.x -= dimensionProjectile.x/2;
    positionReal.y -= dimensionProjectile.y/2;

}

Mix_Chunk* ProjectileBolt::getSonHit()
{
    return sonHit;
}

SDL_Texture* ProjectileBolt::getImage()
{
    return image;
}
void ProjectileBolt::initialiserProjectileBolt()
{
    sonHit = Mix_LoadWAV("../Sons/BoltHit.wav");
    image = chargerSurface("../Images/Lightbolt.png");
}

void ProjectileBolt::releaseProjectileBolt()
{
    if(sonHit)
        Mix_FreeChunk(sonHit);
    sonHit = nullptr;
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}
