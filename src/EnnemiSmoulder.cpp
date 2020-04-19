#include "EnnemiSmoulder.h"
#include "Utile.h"
#include "Carte.h"
#include "MenuJeu.h"
#include "ProjectileFlame.h"
#include <vector>
#include <algorithm>

using std::vector;
Mix_Chunk* EnnemiSmoulder::sonAtk;
Mix_Chunk* EnnemiSmoulder::sonMort;
Mix_Chunk* EnnemiSmoulder::sonRandom;
SDL_Texture* EnnemiSmoulder::image;
EnnemiSmoulder::EnnemiSmoulder(Case* c) : Ennemi(c)
{
    idEnnemi = IDEnnemi::SMOULDER;
    score = 900;
    frameManagerBas = {0,0,4,12,100,0,1};
    frameManagerHaut = {4,0,4,12,100,0,1};
    frameManagerDroite = {8,0,4,12,100,0,1};
    frameManagerGauche = {0,1,4,12,100,0,1};
    frameManagerAtkBas = {4,1,1,12,100,0,1};
    frameManagerAtkHaut = {5,1,1,12,100,0,1};
    frameManagerAtkDroite = {6,1,1,12,100,0,1};
    frameManagerAtkGauche = {7,1,1,12,100,0,1};
    frameManagerDead = {8,1,4,12,150,0,1};

    frameManager = &frameManagerBas;
    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
    intervalAtk = 200;
    vitesse = 0.6;
    tempsFrameAtk = 200;
    range = 5;
}

void EnnemiSmoulder::initialiserEnnemiSmoulder()
{
    image = chargerSurface("../Images/Smoulder.png");
    sonAtk = Mix_LoadWAV("../Sons/Flame.wav");
    sonMort = Mix_LoadWAV("../Sons/SmoulderDeath.wav");
    sonRandom = Mix_LoadWAV("../Sons/SmoulderNoise.wav");
}

Mix_Chunk* EnnemiSmoulder::getSonRandom()
{
    return nullptr;
}

void EnnemiSmoulder::releaseEnnemiSmoulder()
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

Mix_Chunk* EnnemiSmoulder::getSonMort()
{
    return sonMort;
}

SDL_Texture* EnnemiSmoulder::getImage()
{
    if(!alienMode)
        return image;
    else
        return imageAlien;
}
EnnemiSmoulder::~EnnemiSmoulder()
{

}

void EnnemiSmoulder::accelerer()
{
    vitesse *=2;
    if(vitesse>2)
        vitesse = 2;
}
bool EnnemiSmoulder::attaque()
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
                    if(std::abs(b->getCase()->getPositionDamier().y - casePersonnage->getPositionDamier().y) < range)
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
                            Carte::ajouterProjectile(new ProjectileFlame(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y },direction));
                            return true;
                        }
                    }
                }

                break;
            case Direction::BAS :
                if(b->getPosition().x == casePersonnage->getPositionPixel().x && b->getCase()->getPositionDamier().y > casePersonnage->getPositionDamier().y)
                {
                    if(std::abs(b->getCase()->getPositionDamier().y - casePersonnage->getPositionDamier().y) < range)
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
                            Carte::ajouterProjectile(new ProjectileFlame(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y + CASE_DIMENSION + CASE_DIMENSION/3 },direction));
                            return true;
                        }
                    }
                }
                break;
            case Direction::DROITE :
               if(b->getCase()->getPositionDamier().x > casePersonnage->getPositionDamier().x && b->getPosition().y == casePersonnage->getPositionPixel().y)
                {
                    if(std::abs(b->getCase()->getPositionDamier().x - casePersonnage->getPositionDamier().x) < range)
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
                            Carte::ajouterProjectile(new ProjectileFlame(this,caseCollision,{positionReal.x + CASE_DIMENSION*3/2,positionReal.y + CASE_DIMENSION/2 },direction));
                            return true;
                        }
                    }
                }
                break;
            case Direction::GAUCHE :
              if(b->getCase()->getPositionDamier().x < casePersonnage->getPositionDamier().x && b->getPosition().y == casePersonnage->getPositionPixel().y)
                {
                    if(std::abs(b->getCase()->getPositionDamier().x - casePersonnage->getPositionDamier().x) < range)
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
                            Carte::ajouterProjectile(new ProjectileFlame(this,caseCollision,{positionReal.x ,positionReal.y + CASE_DIMENSION/2 },direction));
                            return true;
                        }
                    }
                }
                break;
            default :
                break;
            }

    }
    return false;
}

void EnnemiSmoulder::deplacerEnnemi() // Calcul direction
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
