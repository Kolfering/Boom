#include "ProjectileEgg.h"
#include "Utile.h"
#include "MenuJeu.h"
#include "EnnemiSoldier.h"
#include "EnnemiSarge.h"
#include "EnnemiLizzy.h"
#include "EnnemiTaur.h"
#include "EnnemiGunner.h"
#include "EnnemiThing.h"
#include "EnnemiGhost.h"
#include "EnnemiSmoulder.h"
#include "EnnemiSkully.h"
#include "EnnemiGiggler.h"

Mix_Chunk* ProjectileEgg::sonHit;
SDL_Texture* ProjectileEgg::image;
SDL_Texture* ProjectileEgg::imageExplose;

ProjectileEgg::~ProjectileEgg()
{

}

ProjectileEgg::ProjectileEgg(Ennemi* ennemi,Case* caseCollision,Position position,Direction direction) : Projectile(ennemi,caseCollision,position,direction)
{
    frameManagerAtkBas = {4,0,4,9,100,0,1};
    frameManagerAtkHaut = {0,0,4,9,100,0,1};
    frameManagerAtkDroite = {3,1,4,9,100,0,1};
    frameManagerAtkGauche = {8,0,4,9,100,0,1};
    frameManagerExplosion = {0,0,4,4,100,0,1};
    frameManagerCourant = {7,1,1,9,0,0,1};

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
    vitessePonte = 0;
    vitesseDeplacement = 2;
    vitesse = vitessePonte;
    degat = 1;
    dimensionProjectile = {32,32};
    dimensionExplosion = {64,64};
    dimension = dimensionProjectile;
    positionReal.x -= dimensionProjectile.x/2;
    positionReal.y -= dimensionProjectile.y/2;
    pondu = false;

}


