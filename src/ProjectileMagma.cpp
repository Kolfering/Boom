#include "ProjectileMagma.h"
#include "Utile.h"

Mix_Chunk* ProjectileMagma::sonHit;
SDL_Texture* ProjectileMagma::image;
ProjectileMagma::~ProjectileMagma()
{

}

ProjectileMagma::ProjectileMagma(Ennemi* ennemi,Case* caseCollision,Position position,Direction direction) : Projectile(ennemi,caseCollision,position,direction)
{
    frameManagerAtkBas = {0,0,1,8,0,0,1};
    frameManagerAtkHaut = {1,0,1,8,0,0,1};
    frameManagerAtkDroite = {2,0,1,8,0,0,1};
    frameManagerAtkGauche = {3,0,1,8,0,0,1};
    frameManagerExplosion = {4,0,4,8,50,0,1};

    switch(direction)
    {
    case Direction::HAUT:
        frameManager = &frameManagerAtkHaut;
        dimensionProjectile = {26,26};
        break;
    case Direction::BAS:
        frameManager = &frameManagerAtkBas;
        dimensionProjectile = {26,26};
        break;
    case Direction::DROITE:
        frameManager = &frameManagerAtkDroite;
        dimensionProjectile = {26,26};
        break;
    case Direction::GAUCHE:
        frameManager = &frameManagerAtkGauche;
        dimensionProjectile = {26,26};
        break;
    default:
        break;
    }

    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
    frameManager->tempsBefore = getTime();
    vitesse = 2;
    degat = 4;
    dimensionExplosion = {32,32};
    dimension = dimensionProjectile;
    positionReal.x -= dimensionProjectile.x/2;
    positionReal.y -= dimensionProjectile.y/2;

}

Mix_Chunk* ProjectileMagma::getSonHit()
{
    return sonHit;
}

SDL_Texture* ProjectileMagma::getImage()
{
    return image;
}
void ProjectileMagma::initialiserProjectileMagma()
{
    sonHit = Mix_LoadWAV("../Sons/MagmaHit.wav");
    image = chargerSurface("../Images/Magma.png");
}

void ProjectileMagma::releaseProjectileMagma()
{
    if(sonHit)
        Mix_FreeChunk(sonHit);
    sonHit = nullptr;
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}
