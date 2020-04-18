#include "EnnemiAlienBoss.h"
#include "Utile.h"
#include "Carte.h"
#include "MenuJeu.h"
#include "ProjectileHeadMissile.h"
#include "ProjectileEgg.h"
#include <vector>
#include <algorithm>

using std::vector;
Mix_Chunk* EnnemiAlienBoss::sonAtk;
Mix_Chunk* EnnemiAlienBoss::sonMort;
Mix_Chunk* EnnemiAlienBoss::sonRandom;
SDL_Texture* EnnemiAlienBoss::image;
SDL_Texture* EnnemiAlienBoss::imageHurt;
EnnemiAlienBoss::EnnemiAlienBoss(Case* c) : Ennemi(c)
{
    alienable = false;
    idEnnemi = ALIENBOSS;
    score = 100000;
    frameManagerBas = {0,0,4,4,100,0,1};
    frameManagerHaut = {0,1,4,4,100,0,1};
    frameManagerDroite = {0,2,4,4,100,0,1};
    frameManagerGauche = {0,3,4,4,100,0,1};
    vie = ALIENBOSS_VIEMAX;
    degat = 16;
    taille = CASE_DIMENSION*5;
    frameManager = &frameManagerBas;
    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
    intervalAtk = 4500;
    vitesse = 1.2;
    tempsFrameAtk = 500;
    timeBlink = 0;
    timeExplosion = 0;
    timeHit = 0;
    hit = false;
    freeDirection = false;
    frameAtk = false;
    standWhileAtk = true;

    for(int i=0;i<5;i++)
    {
        for(int j = 0;j<5;j++)
            listeCase.push_back(Carte::getCase(c->getPositionDamier().y+i,c->getPositionDamier().x+j));
    }

    for(Case* ca : listeCase)
        ca->setEnnemi(this);

    position.x = (Carte::getCase(c->getPositionDamier().y,c->getPositionDamier().x))->getPositionPixel().x;
    position.y = (Carte::getCase(c->getPositionDamier().y,c->getPositionDamier().x))->getPositionPixel().y;
    position.w = taille;
    position.h = taille;
    positionReal.x = position.x;
    positionReal.y = position.y;
}

void EnnemiAlienBoss::initialiserEnnemiAlienBoss()
{
    image = chargerSurface("Images/AlienBoss.png");
    sonAtk = Mix_LoadWAV("Sons/Egg.wav");
    sonMort = Mix_LoadWAV("Sons/BossDeath.wav");
    sonRandom = Mix_LoadWAV("Sons/BossNoise.wav");

    SDL_Surface* imageHurtTmp = IMG_Load("Images/AlienBoss.png");

    Uint32 pixel;
    Uint8 r,g,b,a;
    SDL_LockSurface(imageHurtTmp);
    for(int i = 0;i<imageHurtTmp->w;i++)
    {
        for(int j = 0;j<imageHurtTmp->h;j++)
        {
            pixel = obtenirPixel(imageHurtTmp,i,j);
            SDL_GetRGBA(pixel, imageHurtTmp->format, &r, &g, &b, &a);

            if(a==255)
            {
                r = 255;
                g = 255;
                b = 255;
                pixel = SDL_MapRGBA(imageHurtTmp->format,r,g,b,a);
                definirPixel(imageHurtTmp,i,j,pixel);
            }

        }
    }
    SDL_UnlockSurface(imageHurtTmp);
    imageHurt = SDL_CreateTextureFromSurface(Jeu::getRenderer(),imageHurtTmp);
    SDL_FreeSurface(imageHurtTmp);
}

Mix_Chunk* EnnemiAlienBoss::getSonRandom()
{
    return nullptr;
}


std::vector<Case*> EnnemiAlienBoss::getCaseOnList()
{
    return listeCase;
}

