#include "EnnemiSarge.h"
#include "EnnemiHumain.h"
#include "Utile.h"


Mix_Chunk* EnnemiSarge::sonAtk;
Mix_Chunk* EnnemiSarge::sonMort;
Mix_Chunk* EnnemiSarge::sonRandom;
SDL_Texture* EnnemiSarge::image;

EnnemiSarge::EnnemiSarge(Case* c) : EnnemiHumain(c)
{
    idEnnemi = SARGE;
    score = 300;
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

    intervalAtk = 1000;


}

SDL_Texture* EnnemiSarge::getImage()
{
     if(!alienMode)
        return image;
    else
        return imageAlien;
}

void EnnemiSarge::initialiserEnnemiSarge()
{
    image = chargerSurface("Images/Sarge.png");
    sonMort = Mix_LoadWAV("Sons/SargeDeath2.wav");
    sonRandom = Mix_LoadWAV("Sons/SargeNoise.wav");
    sonAtk = Mix_LoadWAV("Sons/Shot.wav");
}

Mix_Chunk* EnnemiSarge::getSonMort()
{
    return sonMort;
}


Mix_Chunk* EnnemiSarge::getSonRandom()
{
    return sonRandom;
}

Mix_Chunk* EnnemiSarge::getSonAtk()
{
    return sonAtk;
}

void EnnemiSarge::releaseEnnemiSarge()
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

EnnemiSarge::~EnnemiSarge()
{

}
