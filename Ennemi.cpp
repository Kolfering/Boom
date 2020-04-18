#include "Ennemi.h"
#include "Teleporter.h"
#include "Enumeration.h"
#include "Explosion.h"
#include "Carte.h"
#include "MenuJeu.h"
#include "Utile.h"

SDL_Texture* Ennemi::imageAlien = nullptr;
Mix_Chunk* Ennemi::sonAlienMort = nullptr;
bool Ennemi::alienMode = false;
Ennemi::Ennemi(Case* caseEnnemi) : Personnage(caseEnnemi)
{
    this->casePersonnage->setEnnemi(this);
    position.x = caseEnnemi->getPositionPixel().x;
    position.y = caseEnnemi->getPositionPixel().y;
    position.w = caseEnnemi->getPositionPixel().w;
    position.h = caseEnnemi->getPositionPixel().h;
    positionReal.x = position.x;
    positionReal.y = position.y;
    amical = false;
    mort = false;
    apparaitre = true;
    alienable = true;
    casePrecedente = casePersonnage;
    tempsDerniereAtk = getTime();
    tempsSonRandom = 0;
    frameAtkActif = false;
    degat = 1;
    frameAtk = true;
    standWhileAtk = false;
    direction = UNDEFINED;
    frameManagerAlienBas = {0,0,4,9,100,0,1};
    frameManagerAlienHaut = {4,0,4,9,100,0,1};
    frameManagerAlienDroite = {8,0,4,9,100,0,1};
    frameManagerAlienGauche = {3,1,4,9,100,0,1};
    frameManagerAlienMort = {7,1,2,9,150,0,1};
}

IDEnnemi Ennemi::getIDEnnemi()
{
    return idEnnemi;
}

bool Ennemi::isApparaitre()
{
    return apparaitre;
}

void Ennemi::initialiserEnnemi()
{
    imageAlien = chargerSurface("Images/AlienFrames.png");
    sonAlienMort = Mix_LoadWAV("Sons/AlienDeath.wav");
    alienMode = false;
}

void Ennemi::releaseEnnemi()
{
    if(imageAlien)
        SDL_DestroyTexture(imageAlien);
    imageAlien = nullptr;
    if(sonAlienMort)
        Mix_FreeChunk(sonAlienMort);
    sonAlienMort = nullptr;
}

Ennemi::~Ennemi()
{
    casePersonnage->setEnnemi(nullptr);
}

void Ennemi::gestionSonRandom()
{
    if(getSonRandom() && !alienMode && !meurt)
    {
        if(getTime() - tempsSonRandom > 500)
        {
           tempsSonRandom = getTime();

           if(genererNombreAleatoire(200) <= PROBABILITY_ENNEMI_SON_RANDOM)
               Mix_PlayChannel(-1,getSonRandom(),0);
        }
    }
}

void Ennemi::setCase(Case* casePersonnage)
{
    this->casePersonnage->setEnnemi(nullptr);
    this->casePersonnage = casePersonnage;
    this->casePersonnage->setEnnemi(this);
    if(casePersonnage->getTeleporter())
    {
        if(casePersonnage->getTeleporter()->isAccessible())
        {
            if(casePersonnage->getTeleporter()->teleporter(this))
                direction = UNDEFINED;
        }
    }

}

bool Ennemi::getAlienMode()
{
    return alienMode;
}

void Ennemi::setAlienMode(bool am)
{
    alienMode = am;
}

void Ennemi::accelerer()
{
    vitesse *= 2;
    if(vitesse>2)
        vitesse = 2;
    intervalAtk /= 2;
}

