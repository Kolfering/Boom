#include "EnnemiGhost.h"
#include "Utile.h"
#include "Carte.h"
#include "MenuJeu.h"
#include <vector>

using std::vector;
Mix_Chunk* EnnemiGhost::sonAtk;
Mix_Chunk* EnnemiGhost::sonMort;
SDL_Texture* EnnemiGhost::image;
EnnemiGhost::EnnemiGhost(Case* c) : Ennemi(c)
{
    idEnnemi = GHOST;
    score = 800;
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
    intervalAtk = 2000;
    vitesse = 0.6;
    vitesseBase = 0.6;
    vitesseAtk = 2;
    tempsFrameAtk = 99999;
    spotted = false;
    frameAllowed = true;
}

void EnnemiGhost::initialiserEnnemiGhost()
{
    image = chargerSurface("Images/Ghost.png");
    sonAtk = Mix_LoadWAV("Sons/GhostNoise.wav");
    sonMort = Mix_LoadWAV("Sons/GhostDeath.wav");
}

Mix_Chunk* EnnemiGhost::getSonRandom()
{
    return nullptr;
}

void EnnemiGhost::releaseEnnemiGhost()
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

Mix_Chunk* EnnemiGhost::getSonMort()
{
    return sonMort;
}

SDL_Texture* EnnemiGhost::getImage()
{
    if(!alienMode)
        return image;
    else
        return imageAlien;
}
EnnemiGhost::~EnnemiGhost()
{

}

void EnnemiGhost::accelerer()
{
    vitesseAtk *= 2;
    vitesseBase *= 2;
    vitesse *= 2;
    if(vitesse>2)
        vitesse = 2;
    intervalAtk /= 2;
}

