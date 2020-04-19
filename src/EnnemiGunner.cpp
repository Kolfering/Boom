#include "EnnemiGunner.h"
#include "Utile.h"
#include "Carte.h"
#include "MenuJeu.h"
#include "ProjectileMGShot.h"
#include <vector>

using std::vector;
Mix_Chunk* EnnemiGunner::sonAtk;
Mix_Chunk* EnnemiGunner::sonMort;
Mix_Chunk* EnnemiGunner::sonRandom;
SDL_Texture* EnnemiGunner::image;
EnnemiGunner::EnnemiGunner(Case* c) : Ennemi(c)
{
    idEnnemi = IDEnnemi::GUNNER;
    score = 600;
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
    intervalAtk = 200;
    vitesse = 0.6;
    tempsFrameAtk = 200;
}

void EnnemiGunner::initialiserEnnemiGunner()
{
    image = chargerSurface("Images/Gunner.png");
    sonAtk = Mix_LoadWAV("Sons/Pulse.wav");
    sonMort = Mix_LoadWAV("Sons/GunnerDeath.wav");
    sonRandom = Mix_LoadWAV("Sons/GunnerNoise.wav");
}

Mix_Chunk* EnnemiGunner::getSonRandom()
{
    return sonRandom;
}

void EnnemiGunner::releaseEnnemiGunner()
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

Mix_Chunk* EnnemiGunner::getSonMort()
{
    return sonMort;
}

SDL_Texture* EnnemiGunner::getImage()
{
    if(!alienMode)
        return image;
    else
        return imageAlien;
}
EnnemiGunner::~EnnemiGunner()
{

}

void EnnemiGunner::accelerer()
{
    vitesse *=2;
    if(vitesse>2)
        vitesse = 2;
}
bool EnnemiGunner::attaque()
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
                        Carte::ajouterProjectile(new ProjectileMGShot(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y },direction));
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
                        Carte::ajouterProjectile(new ProjectileMGShot(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y + CASE_DIMENSION },direction));
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
                        Carte::ajouterProjectile(new ProjectileMGShot(this,caseCollision,{positionReal.x + CASE_DIMENSION,positionReal.y + CASE_DIMENSION*3/4 },direction));
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
                        Carte::ajouterProjectile(new ProjectileMGShot(this,caseCollision,{positionReal.x,positionReal.y + CASE_DIMENSION*3/4 },direction));
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
void EnnemiGunner::deplacerEnnemi() // Calcul direction
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
