#include "EnnemiLizzy.h"
#include "Utile.h"
#include "Carte.h"
#include "MenuJeu.h"
#include "ProjectileFireBall.h"
#include <vector>

using std::vector;
Mix_Chunk* EnnemiLizzy::sonAtk;
Mix_Chunk* EnnemiLizzy::sonMort;
Mix_Chunk* EnnemiLizzy::sonRandom;
SDL_Texture* EnnemiLizzy::image;
EnnemiLizzy::EnnemiLizzy(Case* c) : Ennemi(c)
{
    idEnnemi = IDEnnemi::LIZZY;
    score = 400;
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
    intervalAtk = 1500;
    vitesse = 0.6;
    tempsFrameAtk = 250;
}

void EnnemiLizzy::initialiserEnnemiLizzy()
{
    image = chargerSurface("../Images/Lizzy.png");
    sonAtk = Mix_LoadWAV("../Sons/FireBall.wav");
    sonMort = Mix_LoadWAV("../Sons/LizzyDeath.wav");
    sonRandom = Mix_LoadWAV("../Sons/LizzyNoise.wav");
}

Mix_Chunk* EnnemiLizzy::getSonRandom()
{
    return sonRandom;
}

void EnnemiLizzy::releaseEnnemiLizzy()
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

Mix_Chunk* EnnemiLizzy::getSonMort()
{
    return sonMort;
}

SDL_Texture* EnnemiLizzy::getImage()
{
     if(!alienMode)
        return image;
    else
        return imageAlien;
}
EnnemiLizzy::~EnnemiLizzy()
{

}

bool EnnemiLizzy::attaque()
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
                        Carte::ajouterProjectile(new ProjectileFireBall(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y},direction));
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
                        Carte::ajouterProjectile(new ProjectileFireBall(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y + CASE_DIMENSION },direction));
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
                        Carte::ajouterProjectile(new ProjectileFireBall(this,caseCollision,{positionReal.x + CASE_DIMENSION,positionReal.y + CASE_DIMENSION*3/4 },direction));
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
                        Carte::ajouterProjectile(new ProjectileFireBall(this,caseCollision,{positionReal.x,positionReal.y + CASE_DIMENSION*3/4 },direction));
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
void EnnemiLizzy::deplacerEnnemi() // Calcul direction
{
        gestionSonRandom();

       if(!enDeplacementVersCase)
       {
           vector<Direction> listeDirection;
           vector<Case*> listeCase;
           vector<Direction> listeDirectionInverse;

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

        enDeplacementVersCase = !goToCase();
}