void ProjectileEgg::deplacer()
{
    if(!explose)
    {
        for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
        {
            if(positionReal.y + dimension.y/2 < MenuJeu::getListeBomber().at(i)->getPosition().y + MenuJeu::getListeBomber().at(i)->getTaille() && positionReal.y + dimension.y/2 > MenuJeu::getListeBomber().at(i)->getPosition().y &&positionReal.x + dimension.x/2 > MenuJeu::getListeBomber().at(i)->getPosition().x && positionReal.x + dimension.x/2 < MenuJeu::getListeBomber().at(i)->getPosition().x + MenuJeu::getListeBomber().at(i)->getTaille())
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
                if(positionReal.y + dimension.y/2 < Carte::getListeEnnemi().at(i)->getPosition().y + Carte::getListeEnnemi().at(i)->getTaille() && positionReal.y + dimension.y/2 > Carte::getListeEnnemi().at(i)->getPosition().y &&positionReal.x + dimension.x/2 > Carte::getListeEnnemi().at(i)->getPosition().x && positionReal.x + dimension.x/2 < Carte::getListeEnnemi().at(i)->getPosition().x + Carte::getListeEnnemi().at(i)->getTaille())
                {
                    explose = true;
                    break;
                }
            }
        }


        if(!explose)
        {
            switch(direction)
            {
            case HAUT:
                if(caseCollision)
                {
                    if(caseCollision->getType() != FOND)
                    {
                        if(positionReal.y + dimension.y/2 < caseCollision->getPositionPixel().y + CASE_DIMENSION)
                        {
                            explose = true;
                            break;
                        }
                    }
                    else
                    {
                        for(unsigned int i = caseCollision->getPositionDamier().y;i>=1;i--)
                        {
                            if(Carte::getCase(i,caseCollision->getPositionDamier().x)->getType() == DUR || Carte::getCase(i,caseCollision->getPositionDamier().x)->getType() == CASSABLE)
                            {
                                if(positionReal.y + dimension.y/2 < Carte::getCase(i,caseCollision->getPositionDamier().x)->getPositionPixel().y + CASE_DIMENSION)
                                {
                                    explose = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                if(positionReal.y + dimension.y/2 < Carte::getCase(1,1)->getPositionPixel().y)
                {
                    explose = true;
                    break;
                }

                positionReal.y -= vitesse;
                distanceRange += vitesse;
                break;
            case BAS:
                if(caseCollision)
                {
                    if(caseCollision->getType() != FOND)
                    {
                        if(positionReal.y +dimension.y/2 > caseCollision->getPositionPixel().y)
                        {
                            explose = true;
                            break;
                        }
                    }
                    else
                    {
                        for(unsigned int i = caseCollision->getPositionDamier().y;i<=CASE_HAUTEUR;i++)
                        {
                            if(Carte::getCase(i,caseCollision->getPositionDamier().x)->getType() == DUR || Carte::getCase(i,caseCollision->getPositionDamier().x)->getType() == CASSABLE)
                            {
                                if(positionReal.y + dimension.y/2 > Carte::getCase(i,caseCollision->getPositionDamier().x)->getPositionPixel().y)
                                {
                                    explose = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                if(positionReal.y + dimension.y/2 > Carte::getCase(CASE_HAUTEUR,1)->getPositionPixel().y  + CASE_DIMENSION)
                {
                    explose = true;
                    break;
                }

                positionReal.y += vitesse;
                distanceRange += vitesse;
                break;
            case GAUCHE:
                if(caseCollision)
                {
                    if(caseCollision->getType() != FOND)
                    {
                        if(positionReal.x +dimension.x/2 < caseCollision->getPositionPixel().x + CASE_DIMENSION)
                        {
                            explose = true;
                            break;
                        }
                    }
                    else
                    {
                        for(unsigned int i = caseCollision->getPositionDamier().x;i>=1;i--)
                        {
                            if(Carte::getCase(caseCollision->getPositionDamier().y,i)->getType() == DUR || Carte::getCase(caseCollision->getPositionDamier().y,i)->getType() == CASSABLE)
                            {
                                 if(positionReal.x + dimension.y/2 < Carte::getCase(caseCollision->getPositionDamier().y,i)->getPositionPixel().x + CASE_DIMENSION)
                                {
                                    explose = true;
                                    break;
                                }
                            }

                        }
                    }
                }

                if(positionReal.x + dimension.x/2 < Carte::getCase(1,1)->getPositionPixel().x)
                {
                    explose = true;
                    break;
                }
                positionReal.x -= vitesse;
                distanceRange += vitesse;
                break;
            case DROITE:
                if(caseCollision)
                {
                    if(caseCollision->getType() != FOND)
                    {
                        if(positionReal.x +dimension.x/2> caseCollision->getPositionPixel().x)
                        {
                            explose = true;
                            break;
                        }
                    }
                    else
                    {
                        for(unsigned int i = caseCollision->getPositionDamier().x;i<=CASE_LONGUEUR;i++)
                        {
                            if(Carte::getCase(caseCollision->getPositionDamier().y,i)->getType() == DUR || Carte::getCase(caseCollision->getPositionDamier().y,i)->getType() == CASSABLE)
                            {

                                if(positionReal.x + dimension.y/2 > Carte::getCase(caseCollision->getPositionDamier().y,i)->getPositionPixel().x)
                                {
                                    explose = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                if(positionReal.x + dimension.x/2 > Carte::getCase(1,CASE_LONGUEUR)->getPositionPixel().x + CASE_DIMENSION)
                {
                    explose = true;
                    break;
                }

                positionReal.x += vitesse;
                distanceRange += vitesse;
                break;

            default:
                break;
            }

            if(range > 0)
            {
                if(distanceRange > (range * CASE_DIMENSION))
                {
                    fini = true;
                    return;
                }

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
                frameManager->tempsBefore = getTime();
                positionReal.x+=dimension.x/2;
                positionReal.y+=dimension.y/2;
                dimension = dimensionExplosion;
                positionReal.x-=dimension.x/2;
                positionReal.y-=dimension.y/2;

                spawnEnnemi();

            }
        }
    }

    calculNextFrame();
}

void ProjectileEgg::spawnEnnemi()
{
    if(!Carte::getBoss())
        return;
    else
    {
        if(Carte::getBoss()->isMeurt())
            return;
    }

    bool check = false;
    Case* c = Carte::getCaseByPixel(position.y,position.x);
    if(c)
    {
        if(c->getType() == FOND && !c->getEnnemi())
        {
            check = true;
        }
    }
    else
    {
        switch(direction)
        {
            case HAUT:
                c = Carte::getCaseByPixel(position.y + CASE_DIMENSION,position.x);
                break;
            case BAS:
                c = Carte::getCaseByPixel(position.y - CASE_DIMENSION,position.x);
                break;
            case DROITE:
                c = Carte::getCaseByPixel(position.y,position.x - CASE_DIMENSION);
                break;
            case GAUCHE:
                c = Carte::getCaseByPixel(position.y,position.x + CASE_DIMENSION);
                break;
            default:
                c = nullptr;
        }
        check = true;
    }
    if(!check)
    {
        switch(direction)
        {
        case HAUT:
            c = Carte::getCase(c->getPositionDamier().y+1,c->getPositionDamier().x);
            break;
        case BAS:
            c = Carte::getCase(c->getPositionDamier().y-1,c->getPositionDamier().x);
            break;
        case DROITE:
            c = Carte::getCase(c->getPositionDamier().y,c->getPositionDamier().x-1);
            break;
        case GAUCHE:
            c = Carte::getCase(c->getPositionDamier().y,c->getPositionDamier().x+1);
            break;
        default:
            c = nullptr;
        }


    }


    if(c)
    {
        if(c->getType() == FOND && !c->getEnnemi())
        {

            unsigned int nb = genererNombreAleatoire(10) - 1;

            IDEnnemi id = static_cast<IDEnnemi>(nb);
            Ennemi* ennemi;
            switch(id)
            {
                case SOLDIER:
                    ennemi = new EnnemiSoldier(c);
                    break;
                case SARGE:
                    ennemi = new EnnemiSarge(c);
                    break;
                case LIZZY:
                    ennemi = new EnnemiLizzy(c);
                    break;
                case TAUR:
                    ennemi = new EnnemiTaur(c);
                    break;
                case GUNNER:
                    ennemi = new EnnemiGunner(c);
                    break;
                case THING:
                    ennemi = new EnnemiThing(c);
                    break;
                case GHOST:
                    ennemi = new EnnemiGhost(c);
                    break;
                case SMOULDER:
                    ennemi = new EnnemiSmoulder(c);
                    break;
                case SKULLY:
                    ennemi = new EnnemiSkully(c);
                    break;
                case GIGGLER:
                    ennemi = new EnnemiGiggler(c);
                    break;
                default:
                    return;
            }

            if(Carte::getTempsEcoule() == Carte::getTemps())
                ennemi->accelerer();

            Carte::ajouterEnnemi(ennemi);
        }


    }


}

void ProjectileEgg::calculNextFrame()
{
    if(getTime() - frameManager->tempsBefore > frameManager->tempsFrame)
    {
        counterFrame.x += frameManager->increment;
        frameManager->tempsBefore = getTime();
        if(counterFrame.x == frameManager->longueur)
        {
            counterFrame.x = 0;
            counterFrame.y++;
            counterFrame.nbLigne++;
        }

            if(counterFrame.x + (frameManager->longueur*counterFrame.nbLigne) == (frameManager->x + frameManager->nbFrame))
            {

                if(!explose)
                {
                    if(!pondu)
                    {
                        frameManager = &frameManagerCourant;
                        vitesse = vitesseDeplacement;
                        pondu = true;
                    }
                     counterFrame.x = frameManager->x;
                     counterFrame.y = frameManager->y;
                     counterFrame.nbLigne = 0;
                }
                else
                    fini = true;
            }

    }
}


Mix_Chunk* ProjectileEgg::getSonHit()
{
    return sonHit;
}

SDL_Texture* ProjectileEgg::getImage()
{
    if(explose)
        return imageExplose;
    return image;
}
void ProjectileEgg::initialiserProjectileEgg()
{
    sonHit = Mix_LoadWAV("../Sons/EggCrack.wav");
    image = chargerSurface("../Images/EggFrames.png");
    imageExplose = chargerSurface("../Images/EggCrack.png");
}


SDL_Rect* ProjectileEgg::getAdresseAntiPosition()
{
    antiPosition.x = counterFrame.x * dimension.x;
    antiPosition.y = counterFrame.y * dimension.y;
    antiPosition.w = dimension.x;
    antiPosition.h = dimension.y;
    return &antiPosition;
}

void ProjectileEgg::releaseProjectileEgg()
{
    if(sonHit)
        Mix_FreeChunk(sonHit);
    sonHit = nullptr;
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
    if(imageExplose)
        SDL_DestroyTexture(imageExplose);
    imageExplose = nullptr;
}