std::vector<Case*> EnnemiAlienBoss::getCaseToGoOnList()
{
    std::vector<Case*> vec;
    switch(direction)
    {
    case HAUT:
         for(unsigned int i = 0;i<5;i++)
            vec.push_back(Carte::getCase(caseToGo->getPositionDamier().y,caseToGo->getPositionDamier().x+i));
        break;
    case BAS:
        for(unsigned int i = 0;i<5;i++)
            vec.push_back(Carte::getCase(caseToGo->getPositionDamier().y+4,caseToGo->getPositionDamier().x+i));
        break;
    case DROITE:
         for(unsigned int i = 0;i<5;i++)
            vec.push_back(Carte::getCase(caseToGo->getPositionDamier().y+i,caseToGo->getPositionDamier().x+4));
        break;
    case GAUCHE:
        for(unsigned int i = 0;i<5;i++)
            vec.push_back(Carte::getCase(caseToGo->getPositionDamier().y+i,caseToGo->getPositionDamier().x));
        break;
    default:
        break;
    }
    return vec;
}

std::vector<Case*> EnnemiAlienBoss::getCasePrecedenteOnList()
{
    std::vector<Case*> vec;
    switch(direction)
    {
    case HAUT:
    case BAS:
         for(unsigned int i = 0;i<5;i++)
            vec.push_back(Carte::getCase(casePrecedente->getPositionDamier().y,casePrecedente->getPositionDamier().x+i));
        break;
    case DROITE:
    case GAUCHE:
         for(unsigned int i = 0;i<5;i++)
            vec.push_back(Carte::getCase(caseToGo->getPositionDamier().y+i,caseToGo->getPositionDamier().x));
        break;
    default:
        break;
    }
    return vec;
}


