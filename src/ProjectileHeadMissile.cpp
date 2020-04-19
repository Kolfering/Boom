#include "ProjectileHeadMissile.h"
#include "Utile.h"
#include "MenuJeu.h"
#include "math.h"

Mix_Chunk* ProjectileHeadMissile::sonHit;
SDL_Texture* ProjectileHeadMissile::image;
ProjectileHeadMissile::~ProjectileHeadMissile()
{

}

ProjectileHeadMissile::ProjectileHeadMissile(Ennemi* ennemi,Case* caseCollision,Position position,Position cible) : Projectile(ennemi,caseCollision,position,Direction::UNDEFINED)
{
    frameManagerAtkBas = {0,0,2,7,50,0,1};
    frameManagerAtkHaut = {0,0,2,7,50,0,1};
    frameManagerAtkDroite = {0,0,2,7,50,0,1};
    frameManagerAtkGauche = {0,0,2,7,50,0,1};
    frameManagerExplosion = {2,0,5,7,100,0,1};
    this->cible = cible;


    angle = atan(((cible.y)-position.y)/((cible.x)-(position.x)));
    angle += position.x > cible.x ? M_PI : 0;

    frameManager = &frameManagerAtkHaut;

    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
    frameManager->tempsBefore = getTime();
    vitesse = 1.2;
    degat = 4;
    dimensionProjectile = {20,20};
    dimensionExplosion = {32,32};
    dimension = dimensionProjectile;
    positionReal.x -= dimensionProjectile.x/2;
    positionReal.y -= dimensionProjectile.y/2;


}

ProjectileHeadMissile::ProjectileHeadMissile(Position pos) : Projectile(pos)
{
    explose = true;
    frameManagerExplosion = {2,0,5,7,150,0,1};
    frameManager = &frameManagerExplosion;
    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
    dimensionProjectile = {20,20};
    dimensionExplosion = {32,32};
    dimension = dimensionExplosion;
    positionReal.x -= dimensionProjectile.x/2;
    positionReal.y -= dimensionProjectile.y/2;
    Mix_PlayChannel(-1,getSonHit(),0);
}

Mix_Chunk* ProjectileHeadMissile::getSonHit()
{
    return sonHit;
}

SDL_Texture* ProjectileHeadMissile::getImage()
{
    return image;
}
void ProjectileHeadMissile::initialiserProjectileHeadMissile()
{
    sonHit = Mix_LoadWAV("../Sons/HeadShotHit.wav");
    image = chargerSurface("../Images/HeadMissile.png");
}

void ProjectileHeadMissile::releaseProjectileHeadMissile()
{
    if(sonHit)
        Mix_FreeChunk(sonHit);
    sonHit = nullptr;
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}

void ProjectileHeadMissile::deplacer()
{
    if(!explose)
    {
        for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
        {
            if(positionReal.y + dimension.y/2 < MenuJeu::getListeBomber().at(i)->getPosition().y + CASE_DIMENSION && positionReal.y + dimension.y/2 > MenuJeu::getListeBomber().at(i)->getPosition().y &&positionReal.x + dimension.x/2 > MenuJeu::getListeBomber().at(i)->getPosition().x && positionReal.x + dimension.x/2 < MenuJeu::getListeBomber().at(i)->getPosition().x + CASE_DIMENSION)
            {
                explose = true;
                MenuJeu::getListeBomber().at(i)->prendreDegat(degat);
                break;

            }
        }
        for(unsigned int i =0;i<Carte::getListeEnnemi().size();i++)
        {
            if(Carte::getListeEnnemi().at(i) != ennemi)
            {
                if(Carte::getListeEnnemi().at(i)->getIDEnnemi() != IDEnnemi::HEADBOSS)
                {
                    if(positionReal.y + dimension.y/2 < Carte::getListeEnnemi().at(i)->getPosition().y + CASE_DIMENSION && positionReal.y + dimension.y/2 > Carte::getListeEnnemi().at(i)->getPosition().y &&positionReal.x + dimension.x/2 > Carte::getListeEnnemi().at(i)->getPosition().x && positionReal.x + dimension.x/2 < Carte::getListeEnnemi().at(i)->getPosition().x + CASE_DIMENSION)
                    {
                        explose = true;
                        break;
                    }
                }
            }
        }


        if(!explose)
        {

            if(positionReal.y + dimension.y/2 < Carte::getCase(1,1)->getPositionPixel().y)
            {
                explose = true;
            }

            else if(positionReal.y + dimension.y/2 > Carte::getCase(CASE_HAUTEUR,1)->getPositionPixel().y  + CASE_DIMENSION)
            {
                explose = true;
            }

            else if(positionReal.x + dimension.x/2 < Carte::getCase(1,1)->getPositionPixel().x)
            {
                explose = true;
            }

            else if(positionReal.x + dimension.x/2 > Carte::getCase(1,CASE_LONGUEUR)->getPositionPixel().x + CASE_DIMENSION)
            {
                explose = true;
            }
            else
            {
                positionReal.x = positionReal.x + vitesse*cos(angle);
                positionReal.y = positionReal.y + vitesse*sin(angle);
            }


        }


        if(explose)
        {
            if(!compareFrameManager(*frameManager,frameManagerExplosion))
            {
                Mix_PlayChannel(-1,getSonHit(),0);
                frameManager = &frameManagerExplosion;
                counterFrame.x = frameManager->x;
                counterFrame.y = frameManager->y;
                counterFrame.nbLigne = 0;
                positionReal.x+=dimension.x/2;
                positionReal.y+=dimension.y/2;
                dimension = dimensionExplosion;

                positionReal.x-=dimension.x/2;
                positionReal.y-=dimension.y/2;

            }
        }
    }

    calculNextFrame();
}