bool EnnemiGhost::attaque()
{
     for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
    {
        Bomber* b = MenuJeu::getListeBomber().at(i);

          if (b->isGameEnded())
            continue;

        bool fire = true;
        switch(direction)
            {
            case HAUT :
                if(b->getPosition().x == casePersonnage->getPositionPixel().x && b->getCase()->getPositionDamier().y < casePersonnage->getPositionDamier().y)
                {
                     for(int i = casePersonnage->getPositionDamier().y;i > b->getCase()->getPositionDamier().y;i--)
                     {
                        if(Carte::getCase(i,casePersonnage->getPositionDamier().x)->getType() != FOND)
                        {
                            fire = false;
                            break;

                        }
                     }

                    if(fire)
                    {
                        tempsDerniereAtk = getTime();
                        Mix_PlayChannel(-1,sonAtk,0);
                        directionSpotted = HAUT;
                        spotted = true;
                        vitesse = vitesseAtk;
                        return true;
                    }
                }

                break;
            case BAS :
                if(b->getPosition().x == casePersonnage->getPositionPixel().x && b->getCase()->getPositionDamier().y > casePersonnage->getPositionDamier().y)
                {
                    for(int i = casePersonnage->getPositionDamier().y;i< b->getCase()->getPositionDamier().y ;i++)
                    {
                        if(Carte::getCase(i,casePersonnage->getPositionDamier().x)->getType() != FOND)
                        {
                            fire = false;
                            break;
                        }
                    }
                    if(fire)
                    {
                        tempsDerniereAtk = getTime();
                        Mix_PlayChannel(-1,sonAtk,0);
                        directionSpotted = BAS;
                        spotted = true;
                        vitesse = vitesseAtk;
                        return true;
                    }
                }
                break;
            case DROITE :
               if(b->getCase()->getPositionDamier().x > casePersonnage->getPositionDamier().x && b->getPosition().y == casePersonnage->getPositionPixel().y)
                {
                    for(int i = casePersonnage->getPositionDamier().x;i< b->getCase()->getPositionDamier().x;i++)
                    {
                        if(Carte::getCase(casePersonnage->getPositionDamier().y,i)->getType() != FOND)
                        {

                            fire = false;
                            break;

                        }
                    }
                    if(fire)
                    {
                        tempsDerniereAtk = getTime();
                        Mix_PlayChannel(-1,sonAtk,0);
                        directionSpotted = DROITE;
                        spotted = true;
                        vitesse = vitesseAtk;
                        return true;
                    }
                }
                break;
            case GAUCHE :
              if(b->getCase()->getPositionDamier().x < casePersonnage->getPositionDamier().x && b->getPosition().y == casePersonnage->getPositionPixel().y)
                {
                    for(int i = casePersonnage->getPositionDamier().x;i> b->getCase()->getPositionDamier().x;i--)
                    {
                        if(Carte::getCase(casePersonnage->getPositionDamier().y,i)->getType() != FOND)
                        {
                            fire = false;
                            break;

                        }
                    }
                    if(fire)
                    {
                        tempsDerniereAtk = getTime();
                        Mix_PlayChannel(-1,sonAtk,0);
                        directionSpotted = GAUCHE;
                        spotted = true;
                        vitesse = vitesseAtk;
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

bool EnnemiGhost::goToCase()
{
    if(meurt)
    {
        if(getTime() - timeMort > TEMPS_MORT)
            mort = true;
    if(!alienMode)
    {
        if(!compareFrameManager(*frameManager,frameManagerDead))
        {
             counterFrame.x = frameManagerDead.x;
             counterFrame.y = frameManagerDead.y;
             counterFrame.nbLigne = 0;
        }
        frameManager = &frameManagerDead;
        calculNextFrame();
        return false;
    }
    else
    {
        if(!compareFrameManager(*frameManager,frameManagerAlienMort))
        {
             counterFrame.x = frameManagerAlienMort.x;
             counterFrame.y = frameManagerAlienMort.y;
             counterFrame.nbLigne = 0;
        }
        frameManager = &frameManagerAlienMort;
        calculNextFrame();
        return false;
    }


    }
    if(caseToGo->getExplosion())
    {
        prendreDegat(caseToGo->getExplosion()->getBombe()->getBomber(), 0);
        return false;
    }


    collisionBomber();


    if(aAtteintCase())
    {
        this->setCase(caseToGo);
        this->caseToGo = casePersonnage;
        this->casePrecedente = casePersonnage;
        return true;
    }



    if(!collision())
    {
        if(!frameAtkActif || frameAllowed)
        {
            switch(direction)
            {
            case HAUT :
                positionReal.y-= vitesse;
                break;
            case BAS :
                positionReal.y+= vitesse;
                break;
            case DROITE :
                positionReal.x+= vitesse;
                break;
            case GAUCHE :
                positionReal.x-= vitesse;
                break;
            default :
                break;
            }
        }


    }
    else
    {
        if(spotted)
        {
            spotted = false;
            vitesse = vitesseBase;
            frameAtkActif = false;
            tempsDerniereAtk = getTime();
        }
        faitDemiTour();
    }


if(!alienMode)
{
    if(getTime() - tempsDerniereAtk > intervalAtk)
    {
         frameAtkActif = attaque();

         if(frameAtkActif)
         {
              tempsDerniereAtk = getTime();
              directionAtk = direction;
         }

    }
}

if(getTime() - tempsDerniereAtk > tempsFrameAtk)
    frameAtkActif = false;

if(!alienMode)
{
    if(!frameAtkActif)
    {
            switch(direction)
            {
            case HAUT :
                if(!compareFrameManager(*frameManager,frameManagerHaut))
                {
                     counterFrame.x = frameManagerHaut.x;
                     counterFrame.y = frameManagerHaut.y;
                     counterFrame.nbLigne = 0;
                }
                frameManager = &frameManagerHaut;
                break;
            case BAS :
                if(!compareFrameManager(*frameManager,frameManagerBas))
                {
                     counterFrame.x = frameManagerBas.x;
                     counterFrame.y = frameManagerBas.y;
                     counterFrame.nbLigne = 0;
                }
                frameManager = &frameManagerBas;
                break;
            case DROITE :
                if(!compareFrameManager(*frameManager,frameManagerDroite))
                {
                     counterFrame.x = frameManagerDroite.x;
                     counterFrame.y = frameManagerDroite.y;
                     counterFrame.nbLigne = 0;
                }
                frameManager = &frameManagerDroite;
                break;
            case GAUCHE :
                if(!compareFrameManager(*frameManager,frameManagerGauche))
                {
                     counterFrame.x = frameManagerGauche.x;
                     counterFrame.y = frameManagerGauche.y;
                     counterFrame.nbLigne = 0;
                }
                frameManager = &frameManagerGauche;
                break;
            default :
                break;
            }
    }
    else
    {
         switch(directionAtk)
            {
            case HAUT :
                if(!compareFrameManager(*frameManager,frameManagerAtkHaut))
                {
                     counterFrame.x = frameManagerAtkHaut.x;
                     counterFrame.y = frameManagerAtkHaut.y;
                     counterFrame.nbLigne = 0;
                }
                frameManager = &frameManagerAtkHaut;
                break;
            case BAS :
                if(!compareFrameManager(*frameManager,frameManagerAtkBas))
                {
                     counterFrame.x = frameManagerAtkBas.x;
                     counterFrame.y = frameManagerAtkBas.y;
                     counterFrame.nbLigne = 0;
                }
                frameManager = &frameManagerAtkBas;
                break;
            case DROITE :
                if(!compareFrameManager(*frameManager,frameManagerAtkDroite))
                {
                     counterFrame.x = frameManagerAtkDroite.x;
                     counterFrame.y = frameManagerAtkDroite.y;
                     counterFrame.nbLigne = 0;
                }
                frameManager = &frameManagerAtkDroite;
                break;
            case GAUCHE :
                if(!compareFrameManager(*frameManager,frameManagerAtkGauche))
                {
                     counterFrame.x = frameManagerAtkGauche.x;
                     counterFrame.y = frameManagerAtkGauche.y;
                     counterFrame.nbLigne = 0;
                }
                frameManager = &frameManagerAtkGauche;
                break;
            default :
                break;
            }
    }
}
else
{
     switch(direction)
    {
    case HAUT :
        if(!compareFrameManager(*frameManager,frameManagerAlienHaut))
        {
             counterFrame.x = frameManagerAlienHaut.x;
             counterFrame.y = frameManagerAlienHaut.y;
             counterFrame.nbLigne = 0;
        }
        frameManager = &frameManagerAlienHaut;
        break;
    case BAS :
        if(!compareFrameManager(*frameManager,frameManagerAlienBas))
        {
             counterFrame.x = frameManagerAlienBas.x;
             counterFrame.y = frameManagerAlienBas.y;
             counterFrame.nbLigne = 0;
        }
        frameManager = &frameManagerAlienBas;
        break;
    case DROITE :
        if(!compareFrameManager(*frameManager,frameManagerAlienDroite))
        {
             counterFrame.x = frameManagerAlienDroite.x;
             counterFrame.y = frameManagerAlienDroite.y;
             counterFrame.nbLigne = 0;
        }
        frameManager = &frameManagerAlienDroite;
        break;
    case GAUCHE :
        if(!compareFrameManager(*frameManager,frameManagerAlienGauche))
        {
             counterFrame.x = frameManagerAlienGauche.x;
             counterFrame.y = frameManagerAlienGauche.y;
             counterFrame.nbLigne = 0;
        }
        frameManager = &frameManagerAlienGauche;
        break;
    default :
        break;
    }
}

    calculNextFrame();
    return false;

}
void EnnemiGhost::deplacerEnnemi() // Calcul direction
{
       if(!enDeplacementVersCase)
       {

           vector<Direction> listeDirection;
           vector<Case*> listeCase;
           vector<Direction> listeDirectionInverse;

           if(spotted)
           {
               if(!collision())
               {
                   switch(directionSpotted)
                    {
                       case HAUT:
                           if(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x)!= nullptr)
                           {
                               if(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x)->getType() == FOND)
                               {
                                    listeDirectionInverse.push_back(BAS);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x));
                                    listeDirection.push_back(directionSpotted);
                               }
                               else
                               {
                                   spotted = false;
                                   vitesse = vitesseBase;
                                   frameAtkActif = false;
                                   tempsDerniereAtk = getTime();
                               }


                           }
                           else
                           {
                                vitesse = vitesseBase;
                                spotted = false;
                                frameAtkActif = false;
                                tempsDerniereAtk = getTime();
                           }


                        break;
                       case BAS:
                           if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x)!= nullptr)
                           {
                               if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x)->getType() == FOND)
                               {
                                    listeDirectionInverse.push_back(HAUT);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x));
                                    listeDirection.push_back(directionSpotted);
                               }
                               else
                               {
                                   vitesse = vitesseBase;
                                   spotted = false;
                                   frameAtkActif = false;
                                   tempsDerniereAtk = getTime();
                               }

                           }
                           else
                           {
                               vitesse = vitesseBase;
                               spotted = false;
                               frameAtkActif = false;
                               tempsDerniereAtk = getTime();
                           }

                        break;
                       case DROITE:
                           if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1)!= nullptr)
                           {
                               if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1)->getType() == FOND)
                               {
                                    listeDirectionInverse.push_back(GAUCHE);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1));
                                    listeDirection.push_back(directionSpotted);
                               }
                               else
                               {
                                   spotted = false;
                                   vitesse = vitesseBase;
                                   frameAtkActif = false;
                                   tempsDerniereAtk = getTime();
                               }
                           }
                           else
                           {
                                 spotted = false;
                                 vitesse = vitesseBase;
                                 frameAtkActif = false;
                                 tempsDerniereAtk = getTime();
                           }

                        break;
                       case GAUCHE:
                           if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1)!= nullptr)
                           {
                               if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1)->getType() == FOND)
                               {
                                    listeDirectionInverse.push_back(DROITE);
                                    listeCase.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1));
                                    listeDirection.push_back(directionSpotted);
                               }
                               else
                               {
                                   spotted = false;
                                   vitesse = vitesseBase;
                                   frameAtkActif = false;
                                   tempsDerniereAtk = getTime();
                               }
                           }
                           else
                           {
                               spotted = false;
                               vitesse = vitesseBase;
                               frameAtkActif = false;
                               tempsDerniereAtk = getTime();
                           }

                        break;
                       default:
                        break;
                       }

                   }
                   else
                   {
                       spotted = false;
                       vitesse = vitesseBase;
                       frameAtkActif = false;
                       tempsDerniereAtk = getTime();
                   }
               }


           if(!spotted)
           {
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