void EnnemiAlienBoss::releaseEnnemiAlienBoss()
{
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
    if(imageHurt)
        SDL_DestroyTexture(imageHurt);
    imageHurt = nullptr;
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

Mix_Chunk* EnnemiAlienBoss::getSonMort()
{
    return sonMort;
}

SDL_Texture* EnnemiAlienBoss::getImage()
{
    if(hit)
        return imageHurt;
    return image;
}

EnnemiAlienBoss::~EnnemiAlienBoss()
{
    for(Case* ca : listeCase)
        ca->setEnnemi(nullptr);
}

void EnnemiAlienBoss::accelerer()
{
    vitesse *=2;
    if(vitesse>2)
        vitesse = 2;
}

bool EnnemiAlienBoss::attaque()
{
   /* if(Carte::getListeEnnemi().size()<30)
    {*/
        Case* caseCollision = nullptr;
        switch(direction)
        {
        case HAUT:
            for(int i = casePersonnage->getPositionDamier().y;i<=CASE_HAUTEUR;i++)
            {
                if(Carte::getCase(i,casePersonnage->getPositionDamier().x)->getType() != FOND)
                {
                    caseCollision = Carte::getCase(i,casePersonnage->getPositionDamier().x);
                    break;
                }
            }
            tempsDerniereAtk = getTime();
            Mix_PlayChannel(-1,sonAtk,0);
            freeDirection = true;
            Carte::ajouterProjectile(new ProjectileEgg(this,caseCollision,{positionReal.x + CASE_DIMENSION*2 +CASE_DIMENSION/2 ,positionReal.y + taille + CASE_DIMENSION/5},directionInverse));
            return true;
        case BAS:
            for(int i = casePersonnage->getPositionDamier().y;i>=1;i--)
            {
                if(Carte::getCase(i,casePersonnage->getPositionDamier().x)->getType() != FOND)
                {
                    caseCollision = Carte::getCase(i,casePersonnage->getPositionDamier().x);
                    break;
                }
            }
            tempsDerniereAtk = getTime();
            Mix_PlayChannel(-1,sonAtk,0);
            freeDirection = true;
            Carte::ajouterProjectile(new ProjectileEgg(this,caseCollision,{positionReal.x + CASE_DIMENSION*2 + CASE_DIMENSION/2,positionReal.y - CASE_DIMENSION/5},directionInverse));
            return true;
        case DROITE:
            for(int i = casePersonnage->getPositionDamier().x;i>=1;i--)
             {
                if(Carte::getCase(casePersonnage->getPositionDamier().y,i)->getType() != FOND)
                {
                    caseCollision = Carte::getCase(casePersonnage->getPositionDamier().y,i);
                    break;
                }
             }
            tempsDerniereAtk = getTime();
            Mix_PlayChannel(-1,sonAtk,0);
            freeDirection = true;
            Carte::ajouterProjectile(new ProjectileEgg(this,caseCollision,{positionReal.x - CASE_DIMENSION/5 ,positionReal.y + CASE_DIMENSION*2 + CASE_DIMENSION/2},directionInverse));
            return true;
        case GAUCHE:
            for(int i = casePersonnage->getPositionDamier().x;i<=CASE_LONGUEUR;i++)
             {
                if(Carte::getCase(casePersonnage->getPositionDamier().y,i)->getType() != FOND)
                {
                    caseCollision = Carte::getCase(casePersonnage->getPositionDamier().y,i);
                    break;
                }
            }
            tempsDerniereAtk = getTime();
            Mix_PlayChannel(-1,sonAtk,0);
            freeDirection = true;
            Carte::ajouterProjectile(new ProjectileEgg(this,caseCollision,{positionReal.x + taille + CASE_DIMENSION/5,positionReal.y + CASE_DIMENSION*2 + CASE_DIMENSION/2},directionInverse));
            return true;
        default:
            return false;
        }
   // }
}

void EnnemiAlienBoss::prendreDegat(Bomber* bomber,unsigned int ID)
{
  if(ID > 0)
  {
    if(!(std::find(listeExplosion.begin(),listeExplosion.end(),ID) != listeExplosion.end()) || listeExplosion.empty())
    {
        if(!meurt)
        {
            vie -= 1;
            listeExplosion.push_back(ID);
            if(vie == 0)
            {
                Mix_PlayChannel(-1,sonMort,0);
                timeMort = getTime();
                meurt = true;
                hit = false;


                for(unsigned int i = 0;i<Carte::getListeEnnemi().size();i++)
                    Carte::getListeEnnemi().at(i)->prendreDegat(bomber,0);

            }
            else
            {
                timeHit = getTime();
                tempsDerniereAtk = getTime();
                hit = true;
                freeDirection = true;
                Mix_PlayChannel(-1,sonRandom,0);
            }

        }
    }

    }

}

bool EnnemiAlienBoss::collision()
{
    for(unsigned int i = 0;i<Carte::getListeEnnemi().size();i++)
    {
        if(!Carte::getListeEnnemi().at(i)->getCase()->equals(casePersonnage))
        {
            if(checkCase(Carte::getListeEnnemi().at(i)->getCaseToGo(),getCaseToGoOnList()) || checkCase(Carte::getListeEnnemi().at(i)->getCase(),getCaseToGoOnList()) || checkCase(Carte::getListeEnnemi().at(i)->getCasePrecedente(),getCaseToGoOnList()))
            {
                switch(direction)
                {
                case HAUT:
                    if(getPosition().y <= Carte::getListeEnnemi().at(i)->getPosition().y + Carte::getListeEnnemi().at(i)->getTaille())
                        return true;

                    break;
                case BAS:
                    if(getPosition().y + taille  >= Carte::getListeEnnemi().at(i)->getPosition().y)
                       return true;

                    break;
                case DROITE:

                    if(getPosition().x + taille  >= Carte::getListeEnnemi().at(i)->getPosition().x)
                        return true;


                    break;
                case GAUCHE:
                    if(getPosition().x <= Carte::getListeEnnemi().at(i)->getPosition().x + Carte::getListeEnnemi().at(i)->getTaille())
                        return true;

                    break;

                default:
                    break;
                }
            }

            else if(caseToGo->getEnnemi())
            {
                if(caseToGo->getEnnemi()->getIDEnnemi() == HEADBOSS)
                      return true;
            }



        }

    }

    return false;
}

void EnnemiAlienBoss::setCase(Case* casePersonnage)
{
    Case* c = nullptr;
    Case* cToRemove = nullptr;
    bool moveIn = true;
    switch(direction)
    {
    case HAUT:
      c = Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x);
      for(unsigned int i = 0;i<listeCase.size();i++)
      {
            if(listeCase.at(i)->equals(c))
            {
                moveIn = false;
                break;
            }

      }
      if(moveIn)
      {
        for(int i=0;i<5;i++)
        {
            c = Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+i);
            cToRemove = Carte::getCase(casePersonnage->getPositionDamier().y+5,casePersonnage->getPositionDamier().x+i);
            c->setEnnemi(this);
            cToRemove->setEnnemi(nullptr);
            for(unsigned int i = 0;i<listeCase.size();i++)
            {
                if(listeCase.at(i)->equals(cToRemove))
                {
                    listeCase.erase(listeCase.begin() + i);
                    break;
                }

            }
            listeCase.push_back(c);


        }
      }
        break;
    case BAS:
      c = Carte::getCase(casePersonnage->getPositionDamier().y+4,casePersonnage->getPositionDamier().x);
      for(unsigned int i = 0;i<listeCase.size();i++)
      {
            if(listeCase.at(i)->equals(c))
            {
                moveIn = false;
                break;
            }

      }
      if(moveIn)
      {
        for(int i=0;i<5;i++)
        {
            c = Carte::getCase(casePersonnage->getPositionDamier().y+4,casePersonnage->getPositionDamier().x+i);
            cToRemove = Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x+i);
            c->setEnnemi(this);
            cToRemove->setEnnemi(nullptr);
            for(unsigned int i = 0;i<listeCase.size();i++)
            {
                if(listeCase.at(i)->equals(cToRemove))
                {
                    listeCase.erase(listeCase.begin() + i);
                    break;
                }

            }
            listeCase.push_back(c);

        }
      }
        break;
    case DROITE:
      c = Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+4);
      for(unsigned int i = 0;i<listeCase.size();i++)
      {
            if(listeCase.at(i)->equals(c))
            {
                moveIn = false;
                break;
            }

      }
      if(moveIn)
      {
        for(int i=0;i<5;i++)
        {
            c = Carte::getCase(casePersonnage->getPositionDamier().y+i,casePersonnage->getPositionDamier().x+4);
            cToRemove = Carte::getCase(casePersonnage->getPositionDamier().y+i,casePersonnage->getPositionDamier().x-1);
            c->setEnnemi(this);
            cToRemove->setEnnemi(nullptr);
            for(unsigned int i = 0;i<listeCase.size();i++)
            {
                if(listeCase.at(i)->equals(cToRemove))
                {
                    listeCase.erase(listeCase.begin() + i);
                    break;
                }

            }
            listeCase.push_back(c);

        }
      }
        break;
    case GAUCHE:
    c = Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x);
      for(unsigned int i = 0;i<listeCase.size();i++)
      {
            if(listeCase.at(i)->equals(c))
            {
                moveIn = false;
                break;
            }

      }
      if(moveIn)
      {
        for(int i=0;i<5;i++)
        {
            c = Carte::getCase(casePersonnage->getPositionDamier().y+i,casePersonnage->getPositionDamier().x);
            cToRemove = Carte::getCase(casePersonnage->getPositionDamier().y+i,casePersonnage->getPositionDamier().x+5);
            c->setEnnemi(this);
            cToRemove->setEnnemi(nullptr);
            for(unsigned int i = 0;i<listeCase.size();i++)
            {
                if(listeCase.at(i)->equals(cToRemove))
                {
                    listeCase.erase(listeCase.begin() + i);
                    break;
                }

            }
            listeCase.push_back(c);

        }
      }
        break;
    default:
        break;
    }

    this->casePersonnage = casePersonnage;

}

