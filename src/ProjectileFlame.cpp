#include "ProjectileFlame.h"
#include "Utile.h"

Mix_Chunk* ProjectileFlame::sonHit;
SDL_Texture* ProjectileFlame::image;
ProjectileFlame::~ProjectileFlame()
{

}

ProjectileFlame::ProjectileFlame(Ennemi* ennemi,Case* caseCollision,Position position,Direction direction) : Projectile(ennemi,caseCollision,position,direction)
{
    frameManagerAtkBas = {0,0,5,10,275,0,1};
    frameManagerAtkHaut = {0,0,5,10,275,0,1};
    frameManagerAtkDroite = {5,0,5,10,275,0,1};
    frameManagerAtkGauche = {5,0,5,10,275,0,1};
    frameManagerExplosion = {4,0,5,10,100,0,-1};

    switch(direction)
    {
    case Direction::HAUT:
        frameManager = &frameManagerAtkHaut;
        break;
    case Direction::BAS:
        frameManager = &frameManagerAtkBas;
        break;
    case Direction::DROITE:
        frameManager = &frameManagerAtkDroite;
        break;
    case Direction::GAUCHE:
        frameManager = &frameManagerAtkGauche;
        break;
    default:
        break;
    }

    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
    frameManager->tempsBefore = getTime();
    vitesse = 1;
    degat = 3;
    range = 4;
    dimensionExplosion = {32,32};
    dimensionProjectile = {32,32};
    dimension = dimensionProjectile;
    positionReal.x -= dimensionProjectile.x/2;
    positionReal.y -= dimensionProjectile.y/2;

}

Mix_Chunk* ProjectileFlame::getSonHit()
{
    return sonHit;
}

SDL_Texture* ProjectileFlame::getImage()
{
    return image;
}
void ProjectileFlame::initialiserProjectileFlame()
{
    sonHit = Mix_LoadWAV("../Sons/FlameHit.wav");
    image = chargerSurface("../Images/Flame.png");
}

void ProjectileFlame::releaseProjectileFlame()
{
    if(sonHit)
        Mix_FreeChunk(sonHit);
    sonHit = nullptr;
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}
