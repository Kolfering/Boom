#include "Projectile.h"
#include "Carte.h"
#include "Utile.h"
#include "MenuJeu.h"

Projectile::Projectile(Ennemi* ennemi,Case* caseCollision,Position position,Direction direction)
{
    this->caseCollision = caseCollision;
    this->ennemi = ennemi;
    fini = false;
    explose = false;
    this->direction = direction;
    this->positionReal = position;
    range = -1;
    distanceRange = 0;

}

Projectile::Projectile(Position pos)
{
    caseCollision = nullptr;
    positionReal = pos;
    fini = false;
    direction = Direction::UNDEFINED;
}

Projectile::~Projectile()
{

}

void Projectile::calculNextFrame()
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
        if(frameManager->increment > 0)
        {
            if(counterFrame.x + (frameManager->longueur*counterFrame.nbLigne) == (frameManager->x + frameManager->nbFrame))
            {
                if(!explose)
                {
                     counterFrame.x = frameManager->x;
                     counterFrame.y = frameManager->y;
                     counterFrame.nbLigne = 0;
                }
                else
                    fini = true;
            }
        }
        else
        {
            if(counterFrame.x + (frameManager->longueur*counterFrame.nbLigne) == frameManager->x - frameManager->nbFrame)
            {
                if(!explose)
                {
                     counterFrame.x = frameManager->x;
                     counterFrame.y = frameManager->y;
                     counterFrame.nbLigne = 0;
                }
                else
                    fini = true;
            }
        }
    }
}


SDL_Rect* Projectile::getAdresseAntiPosition()
{
    antiPosition.x = counterFrame.x * CASE_DIMENSION;
    antiPosition.y = counterFrame.y * CASE_DIMENSION;
    antiPosition.w = dimension.x;
    antiPosition.h = dimension.y;
    return &antiPosition;
}

bool Projectile::isFini()
{
    return fini;
}

SDL_Rect* Projectile::getAdressePosition()
{
    position.w = dimension.x;
    position.h = dimension.y;
    position.x = positionReal.x;
    position.y = positionReal.y;
    return &position;
}
void Projectile::deplacer()
{
    if(!explose)
    {
        for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
        {
            if(positionReal.y + dimension.y/2 < MenuJeu::getListeBomber().at(i)->getPosition().y + MenuJeu::getListeBomber().at(i)->getTaille() && positionReal.y + dimension.y/2 > MenuJeu::getListeBomber().at(i)->getPosition().y &&positionReal.x + dimension.x/2 > MenuJeu::getListeBomber().at(i)->getPosition().x && positionReal.x + dimension.x/2 < MenuJeu::getListeBomber().at(i)->getPosition().x + MenuJeu::getListeBomber().at(i)->getTaille())
            {
                fini = true;
                MenuJeu::getListeBomber().at(i)->prendreDegat(degat);
                return;

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
            case Direction::HAUT:
                if(caseCollision)
                {
                    if(caseCollision->getType() == TypeCase::DUR ||caseCollision->getType() == TypeCase::CASSABLE)
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
                            if(Carte::getCase(i,caseCollision->getPositionDamier().x)->getType() == TypeCase::DUR || Carte::getCase(i,caseCollision->getPositionDamier().x)->getType() == TypeCase::CASSABLE)
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
            case Direction::BAS:
                if(caseCollision)
                {
                    if(caseCollision->getType() == TypeCase::DUR ||caseCollision->getType() == TypeCase::CASSABLE)
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
                            if(Carte::getCase(i,caseCollision->getPositionDamier().x)->getType() == TypeCase::DUR || Carte::getCase(i,caseCollision->getPositionDamier().x)->getType() == TypeCase::CASSABLE)
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
            case Direction::GAUCHE:
                if(caseCollision)
                {
                    if(caseCollision->getType() == TypeCase::DUR ||caseCollision->getType() == TypeCase::CASSABLE)
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
                            if(Carte::getCase(caseCollision->getPositionDamier().y,i)->getType() == TypeCase::DUR || Carte::getCase(caseCollision->getPositionDamier().y,i)->getType() == TypeCase::CASSABLE)
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
            case Direction::DROITE:
                if(caseCollision)
                {
                    if(caseCollision->getType() == TypeCase::DUR ||caseCollision->getType() == TypeCase::CASSABLE)
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
                            if(Carte::getCase(caseCollision->getPositionDamier().y,i)->getType() == TypeCase::DUR || Carte::getCase(caseCollision->getPositionDamier().y,i)->getType() == TypeCase::CASSABLE)
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

            }
        }
    }

    calculNextFrame();
}
