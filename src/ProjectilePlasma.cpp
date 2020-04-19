#include "ProjectilePlasma.h"
#include "Utile.h"

Mix_Chunk* ProjectilePlasma::sonHit;
SDL_Texture* ProjectilePlasma::image;
ProjectilePlasma::~ProjectilePlasma()
{

}

ProjectilePlasma::ProjectilePlasma(Ennemi* ennemi,Case* caseCollision,Position position,Direction direction) : Projectile(ennemi,caseCollision,position,direction)
{
    frameManagerAtkBas = {0,0,1,5,0,0,1};
    frameManagerAtkHaut = {0,0,1,5,0,0,1};
    frameManagerAtkDroite = {0,0,1,5,0,0,1};
    frameManagerAtkGauche = {0,0,1,5,0,0,1};
    frameManagerExplosion = {1,0,4,5,50,0,1};
    frameManager = &frameManagerAtkBas;
    dimensionProjectile = {13,13};
    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
    frameManager->tempsBefore = getTime();
    vitesse = 2;
    degat = 3;
    dimensionExplosion = {32,32};
    dimension = dimensionProjectile;
    positionReal.x -= dimensionProjectile.x/2;
    positionReal.y -= dimensionProjectile.y/2;

}


Mix_Chunk* ProjectilePlasma::getSonHit()
{
    return sonHit;
}

SDL_Texture* ProjectilePlasma::getImage()
{
    return image;
}
void ProjectilePlasma::initialiserProjectilePlasma()
{
    sonHit = Mix_LoadWAV("../Sons/PlasmaHit.wav");
    image = chargerSurface("../Images/Plasma.png");
}

void ProjectilePlasma::releaseProjectilePlasma()
{
    if(sonHit)
        Mix_FreeChunk(sonHit);
    sonHit = nullptr;
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}
