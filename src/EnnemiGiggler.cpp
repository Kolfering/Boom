#include "EnnemiGiggler.h"
#include "Utile.h"
#include "Carte.h"
#include "MenuJeu.h"
#include "ProjectileMagma.h"
#include <vector>

using std::vector;
Mix_Chunk* EnnemiGiggler::sonAtk;
Mix_Chunk* EnnemiGiggler::sonMort;
Mix_Chunk* EnnemiGiggler::sonRandom;
SDL_Texture* EnnemiGiggler::image;
EnnemiGiggler::EnnemiGiggler(Case* c) : Ennemi(c)
{
    idEnnemi = IDEnnemi::GIGGLER;
    score = 1000;
    frameManagerBas = {0,0,4,11,100,0,1};
    frameManagerHaut = {4,0,4,11,100,0,1};
    frameManagerDroite = {8,0,4,11,100,0,1};
    frameManagerGauche = {1,1,4,11,100,0,1};
    frameManagerAtkBas = {5,1,1,11,100,0,1};
    frameManagerAtkHaut = {6,1,1,11,100,0,1};
    frameManagerAtkDroite = {7,1,1,11,100,0,1};
    frameManagerAtkGauche = {8,1,1,11,100,0,1};
    frameManagerDead = {9,1,2,11,150,0,1};

    frameManager = &frameManagerBas;
    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
    intervalAtk = 1500;
    vitesse = 1.2;
    tempsFrameAtk = 500;
}

void EnnemiGiggler::initialiserEnnemiGiggler()
{
    image = chargerSurface("../Images/Giggler.png");
    sonAtk = Mix_LoadWAV("../Sons/Magma.wav");
    sonRandom = Mix_LoadWAV("../Sons/GigglerNoise.wav");
    sonMort = Mix_LoadWAV("../Sons/GigglerDeath.wav");
}

Mix_Chunk* EnnemiGiggler::getSonRandom()
{
    return nullptr;
}

void EnnemiGiggler::releaseEnnemiGiggler()
{
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
    if(sonMort)
        Mix_FreeChunk(sonMort);
    sonMort = nullptr;
    if(sonAtk)
        Mix_FreeChunk(sonAtk);
    sonAtk = nullptr;
    if(sonRandom)
        Mix_FreeChunk(sonRandom);
    sonRandom = nullptr;
}

Mix_Chunk* EnnemiGiggler::getSonMort()
{
    return sonMort;
}

SDL_Texture* EnnemiGiggler::getImage()
{
    if(!alienMode)
        return image;
    else
        return imageAlien;
}
EnnemiGiggler::~EnnemiGiggler()
{

}

