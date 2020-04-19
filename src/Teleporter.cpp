#include "Teleporter.h"
#include "Utile.h"
#include "Carte.h"
#include "Effet.h"
#include "MenuJeu.h"

SDL_Texture* Teleporter::image = nullptr;
Teleporter::Teleporter(Case* caseTeleporter,unsigned int indice,unsigned int indiceFrere)
{
    this->caseTeleporter = caseTeleporter;
    counter = 0;
    tempsBefore = 0;
    tempsDernierAcces = 0;
    this->indice = indice;
    this->indiceFrere = indiceFrere;
    accessible = true;
}

void Teleporter::initialiserTeleporter()
{
    image = chargerSurface("../Images/teleporter.png");
}

void Teleporter::releaseTeleporter()
{
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}

Teleporter::~Teleporter()
{

}

Case* Teleporter::getCase()
{
    return caseTeleporter;
}

void Teleporter::setCase(Case* caseTeleporter)
{
    this->caseTeleporter = caseTeleporter;
}

SDL_Texture* Teleporter::getImage()
{
    return image;
}

void Teleporter::execute()
{
    if(getTime()-tempsBefore > TEMPS_FRAME_TELEPORTER && accessible)
    {
        counter++;
        if(counter>=NB_FRAME_TELEPORTER)
            counter = 0;

        tempsBefore = getTime();
    }
    else if(!accessible)
    {
        if(getTime() - tempsDernierAcces > TEMPS_TELEPORTER_REPOS)
        {
            accessible = true;
        }
    }
}

unsigned int Teleporter::getIndice()
{
    return indice;
}

bool Teleporter::teleporter(Personnage* personnage)
{
    Teleporter* tp = Carte::getTeleporterFrere(indiceFrere);

    if(tp==nullptr)
        return false;

    if(tp->getCase()->getBomber()!=nullptr && personnage->isAmical())
        return false;

    if(tp->getCase()->getEnnemi()!=nullptr && !personnage->isAmical())
        return false;

    if(personnage->isAmical())
    {
        for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
        {
            if(!MenuJeu::getListeBomber().at(i)->getCase()->equals(personnage->getCase()))
            {
                if(MenuJeu::getListeBomber().at(i)->getCaseToGo())
                {
                    if(MenuJeu::getListeBomber().at(i)->getCaseToGo()->equals(tp->getCase()))
                        return false;
                }
            }
        }
    }

    else
    {
        for(unsigned int i = 0;i<Carte::getListeEnnemi().size();i++)
        {
            if(!Carte::getListeEnnemi().at(i)->getCase()->equals(personnage->getCase()))
            {
                if(Carte::getListeEnnemi().at(i)->getCaseToGo())
                {
                    if(Carte::getListeEnnemi().at(i)->getCaseToGo()->equals(tp->getCase()))
                        return false;
                }
            }
        }
    }


    tp->setAccessible(false);
    setAccessible(false);
    Carte::ajouterEffet(new Effet(personnage->getCase()->getPositionPixel().x + (CASE_DIMENSION/2 - TAILLE_FRAME_EFFET/2),personnage->getCase()->getPositionPixel().y));
    personnage->setCase(tp->getCase());
    personnage->setPositionReal(personnage->getCase()->getPositionPixel().x,personnage->getCase()->getPositionPixel().y);
    tempsDernierAcces = getTime();
    tp->setTempsDernierAcces(tempsDernierAcces);

    if(tp->getIndiceFrere() != indice)
        tp->setAccessible(true);

    Carte::ajouterEffet(new Effet(personnage->getCase()->getPositionPixel().x + (CASE_DIMENSION/2 - TAILLE_FRAME_EFFET/2),personnage->getCase()->getPositionPixel().y));



    return true;

}

unsigned int Teleporter::getIndiceFrere()
{
    return indiceFrere;
}
void Teleporter::setTempsDernierAcces(unsigned int temps)
{
    tempsDernierAcces = temps;
}

void Teleporter::setAccessible(bool accessible)
{
    this->accessible = accessible;
}
bool Teleporter::isAccessible()
{
    return accessible;
}
SDL_Rect* Teleporter::getAntiPosition()
{
    antiPosition.x = counter* TAILLE_FRAME_TELEPORTER;
    antiPosition.y = 0;
    antiPosition.w = TAILLE_FRAME_TELEPORTER;
    antiPosition.h = TAILLE_FRAME_TELEPORTER;
    return &antiPosition;
}
