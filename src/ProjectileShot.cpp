#include "ProjectileShot.h"
#include "Utile.h"

Mix_Chunk* ProjectileShot::sonHit;
SDL_Texture* ProjectileShot::image;
ProjectileShot::~ProjectileShot()
{

}

ProjectileShot::ProjectileShot(Ennemi* ennemi,Case* caseCollision,Position position,Direction direction) : Projectile(ennemi,caseCollision,position,direction)
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
    vitesse = 2;
    degat = 1;
    dimensionProjectile = {7,7};
    dimensionExplosion = {22,26};
    dimension = dimensionProjectile;
    positionReal.x -= dimensionProjectile.x/2;
    positionReal.y -= dimensionProjectile.y/2;



}

Mix_Chunk* ProjectileShot::getSonHit()
{
    return sonHit;
}

SDL_Texture* ProjectileShot::getImage()
{
    return image;
}
void ProjectileShot::initialiserProjectileShot()
{
    sonHit = Mix_LoadWAV("Sons/ShotHit.wav");
    image = chargerSurface("Images/Shot.png");
}

void ProjectileShot::releaseProjectileShot()
{
    if(sonHit)
        Mix_FreeChunk(sonHit);
    sonHit = nullptr;
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}