bool EnnemiGiggler::attaque()
{
     for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
    {
        Bomber* b = MenuJeu::getListeBomber().at(i);

         if (b->isGameEnded())
            continue;

          bool fire = true;
        Case* caseCollision = nullptr;
        switch(direction)
            {
            case Direction::HAUT :
                if(b->getPosition().x == casePersonnage->getPositionPixel().x && b->getCase()->getPositionDamier().y < casePersonnage->getPositionDamier().y)
                {
                     for(int i = casePersonnage->getPositionDamier().y;i>=1;i--)
                     {
                        if(Carte::getCase(i,casePersonnage->getPositionDamier().x)->getType() != TypeCase::FOND)
                        {
                            if(b->getCase()->getAdressePositionDamier()->y < i)
                                fire = false;
                            else
                                caseCollision = Carte::getCase(i,casePersonnage->getPositionDamier().x);
                            break;
                        }
                     }

                    if(fire)
                    {
                        tempsDerniereAtk = getTime();
                        Mix_PlayChannel(-1,sonAtk,0);
                        Carte::ajouterProjectile(new ProjectileMagma(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y },direction));
                        return true;
                    }
                }

                break;
            case Direction::BAS :
                if(b->getPosition().x == casePersonnage->getPositionPixel().x && b->getCase()->getPositionDamier().y > casePersonnage->getPositionDamier().y)
                {
                    for(int i = casePersonnage->getPositionDamier().y;i<=CASE_HAUTEUR;i++)
                    {
                        if(Carte::getCase(i,casePersonnage->getPositionDamier().x)->getType() != TypeCase::FOND)
                        {
                            if(b->getCase()->getAdressePositionDamier()->y > i)
                                fire = false;
                            else
                                caseCollision = Carte::getCase(i,casePersonnage->getPositionDamier().x);
                            break;
                        }
                    }
                    if(fire)
                    {
                        tempsDerniereAtk = getTime();
                        Mix_PlayChannel(-1,sonAtk,0);
                        Carte::ajouterProjectile(new ProjectileMagma(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y + CASE_DIMENSION },direction));
                        return true;
                    }
                }
                break;
            case Direction::DROITE :
               if(b->getCase()->getPositionDamier().x > casePersonnage->getPositionDamier().x && b->getPosition().y == casePersonnage->getPositionPixel().y)
                {
                    for(int i = casePersonnage->getPositionDamier().x;i<=CASE_LONGUEUR;i++)
                    {
                        if(Carte::getCase(casePersonnage->getPositionDamier().y,i)->getType() != TypeCase::FOND)
                        {
                            if(b->getCase()->getAdressePositionDamier()->x > i)
                                fire = false;
                            else
                                caseCollision = Carte::getCase(casePersonnage->getPositionDamier().y,i);
                            break;
                        }
                    }
                    if(fire)
                    {
                        tempsDerniereAtk = getTime();
                        Mix_PlayChannel(-1,sonAtk,0);
                        Carte::ajouterProjectile(new ProjectileMagma(this,caseCollision,{positionReal.x + CASE_DIMENSION,positionReal.y + CASE_DIMENSION/2 },direction));
                        return true;
                    }
                }
                break;
            case Direction::GAUCHE :
              if(b->getCase()->getPositionDamier().x < casePersonnage->getPositionDamier().x && b->getPosition().y == casePersonnage->getPositionPixel().y)
                {
                    for(int i = casePersonnage->getPositionDamier().x;i>=1;i--)
                    {
                        if(Carte::getCase(casePersonnage->getPositionDamier().y,i)->getType() != TypeCase::FOND)
                        {
                            if(b->getCase()->getAdressePositionDamier()->x < i)
                                fire = false;
                            else
                                caseCollision = Carte::getCase(casePersonnage->getPositionDamier().y,i);
                            break;
                        }
                    }
                    if(fire)
                    {
                        tempsDerniereAtk = getTime();
                        Mix_PlayChannel(-1,sonAtk,0);
                        Carte::ajouterProjectile(new ProjectileMagma(this,caseCollision,{positionReal.x,positionReal.y + CASE_DIMENSION/2 },direction));
                        return true;
                    }
                }
                break;
            default :
                break;
            }

    }
    return false;
}
void EnnemiGiggler::deplacerEnnemi() // Calcul direction
{
       if(!enDeplacementVersCase)
       {
           vector<Direction> listeDirection;
           vector<Case*> listeCase;
           vector<Direction> listeDirectionInverse;

           for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
           {
               Bomber* b = MenuJeu::getListeBomber().at(i);
               if(b->getPosition().x == getPosition().x)
               {
                   if(b->getPosition().y > getPosition().y)
                   {
                         if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x))
                         {
                            if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x)->getType() == TypeCase::FOND)
                            {

                                    listeDirection.push_back(Direction::BAS);
                                    listeDirectionInverse.push_back(Direction::HAUT);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x));

                            }
                         }
                   }
                   else if (b->getPosition().y < getPosition().y)
                   {
                       if(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x))
                       {
                            if(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x)->getType() == TypeCase::FOND)
                            {

                                    listeDirection.push_back(Direction::HAUT);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x));
                                    listeDirectionInverse.push_back(Direction::BAS);


                            }
                       }
                   }
               }
               else if(b->getPosition().y == getPosition().y)
               {
                   if(b->getPosition().x > getPosition().x)
                   {
                        if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1))
                        {
                            if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1)->getType() == TypeCase::FOND)
                            {

                                    listeDirection.push_back(Direction::DROITE);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1));
                                    listeDirectionInverse.push_back(Direction::GAUCHE);

                            }

                        }
                    }
                    else if(b->getPosition().x < getPosition().x)
                    {
                        if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1))
                        {
                            if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1)->getType() == TypeCase::FOND)
                            {

                                    listeDirection.push_back(Direction::GAUCHE);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1));
                                    listeDirectionInverse.push_back(Direction::DROITE);

                            }
                        }
                    }
               }
           }

           if(!listeDirection.empty())
           {

               unsigned int nombre = genererNombreAleatoire(listeDirection.size())-1;

                if((listeDirection.at(nombre) != direction) && !alienMode)
                {
                    if(getTime() - tempsSonRandom > 2000)
                    {
                        tempsSonRandom=getTime();
                        Mix_PlayChannel(-1,sonRandom,0);
                    }
                }

               direction = listeDirection.at(nombre);
               caseToGo = listeCase.at(nombre);
               casePrecedente = caseToGo;
               directionInverse = listeDirectionInverse.at(nombre);
               listeCase.clear();
               listeDirection.clear();
               listeDirectionInverse.clear();
           }
           else
           {

               if(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x))
               {
                   if(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x)->getType() == TypeCase::FOND)
                   {
                       if(Direction::BAS!=direction)
                       {
                        listeDirection.push_back(Direction::HAUT);
                        listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x));
                        listeDirectionInverse.push_back(Direction::BAS);
                       }


                   }
               }
               if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x))
               {
                   if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x)->getType() == TypeCase::FOND)
                   {
                       if(Direction::HAUT!=direction)
                       {
                        listeDirection.push_back(Direction::BAS);
                        listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x));
                        listeDirectionInverse.push_back(Direction::HAUT);
                       }

                   }
               }
              if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1))
               {
                   if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1)->getType() == TypeCase::FOND)
                   {
                       if(Direction::GAUCHE!=direction)
                       {
                        listeDirection.push_back(Direction::DROITE);
                        listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1));
                        listeDirectionInverse.push_back(Direction::GAUCHE);
                       }

                   }
               }
               if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1))
               {
                   if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1)->getType() == TypeCase::FOND)
                   {
                       if(Direction::DROITE!=direction)
                       {
                        listeDirection.push_back(Direction::GAUCHE);
                        listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1));
                        listeDirectionInverse.push_back(Direction::DROITE);
                       }

                   }
               }
               if(!listeDirection.empty())
               {
                   unsigned int nombre = genererNombreAleatoire(listeDirection.size())-1;
                   direction = listeDirection.at(nombre);
                   caseToGo = listeCase.at(nombre);
                   casePrecedente = caseToGo;
                   directionInverse = listeDirectionInverse.at(nombre);
                   listeCase.clear();
                   listeDirection.clear();
                   listeDirectionInverse.clear();
               }
               else
                    faitDemiTour();


           }
        }

        enDeplacementVersCase = !goToCase();
}
