#include "EnnemiSoldier.h"
#include "EnnemiHumain.h"
#include "Utile.h"


Mix_Chunk* EnnemiSoldier::sonAtk;
Mix_Chunk* EnnemiSoldier::sonMort;
Mix_Chunk* EnnemiSoldier::sonRandom;
SDL_Texture* EnnemiSoldier::image;
EnnemiSoldier::EnnemiSoldier(Case* c) : EnnemiHumain(c)
{
    idEnnemi = SOLDIER;
    score = 200;
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


}

SDL_Texture* EnnemiSoldier::getImage()
{
     if(!alienMode)
        return image;
    else
        return imageAlien;
}

Mix_Chunk* EnnemiSoldier::getSonRandom()
{
    return sonRandom;
}

void EnnemiSoldier::initialiserEnnemiSoldier()
{
    image = chargerSurface("Images/soldier.png");
    sonMort = Mix_LoadWAV("Sons/SoldierDeath.wav");
    sonRandom = Mix_LoadWAV("Sons/SoldierNoise.wav");
    sonAtk = Mix_LoadWAV("Sons/Shot.wav");
}

void EnnemiSoldier::releaseEnnemiSoldier()
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

Mix_Chunk* EnnemiSoldier::getSonAtk()
{
    return sonAtk;
}

Mix_Chunk* EnnemiSoldier::getSonMort()
{
    return sonMort;
}

EnnemiSoldier::~EnnemiSoldier()
{

}

