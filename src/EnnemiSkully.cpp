#include "EnnemiSkully.h"
#include "Utile.h"
#include "Carte.h"
#include "MenuJeu.h"
#include "ProjectilePlasma.h"
#include <vector>

using std::vector;
Mix_Chunk* EnnemiSkully::sonAtk;
Mix_Chunk* EnnemiSkully::sonMort;
Mix_Chunk* EnnemiSkully::sonRandom;
SDL_Texture* EnnemiSkully::image;
EnnemiSkully::EnnemiSkully(Case* c) : Ennemi(c)
{
    idEnnemi = SKULLY;
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
    intervalAtk = 200;
    vitesse = 0.6;
    tempsFrameAtk = 200;
}

void EnnemiSkully::initialiserEnnemiSkully()
{
    image = chargerSurface("../Images/Skully.png");
    sonAtk = Mix_LoadWAV("../Sons/Plasma.wav");
    sonRandom = Mix_LoadWAV("../Sons/SkullyNoise.wav");
    sonMort = Mix_LoadWAV("../Sons/SkullyDeath.wav");
}

Mix_Chunk* EnnemiSkully::getSonRandom()
{
    return nullptr;
}

void EnnemiSkully::accelerer()
{
    vitesse *=2;
    if(vitesse>2)
        vitesse = 2;
}

void EnnemiSkully::releaseEnnemiSkully()
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

Mix_Chunk* EnnemiSkully::getSonMort()
{
    return sonMort;
}

SDL_Texture* EnnemiSkully::getImage()
{
    if(!alienMode)
        return image;
    else
        return imageAlien;
}
EnnemiSkully::~EnnemiSkully()
{

}

bool EnnemiSkully::attaque()
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
            case HAUT :
                if(b->getPosition().x == casePersonnage->getPositionPixel().x && b->getCase()->getPositionDamier().y < casePersonnage->getPositionDamier().y)
                {
                     for(int i = casePersonnage->getPositionDamier().y;i>=1;i--)
                     {
                        if(Carte::getCase(i,casePersonnage->getPositionDamier().x)->getType() != FOND)
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
                        Carte::ajouterProjectile(new ProjectilePlasma(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y },direction));
                        return true;
                    }
                }

                break;
            case BAS :
                if(b->getPosition().x == casePersonnage->getPositionPixel().x && b->getCase()->getPositionDamier().y > casePersonnage->getPositionDamier().y)
                {
                    for(int i = casePersonnage->getPositionDamier().y;i<=CASE_HAUTEUR;i++)
                    {
                        if(Carte::getCase(i,casePersonnage->getPositionDamier().x)->getType() != FOND)
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
                        Carte::ajouterProjectile(new ProjectilePlasma(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y + CASE_DIMENSION },direction));
                        return true;
                    }
                }
                break;
            case DROITE :
               if(b->getCase()->getPositionDamier().x > casePersonnage->getPositionDamier().x && b->getPosition().y == casePersonnage->getPositionPixel().y)
                {
                    for(int i = casePersonnage->getPositionDamier().x;i<=CASE_LONGUEUR;i++)
                    {
                        if(Carte::getCase(casePersonnage->getPositionDamier().y,i)->getType() != FOND)
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
                        Carte::ajouterProjectile(new ProjectilePlasma(this,caseCollision,{positionReal.x + CASE_DIMENSION,positionReal.y + CASE_DIMENSION/2 },direction));
                        return true;
                    }
                }
                break;
            case GAUCHE :
              if(b->getCase()->getPositionDamier().x < casePersonnage->getPositionDamier().x && b->getPosition().y == casePersonnage->getPositionPixel().y)
                {
                    for(int i = casePersonnage->getPositionDamier().x;i>=1;i--)
                    {
                        if(Carte::getCase(casePersonnage->getPositionDamier().y,i)->getType() != FOND)
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
                        Carte::ajouterProjectile(new ProjectilePlasma(this,caseCollision,{positionReal.x,positionReal.y + CASE_DIMENSION/2 },direction));
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
void EnnemiSkully::deplacerEnnemi() // Calcul direction
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
                            if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x)->getType() == FOND)
                            {

                                    listeDirection.push_back(BAS);
                                    listeDirectionInverse.push_back(HAUT);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x));

                            }
                         }
                   }
                   else if (b->getPosition().y < getPosition().y)
                   {
                       if(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x))
                       {
                            if(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x)->getType() == FOND)
                            {

                                    listeDirection.push_back(HAUT);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x));
                                    listeDirectionInverse.push_back(BAS);


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
                            if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1)->getType() == FOND)
                            {

                                    listeDirection.push_back(DROITE);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1));
                                    listeDirectionInverse.push_back(GAUCHE);

                            }

                        }
                    }
                    else if(b->getPosition().x < getPosition().x)
                    {
                        if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1))
                        {
                            if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1)->getType() == FOND)
                            {

                                    listeDirection.push_back(GAUCHE);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1));
                                    listeDirectionInverse.push_back(DROITE);

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
                   if(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x)->getType() == FOND)
                   {
                       if(BAS!=direction)
                       {
                        listeDirection.push_back(HAUT);
                        listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x));
                        listeDirectionInverse.push_back(BAS);
                       }


                   }
               }
               if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x))
               {
                   if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x)->getType() == FOND)
                   {
                       if(HAUT!=direction)
                       {
                        listeDirection.push_back(BAS);
                        listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x));
                        listeDirectionInverse.push_back(HAUT);
                       }


                   }
               }
              if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1))
               {
                   if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1)->getType() == FOND)
                   {
                       if(GAUCHE!=direction)
                       {
                        listeDirection.push_back(DROITE);
                        listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1));
                        listeDirectionInverse.push_back(GAUCHE);
                       }

                   }
               }
               if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1))
               {
                   if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1)->getType() == FOND)
                   {
                       if(DROITE!=direction)
                       {
                        listeDirection.push_back(GAUCHE);
                        listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1));
                        listeDirectionInverse.push_back(DROITE);
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
