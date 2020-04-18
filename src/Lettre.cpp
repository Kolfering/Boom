#include "Lettre.h"
#include "Utile.h"

SDL_Texture* Lettre::image = nullptr;
unsigned int Lettre::score = 100;
Lettre::Lettre(SDL_Rect pos)
{
    frameManagerE = {0,0,1,10,2500,0};
    frameManagerEX = {1,0,3,10,50,0};
    frameManagerX = {4,0,1,10,2500,0};
    frameManagerXT = {5,0,3,10,50,0};
    frameManagerT = {8,0,1,10,2500,0};
    frameManagerTR = {9,0,3,10,50,0};
    frameManagerR = {2,1,1,10,2500,0};
    frameManagerRA = {3,1,3,10,50,0};
    frameManagerA = {6,1,1,10,2500,0};
    frameManagerAE = {7,1,3,10,50,0};
    indice = genererNombreAleatoire(5) - 1;
    position = {pos.x,pos.y,CASE_DIMENSION,CASE_DIMENSION};

    switch(indice)
    {
    case 0:
        frameManager = &frameManagerE;
        break;
    case 1:
        frameManager = &frameManagerX;
        break;
    case 2:
        frameManager = &frameManagerT;
        break;
    case 3:
        frameManager = &frameManagerR;
        break;
    case 4:
        frameManager = &frameManagerA;
        break;
    default:
        frameManager = &frameManagerE;
        break;
    }

    frameManager->tempsBefore = getTime();
    counterFrame.x = frameManager->x;
    counterFrame.y = frameManager->y;
    counterFrame.nbLigne = 0;
}

unsigned int Lettre::getScore()
{
    return score;
}
unsigned int Lettre::getIndice()
{
    return indice;
}
void Lettre::initialiserLettre()
{
    image = chargerSurface("Images/Letters.png");
}

void Lettre::releaseLettre()
{
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}

SDL_Texture* Lettre::getImage()
{
    return image;
}

void Lettre::calculNextFrame()
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
            if(compareFrameManager(*frameManager,frameManagerE))
                frameManager = &frameManagerEX;
            else if(compareFrameManager(*frameManager,frameManagerEX))
            {
                frameManager = &frameManagerX;
                indice = 1;
            }
            else if(compareFrameManager(*frameManager,frameManagerX))
                frameManager = &frameManagerXT;
            else if(compareFrameManager(*frameManager,frameManagerXT))
            {
                frameManager = &frameManagerT;
                indice = 2;
            }
            else if(compareFrameManager(*frameManager,frameManagerT))
                frameManager = &frameManagerTR;
            else if(compareFrameManager(*frameManager,frameManagerTR))
            {
                frameManager = &frameManagerR;
                indice = 3;
            }
            else if(compareFrameManager(*frameManager,frameManagerR))
                frameManager = &frameManagerRA;
            else if(compareFrameManager(*frameManager,frameManagerRA))
            {
                 frameManager = &frameManagerA;
                 indice = 4;
            }
            else if(compareFrameManager(*frameManager,frameManagerA))
                frameManager = &frameManagerAE;
            else if(compareFrameManager(*frameManager,frameManagerAE))
            {
                frameManager = &frameManagerE;
                indice = 0;
            }

            counterFrame.x = frameManager->x;
            counterFrame.y = frameManager->y;
            frameManager->tempsBefore = getTime();
            counterFrame.nbLigne = 0;
        }
    }
}

SDL_Rect* Lettre::getAdressePosition()
{
    return &position;
}

void Lettre::execute()
{
    calculNextFrame();
    antiPosition.x = counterFrame.x * CASE_DIMENSION;
    antiPosition.y = counterFrame.y * CASE_DIMENSION;
    antiPosition.w = CASE_DIMENSION;
    antiPosition.h = CASE_DIMENSION;
}
SDL_Rect* Lettre::getAdresseAntiPosition()
{
    return &antiPosition;
}
Lettre::~Lettre()
{

}