void EnnemiAlienBoss::deplacerEnnemi() // Calcul direction
{
    if(meurt)
    {
        if(getTime() - timeMort > ALIENBOSS_DEATH)
        {
            mort = true;
            for(Bomber* b : MenuJeu::getListeBomber())
                MenuJeu::ajouterScore(score,position.x + 43,position.y + 66,b);
        }
        else
        {
            if(getTime() - timeBlink > ALIENBOSS_BLINK)
            {
                apparaitre = !apparaitre;
                timeBlink = getTime();
            }
            if(getTime() - timeExplosion > ALIENBOSS_EXPLOSION)
            {
                Position pos;
                pos.x = position.x + genererNombreAleatoire(CASE_DIMENSION*5);
                pos.y = position.y + genererNombreAleatoire(CASE_DIMENSION*5);
                Carte::ajouterProjectile(new ProjectileHeadMissile(pos));
                timeExplosion = getTime();
            }
        }
    }

    else
    {
        if(hit)
        {
            if(getTime() - timeHit > ALIENBOSS_BLANK)
            {
                hit = false;
            }
            else
                return;
        }


       if(!enDeplacementVersCase && !frameAtkActif)
       {

           vector<Direction> listeDirection;
           vector<Case*> listeCaseDirection;
           vector<Direction> listeDirectionInverse;

           if(!freeDirection && calculCleanArea(direction))
           {
               listeDirection.push_back(direction);
               listeDirectionInverse.push_back(directionInverse);

               switch(direction)
               {
               case HAUT:
                   listeCaseDirection.push_back(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x));
                break;
               case BAS:
                   listeCaseDirection.push_back(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x));
                break;
               case DROITE:
                   listeCaseDirection.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1));
                break;
               case GAUCHE:
                   listeCaseDirection.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1));
                break;
               default:
                break;
               }

           }
           else
           {
               if((direction != HAUT || freeDirection) && calculCleanArea(HAUT))
               {
                   listeDirection.push_back(HAUT);
                   listeDirectionInverse.push_back(BAS);
                   listeCaseDirection.push_back(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x));
               }
               if((direction != BAS || freeDirection) && calculCleanArea(BAS))
               {
                   listeDirection.push_back(BAS);
                   listeDirectionInverse.push_back(HAUT);
                   listeCaseDirection.push_back(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x));
               }
               if((direction != DROITE || freeDirection) && calculCleanArea(DROITE))
               {
                   listeDirection.push_back(DROITE);
                   listeDirectionInverse.push_back(GAUCHE);
                   listeCaseDirection.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1));
               }
               if((direction != GAUCHE || freeDirection) && calculCleanArea(GAUCHE))
               {
                   listeDirection.push_back(GAUCHE);
                   listeDirectionInverse.push_back(DROITE);
                   listeCaseDirection.push_back(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1));
               }
           }

           freeDirection = false;

           if(!listeDirection.empty())
           {
               unsigned int nombre = genererNombreAleatoire(listeDirection.size())-1;
               direction = listeDirection.at(nombre);
               caseToGo = listeCaseDirection.at(nombre);
               casePrecedente = caseToGo;
               directionInverse = listeDirectionInverse.at(nombre);
               listeCaseDirection.clear();
               listeDirection.clear();
               listeDirectionInverse.clear();
           }
           else
                faitDemiTour();

       }
        enDeplacementVersCase = !goToCase();
    }
}

