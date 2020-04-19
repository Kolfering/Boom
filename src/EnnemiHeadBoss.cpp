#include "EnnemiHeadBoss.h"
#include "Utile.h"
#include "Carte.h"
#include "MenuJeu.h"
#include "ProjectileHeadMissile.h"
#include "Carte.h"
#include "Utile.h"
#include <algorithm>

Mix_Chunk* EnnemiHeadBoss::sonAtk;
Mix_Chunk* EnnemiHeadBoss::sonMort;
Mix_Chunk* EnnemiHeadBoss::sonHurt;
SDL_Texture* EnnemiHeadBoss::image;
SDL_Texture* EnnemiHeadBoss::imageHurt;
EnnemiHeadBoss::EnnemiHeadBoss(Case* c) : Ennemi(c)
{
    alienable = false;
    idEnnemi = IDEnnemi::HEADBOSS;
    score = 5000;
    degat = 16;
    taille = CASE_DIMENSION*3;
    intervalAtk = 3500;
    vitesse = 0;
    tempsFrameAtk = 250;
    timeExplosion = 0;
    vie = 20;
    timeBlink = 0;
    nbFire = HEADBOSS_NBFIRE;
    timeFire = getTime();
    timeMiniFire = 0;
    range = 13;
    hit = false;
    frameAtk = false;


    for(int i=0;i<3;i++)
    {
        for(int j = 0;j<3;j++)
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

EnnemiHeadBoss::~EnnemiHeadBoss()
{
    for(Case* ca : listeCase)
        ca->setEnnemi(nullptr);
}

void EnnemiHeadBoss::initialiserEnnemiHeadBoss()
{
    image = chargerSurface("../Images/HeadBoss.png");
    sonAtk = Mix_LoadWAV("../Sons/HeadShot.wav");
    sonMort = Mix_LoadWAV("../Sons/HeadDeath.wav");
    sonHurt = Mix_LoadWAV("../Sons/HeadNoise.wav");

    SDL_Surface* imageHurtTmp = IMG_Load("../Images/HeadBoss.png");

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

Mix_Chunk* EnnemiHeadBoss::getSonRandom()
{
    return nullptr;
}

void EnnemiHeadBoss::prendreDegat(Bomber* bomber,unsigned int ID)
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

            }
            else
            {
                timeHit = getTime();
                hit = true;

                if(nbFire>0 && nbFire<HEADBOSS_NBFIRE)
                {
                    nbFire = HEADBOSS_NBFIRE;
                    timeFire = getTime();
                }

                Mix_PlayChannel(-1,sonHurt,0);
            }

        }
    }

    }

}

void EnnemiHeadBoss::collisionBomber()
{
    for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
    {
        for(unsigned int j = 0;j<listeCase.size();j++)
        {
            if(listeCase.at(j)->equals(MenuJeu::getListeBomber().at(i)->getCaseToGo()) ||  listeCase.at(j)->equals(MenuJeu::getListeBomber().at(i)->getCase()))
            {
                MenuJeu::getListeBomber().at(i)->prendreDegat(degat);
                break;
            }
        }
    }
}

void EnnemiHeadBoss::releaseEnnemiHeadBoss()
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
    if(sonHurt)
        Mix_FreeChunk(sonHurt);
    sonHurt = nullptr;
}

Mix_Chunk* EnnemiHeadBoss::getSonMort()
{
    return sonMort;
}

SDL_Texture* EnnemiHeadBoss::getImage()
{
    if(hit)
        return imageHurt;
    return image;
}


SDL_Rect* EnnemiHeadBoss::getAdresseAntiPosition()
{
    return nullptr;
}

bool EnnemiHeadBoss::attaque()
{
    if(getTime() - timeFire > intervalAtk)
    {
        nbFire = 0;
        timeFire = getTime();
    }

    if(nbFire < HEADBOSS_NBFIRE)
    {
        if(getTime() - timeMiniFire > HEADBOSS_MINIFIRE)
        {
            nbFire++;
            timeMiniFire = getTime();
            unsigned int closest1 = 100;
            unsigned int closest2 = 100;
            unsigned int v1,v2;
            SDL_Rect p1,p2;
            for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
            {
                if(!MenuJeu::getListeBomber().at(i)->isGameEnded())
                {
                   v1 = std::abs(casePersonnage->getPositionDamier().x+1 - MenuJeu::getListeBomber().at(i)->getCase()->getPositionDamier().x) + std::abs(casePersonnage->getPositionDamier().y+1 - MenuJeu::getListeBomber().at(i)->getCase()->getPositionDamier().y);
                   if (v1 < closest1 && v1 < range)
                   {
                       closest1 = v1;
                       p1 = MenuJeu::getListeBomber().at(i)->getPosition();
                   }

                   v2 = std::abs(casePersonnage->getPositionDamier().x+2 - MenuJeu::getListeBomber().at(i)->getCase()->getPositionDamier().x) + std::abs(casePersonnage->getPositionDamier().y+1 - MenuJeu::getListeBomber().at(i)->getCase()->getPositionDamier().y);
                   if (v2 < closest2 && v2 < range)
                   {
                       closest2 = v2;
                       p2 = MenuJeu::getListeBomber().at(i)->getPosition();
                   }
                }

            }
            if(closest1 < range)
            {
                Position pos1 = {p1.x*1.0f + CASE_DIMENSION/2,p1.y*1.0f + CASE_DIMENSION/2};
                Position pp1 = {Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x+1)->getPositionPixel().x*1.0f,Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x+1)->getPositionPixel().y*1.0f};
                Carte::ajouterProjectile(new ProjectileHeadMissile(this,nullptr,pp1,pos1));
                Mix_PlayChannel(-1,sonAtk,0);
            }
            if(closest2 < range)
            {
                Position pos2 = {p2.x*1.0f + CASE_DIMENSION/2,p2.y*1.0f + CASE_DIMENSION/2};
                Position pp2 = {Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x+2)->getPositionPixel().x*1.0f,Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x+2)->getPositionPixel().y*1.0f};
                Carte::ajouterProjectile(new ProjectileHeadMissile(this,nullptr,pp2,pos2));
                Mix_PlayChannel(-1,sonAtk,0);
            }

        }
    }

    return true;
}

void EnnemiHeadBoss::deplacerEnnemi()
{
    if(meurt)
    {
        if(getTime() - timeMort > HEADBOSS_DEATH)
        {
            mort = true;
            for(Bomber* b : MenuJeu::getListeBomber())
                MenuJeu::ajouterScore(score,Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x+1)->getPositionPixel().x-5,Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x+1)->getPositionPixel().y,b);
        }
        else
        {
            if(getTime() - timeBlink > HEADBOSS_BLINK)
            {
                apparaitre = !apparaitre;
                timeBlink = getTime();
            }
            if(getTime() - timeExplosion > HEADBOSS_EXPLOSION)
            {
                Position pos;
                int w,h;
                SDL_QueryTexture(image,nullptr,nullptr,&w,&h);
                pos.x = position.x + genererNombreAleatoire(w);
                pos.y = position.y + genererNombreAleatoire(h);
                Carte::ajouterProjectile(new ProjectileHeadMissile(pos));
                timeExplosion = getTime();
            }
        }
    }
    else
    {
        collisionBomber();
        if(hit)
        {
            if(getTime() - timeHit > HEADBOSS_BLANK)
            {
                hit = false;
            }
        }
        else
        {
            attaque();
        }
    }

}
