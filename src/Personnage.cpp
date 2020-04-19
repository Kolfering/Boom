#include "Personnage.h"
#include "Enumeration.h"
#include "Utile.h"

Personnage::Personnage(Case* casePersonnage)
{
    taille = CASE_DIMENSION;
    enDeplacementVersCase = false;
    this->casePersonnage = casePersonnage;
    meurt = false;
    counter = 0;
    this->caseToGo = casePersonnage;
    timeMort = 0;
}


bool Personnage::isMeurt()
{
    return meurt;
}

void Personnage::calculNextFrame()
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

}

int Personnage::getTaille()
{
    return taille;
}

Personnage::Personnage()
{
    taille = CASE_DIMENSION;
    enDeplacementVersCase = false;
    this->casePersonnage = nullptr;
    meurt = false;
    counter = 0;
    direction = Direction::BAS;
    this->caseToGo = nullptr;
}

unsigned int Personnage::getVie()
{
    return vie;
}

void Personnage::setPositionReal(float x,float y)
{
    positionReal.x = x;
    positionReal.y = y;
}
SDL_Rect Personnage::getPosition()
{
    position.x = positionReal.x;
    position.y = positionReal.y;
    return position;
}

Direction Personnage::getDirection()
{
    return direction;
}

SDL_Rect* Personnage::getAdressePosition()
{
    position.x = positionReal.x;
    position.y = positionReal.y;
    return &position;
}


bool Personnage::isEnDeplacementVersCase()
{
    return enDeplacementVersCase;
}

bool Personnage::isAmical()
{
    return amical;
}

bool Personnage::aAtteintCase()
{
    switch(direction)
    {
    case Direction::HAUT:
        if(getPosition().y <= caseToGo->getPositionPixel().y)
        {
            positionReal.y = caseToGo->getPositionPixel().y;
            return true;
        }
        break;
    case Direction::BAS:
        if(getPosition().y >= caseToGo->getPositionPixel().y)
        {
            positionReal.y = caseToGo->getPositionPixel().y;
            return true;
        }
        break;
    case Direction::DROITE:
        if(getPosition().x >= caseToGo->getPositionPixel().x)
        {
            positionReal.x = caseToGo->getPositionPixel().x;
            return true;
        }
        break;
    case Direction::GAUCHE:
        if(getPosition().x <= caseToGo->getPositionPixel().x)
        {
            positionReal.x = caseToGo->getPositionPixel().x;
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

void Personnage::setVitesse(float vitesse)
{
    this->vitesse = vitesse;
}
void Personnage::setDirection(Direction direction)
{
    this->direction = direction;
}

float Personnage::getVitesse()
{
    return vitesse;
}

SDL_Rect* Personnage::getAdresseAntiPosition()
{
    antiPosition.x = counterFrame.x * taille;
    antiPosition.y = counterFrame.y * taille;
    antiPosition.w = taille;
    antiPosition.h = taille;
    return &antiPosition;
}

Case* Personnage::getCaseToGo()
{
    return caseToGo;
}
Personnage::~Personnage()
{
  casePersonnage = nullptr;
  caseToGo = nullptr;
}


Case* Personnage::getCase()
{
    return casePersonnage;
}
