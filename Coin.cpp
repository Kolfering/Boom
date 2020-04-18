#include "Coin.h"
#include "Utile.h"
#include "MenuJeu.h"

SDL_Texture* Coin::image = nullptr;
Mix_Chunk* Coin::son = nullptr;
Coin::Coin(Case* caseCoin)
{
    this->caseCoin = caseCoin;
    caseCoin->setCoin(this);
    score = 150;
    pickUp = false;
    toRemove = false;
    position.x = caseCoin->getPositionPixel().x;
    position.y = caseCoin->getPositionPixel().y;
    position.h = caseCoin->getPositionPixel().h;
    position.w = caseCoin->getPositionPixel().w;
    frameManagerCoin = {0,0,1,10,200,0};
    frameManagerCoinPickUp = {0,0,10,10,10,0};
    frameManager = &frameManagerCoin;
    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;

    antiPosition.y = 0;
    antiPosition.w = CASE_DIMENSION;
    antiPosition.h = CASE_DIMENSION;

}

SDL_Texture* Coin::getImage()
{
    return image;
}
bool Coin::isToRemove()
{
    return toRemove;
}

void Coin::calculNextFrame()
{
    if(getTime() - frameManager->tempsBefore > frameManager->tempsFrame)
    {
        counterFrame.x++;
        frameManager->tempsBefore = getTime();
        if(counterFrame.x == frameManager->longueur)
        {
            counterFrame.x = 0;
            counterFrame.y++;
            counterFrame.nbLigne++;
        }
        if(counterFrame.x + (frameManager->longueur*counterFrame.nbLigne) == (frameManager->x + frameManager->nbFrame))
        {
            counterFrame.x = frameManager->x;
            counterFrame.y = frameManager->y;
            counterFrame.nbLigne = 0;
        }
    }

    if(pickUp)
    {
        if(getTime() - tempsPickUp > TEMPS_COIN)
        {
            toRemove = true;
        }
    }

}

void Coin::execute()
{
    calculNextFrame();

}

SDL_Rect* Coin::getAdresseAntiPosition()
{
    antiPosition.x = counterFrame.x * CASE_DIMENSION;
    return &antiPosition;
}

SDL_Rect* Coin::getAdressePosition()
{
    return &position;
}

bool Coin::isPickUp()
{
    return pickUp;
}

void Coin::pickedUp(Bomber* bomber)
{
    Mix_PlayChannel(-1,son,0);
    MenuJeu::ajouterScore(score,position.x,position.y,bomber);
    pickUp = true;
    tempsPickUp = getTime();
    frameManager = &frameManagerCoinPickUp;
    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
}

Case* Coin::getCase()
{
    return caseCoin;
}

void Coin::setCase(Case* c)
{
    caseCoin = c;
}

Coin::~Coin()
{

}

void Coin::initialiserCoin()
{
   image = chargerSurface("Images/Coin.png");
   son = Mix_LoadWAV("Sons/Coin.wav");
}

void Coin::releaseCoin()
{
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
    if(son)
        Mix_FreeChunk(son);
    son = nullptr;
}
