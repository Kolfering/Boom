#include "ProjectileFireBall.h"
#include "Utile.h"

Mix_Chunk* ProjectileFireBall::sonHit;
SDL_Texture* ProjectileFireBall::image;
ProjectileFireBall::~ProjectileFireBall()
{

}

ProjectileFireBall::ProjectileFireBall(Ennemi* ennemi,Case* caseCollision,Position position,Direction direction) : Projectile(ennemi,caseCollision,position,direction)
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
    frameManager->tempsBefore = getTime();
    counterFrame.nbLigne = 0;
    vitesse = 1.2;
    degat = 2;
    dimensionProjectile = {13,13};
    dimensionExplosion = {32,32};
    dimension = dimensionProjectile;
    positionReal.x -= dimensionProjectile.x/2;
    positionReal.y -= dimensionProjectile.y/2;

}

Mix_Chunk* ProjectileFireBall::getSonHit()
{
    return sonHit;
}

SDL_Texture* ProjectileFireBall::getImage()
{
    return image;
}
void ProjectileFireBall::initialiserProjectileFireBall()
{
    sonHit = Mix_LoadWAV("../Sons/FireBallHit.wav");
    image = chargerSurface("../Images/FireBall.png");
}

void ProjectileFireBall::releaseProjectileFireBall()
{
    if(sonHit)
        Mix_FreeChunk(sonHit);
    sonHit = nullptr;
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}
