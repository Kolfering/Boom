#include "EnnemiHumain.h"
#include "Carte.h"
#include "Utile.h"
#include "MenuJeu.h"
#include "ProjectileShot.h"
#include <vector>

EnnemiHumain::EnnemiHumain(Case* c) : Ennemi(c)
{
    vitesse = 0.6;
    tempsFrameAtk = 250;
}

EnnemiHumain::~EnnemiHumain()
{

}

bool EnnemiHumain::attaque()
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
                        Mix_PlayChannel(-1,getSonAtk(),0);
                        Carte::ajouterProjectile(new ProjectileShot(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y},direction));
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
                        Mix_PlayChannel(-1,getSonAtk(),0);
                        Carte::ajouterProjectile(new ProjectileShot(this,caseCollision,{positionReal.x + CASE_DIMENSION/2,positionReal.y + CASE_DIMENSION },direction));
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
                        Mix_PlayChannel(-1,getSonAtk(),0);
                        Carte::ajouterProjectile(new ProjectileShot(this,caseCollision,{positionReal.x + CASE_DIMENSION,positionReal.y + CASE_DIMENSION*2/3 },direction));
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
                        Mix_PlayChannel(-1,getSonAtk(),0);
                        Carte::ajouterProjectile(new ProjectileShot(this,caseCollision,{positionReal.x,positionReal.y + CASE_DIMENSION*2/3 },direction));
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

void EnnemiHumain::deplacerEnnemi() // Calcul direction
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

                    listeDirection.push_back(Direction::HAUT);
                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x));
                    listeDirectionInverse.push_back(Direction::BAS);



               }
           }
           if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x))
           {
               if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x)->getType() == TypeCase::FOND)
               {

                    listeDirection.push_back(Direction::BAS);
                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x));
                    listeDirectionInverse.push_back(Direction::HAUT);



               }
           }
          if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1))
           {
               if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1)->getType() == TypeCase::FOND)
               {

                    listeDirection.push_back(Direction::DROITE);
                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1));
                    listeDirectionInverse.push_back(Direction::GAUCHE);


               }
           }
           if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1))
           {
               if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1)->getType() == TypeCase::FOND)
               {

                    listeDirection.push_back(Direction::GAUCHE);
                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1));
                    listeDirectionInverse.push_back(Direction::DROITE);


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