void EnnemiAlienBoss::collisionBomber()
{
    for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
    {

       if(!((getPosition().x >= MenuJeu::getListeBomber().at(i)->getPosition().x + MenuJeu::getListeBomber().at(i)->getTaille())
            || (getPosition().x + taille <= MenuJeu::getListeBomber().at(i)->getPosition().x)
            || (getPosition().y >= MenuJeu::getListeBomber().at(i)->getPosition().y + MenuJeu::getListeBomber().at(i)->getTaille())
            || (getPosition().y + taille <=  MenuJeu::getListeBomber().at(i)->getPosition().y)))
        {
            MenuJeu::getListeBomber().at(i)->prendreDegat(degat);
            break;
        }
    }


}

bool EnnemiAlienBoss::calculCleanArea(Direction directionToCheck)
{
    switch(directionToCheck)
    {
    case HAUT:
          for(int i = 0;i<5;i++)
          {
                if(!Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x+i))
                    return false;
                else if(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x+i)->getType() != FOND)
                    return false;


          }
        return true;
    case BAS:
        for(int i = 0;i<5;i++)
        {
            if(!Carte::getCase(casePersonnage->getPositionDamier().y+5,casePersonnage->getPositionDamier().x+i))
                return false;
            else if(Carte::getCase(casePersonnage->getPositionDamier().y+5,casePersonnage->getPositionDamier().x+i)->getType() != FOND)
                return false;

       }
        return true;
    case DROITE:
        for(int i = 0;i<5;i++)
        {
            if(!Carte::getCase(casePersonnage->getPositionDamier().y+i,casePersonnage->getPositionDamier().x+5))
               return false;
            else if(Carte::getCase(casePersonnage->getPositionDamier().y+i,casePersonnage->getPositionDamier().x+5)->getType() != FOND)
                return false;

        }
        return true;
    case GAUCHE:
         for(int i = 0;i<5;i++)
         {
            if(!Carte::getCase(casePersonnage->getPositionDamier().y+i,casePersonnage->getPositionDamier().x-1))
                return false;
            else if(Carte::getCase(casePersonnage->getPositionDamier().y+i,casePersonnage->getPositionDamier().x-1)->getType() != FOND)
                return false;

         }
         return true;

    default:
        return false;

    }
}
