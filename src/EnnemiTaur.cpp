#include "EnnemiTaur.h"
#include "Utile.h"
#include "Carte.h"
#include "MenuJeu.h"
#include <vector>

using std::vector;
Mix_Chunk* EnnemiTaur::sonAtk;
Mix_Chunk* EnnemiTaur::sonMort;
SDL_Texture* EnnemiTaur::image;
EnnemiTaur::EnnemiTaur(Case* c) : Ennemi(c)
{
    idEnnemi = IDEnnemi::TAUR;
    score = 500;
    frameManagerBas = {0,0,4,11,100,0,1};
    frameManagerHaut = {4,0,4,11,100,0,1};
    frameManagerDroite = {8,0,4,11,100,0,1};
    frameManagerGauche = {1,1,4,11,100,0,1};
    frameManagerAtkBas = {5,1,1,11,200,0,1};
    frameManagerAtkHaut = {6,1,1,11,200,0,1};
    frameManagerAtkDroite = {7,1,1,11,200,0,1};
    frameManagerAtkGauche = {8,1,1,11,200,0,1};
    frameManagerDead = {9,1,2,11,150,0,1};

    frameManager = &frameManagerBas;
    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
    intervalAtk = 350;
    vitesse = 1.2;
    degat = 2;
    tempsFrameAtk = 50;
    attacked = false;
}

void EnnemiTaur::initialiserEnnemiTaur()
{
    image = chargerSurface("../Images/Taur.png");
    sonAtk = Mix_LoadWAV("../Sons/TaurNoise.wav");
    sonMort = Mix_LoadWAV("../Sons/TaurDeath.wav");
}

Mix_Chunk* EnnemiTaur::getSonRandom()
{
    return nullptr;
}

void EnnemiTaur::releaseEnnemiTaur()
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
}

Mix_Chunk* EnnemiTaur::getSonMort()
{
    return sonMort;
}

SDL_Texture* EnnemiTaur::getImage()
{
    if(!alienMode)
        return image;
    else
        return imageAlien;
}
EnnemiTaur::~EnnemiTaur()
{

}

bool EnnemiTaur::attaque()
{

    for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
    {
        Bomber* b = MenuJeu::getListeBomber().at(i);

          if (b->isGameEnded())
            continue;

        if(caseToGo->equals(b->getCaseToGo()) || caseToGo->equals(b->getCase()) || casePersonnage->equals(b->getCaseToGo()) ||  casePersonnage->equals(b->getCase()))
        {
            switch(direction)
            {
            case Direction::HAUT:
                if(getPosition().y < b->getPosition().y + CASE_DIMENSION)
                {
                      attacked = true;
                      break;
                }

                break;
            case Direction::BAS:
                if(getPosition().y + CASE_DIMENSION > b->getPosition().y)
                {
                      attacked = true;
                      break;
                }

                break;
            case Direction::DROITE:
                if(getPosition().x + CASE_DIMENSION > b->getPosition().x)
                {
                      attacked = true;
                      break;
                }

                break;
            case Direction::GAUCHE:
                if(getPosition().x < b->getPosition().x + CASE_DIMENSION)
                {
                      attacked = true;
                      break;
                }
                break;
            default :
                break;
            }
        }
        if(attacked)
        {
            tempsDerniereAtk = getTime();
            Mix_PlayChannel(-1,sonAtk,0);
            return true;
        }

    }
    return false;
}
void EnnemiTaur::deplacerEnnemi() // Calcul direction
{

       if(!enDeplacementVersCase)
       {
           if(attacked)
           {
               faitDemiTour();
               attacked = false;
           }
           else
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
                listeDirectionInverse.clear();
                listeDirection.clear();


           }
           else
                faitDemiTour();

       }
    }

        enDeplacementVersCase = !goToCase();
}