bool Ennemi::goToCase()
{

    if(meurt)
    {
        if(getTime() - timeMort > TEMPS_MORT)
            mort = true;
    if(!alienMode || !alienable)
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

    if(!alienMode || !alienable)
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
{
    frameAtkActif = false;
}


    if(aAtteintCase())
    {
        this->setCase(caseToGo);
        this->caseToGo = casePersonnage;
        this->casePrecedente = casePersonnage;
        return true;
    }



    if(!frameAtkActif)
    {
        if(!collision())
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
        else
            faitDemiTour();

    }




if(!alienMode || !alienable)
{
    if(!frameAtkActif || !frameAtk)
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
    if(!standWhileAtk || !frameAtkActif)
        calculNextFrame();
    return false;

}


void Ennemi::faitDemiTour()
{
    if(!frameAtkActif)
    {
        Direction dir = direction;
        direction = directionInverse;
        directionInverse = dir;

        if(caseToGo->equals(casePersonnage))
            caseToGo = casePrecedente;
        else
            caseToGo = casePersonnage;
    }
}

float Ennemi::getDegat()
{
    return degat;
}

void Ennemi::collisionBomber()
{
    for(unsigned int i = 0;i<MenuJeu::getListeBomber().size();i++)
    {
        if(caseToGo->equals(MenuJeu::getListeBomber().at(i)->getCaseToGo()) || caseToGo->equals(MenuJeu::getListeBomber().at(i)->getCase()) || casePersonnage->equals(MenuJeu::getListeBomber().at(i)->getCaseToGo()) ||  casePersonnage->equals(MenuJeu::getListeBomber().at(i)->getCase()))
        {
            if(!((getPosition().x >= MenuJeu::getListeBomber().at(i)->getPosition().x + MenuJeu::getListeBomber().at(i)->getTaille())
               || (getPosition().x + taille <= MenuJeu::getListeBomber().at(i)->getPosition().x)
               || (getPosition().y >= MenuJeu::getListeBomber().at(i)->getPosition().y + MenuJeu::getListeBomber().at(i)->getTaille())
               || (getPosition().y + taille <=  MenuJeu::getListeBomber().at(i)->getPosition().y)))
                    MenuJeu::getListeBomber().at(i)->prendreDegat(degat);
        }
    }
}


Case* Ennemi::getCasePrecedente()
{
    return casePrecedente;
}
bool Ennemi::collision()
{

    for(unsigned int i = 0;i<Carte::getListeEnnemi().size();i++)
    {
        if(!Carte::getListeEnnemi().at(i)->getCase()->equals(casePersonnage))
        {
            if(checkCase(caseToGo,Carte::getListeEnnemi().at(i)->getCaseToGoOnList()) || checkCase(caseToGo,(Carte::getListeEnnemi().at(i)->getCaseOnList())) || checkCase(caseToGo,(Carte::getListeEnnemi().at(i)->getCasePrecedenteOnList())))
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

std::vector<Case*> Ennemi::getCaseOnList()
{
    std::vector<Case*> vec;
    vec.push_back(casePersonnage);
    return vec;
}

std::vector<Case*> Ennemi::getCaseToGoOnList()
{
    std::vector<Case*> vec;
    vec.push_back(caseToGo);
    return vec;
}

std::vector<Case*> Ennemi::getCasePrecedenteOnList()
{
    std::vector<Case*> vec;
    vec.push_back(casePrecedente);
    return vec;
}

bool Ennemi::checkCase(Case* caseFirst,std::vector<Case*> listeCase)
{
    for(unsigned int i =0;i<listeCase.size();i++)
    {
        if(caseFirst->equals(listeCase.at(i)))
            return true;
    }
    return false;
}

void Ennemi::prendreDegat(Bomber* bomber, unsigned int ID)
{
    if(!meurt)
    {
        if(!alienMode || !alienable)
            Mix_PlayChannel(-1,getSonMort(),0);
        else
            Mix_PlayChannel(-1,sonAlienMort,0);
        timeMort = getTime();
        meurt = true;
        counter = 0;
        enDeplacementVersCase = false;
        MenuJeu::ajouterScore(score,positionReal.x,positionReal.y,bomber);

    }
}

bool Ennemi::isMort()
{
    return mort;
}
