#include "ProjectileMGShot.h"
#include "Utile.h"

Mix_Chunk* ProjectileMGShot::sonHit;
SDL_Texture* ProjectileMGShot::image;
ProjectileMGShot::~ProjectileMGShot()
{

}

ProjectileMGShot::ProjectileMGShot(Ennemi* ennemi,Case* caseCollision,Position position,Direction direction) : Projectile(ennemi,caseCollision,position,direction)
{
    frameManagerAtkBas = {0,0,1,9,0,0,1};
    frameManagerAtkHaut = {1,0,1,9,0,0,1};
    frameManagerAtkDroite = {2,0,1,9,0,0,1};
    frameManagerAtkGauche = {3,0,1,9,0,0,1};
    frameManagerExplosion = {4,0,5,9,50,0,1};

    switch(direction)
    {
    case HAUT:
        frameManager = &frameManagerAtkHaut;
        dimensionProjectile = {7,10};
        break;
    case BAS:
        frameManager = &frameManagerAtkBas;
        dimensionProjectile = {7,10};
        break;
    case DROITE:
        frameManager = &frameManagerAtkDroite;
        dimensionProjectile = {10,7};
        break;
    case GAUCHE:
        frameManager = &frameManagerAtkGauche;
        dimensionProjectile = {10,7};
        break;
    default:
        break;
    }

    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
    frameManager->tempsBefore = getTime();
    vitesse = 2;
    degat = 2;
    dimensionExplosion = {22,22};
    dimension = dimensionProjectile;
    positionReal.x -= dimensionProjectile.x/2;
    positionReal.y -= dimensionProjectile.y/2;

}

Mix_Chunk* ProjectileMGShot::getSonHit()
{
    return sonHit;
}

SDL_Texture* ProjectileMGShot::getImage()
{
    return image;
}
void ProjectileMGShot::initialiserProjectileMGShot()
{
    sonHit = Mix_LoadWAV("Sons/PulseHit.wav");
    image = chargerSurface("Images/MGShot.png");
}

void ProjectileMGShot::releaseProjectileMGShot()
{
    if(sonHit)
        Mix_FreeChunk(sonHit);
    sonHit = nullptr;
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}
