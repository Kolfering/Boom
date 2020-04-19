#include <SDL_image.h>
#include "Bomber.h"
#include "Enumeration.h"
#include "Utile.h"
#include "Carte.h"
#include "Bomb.h"
#include "MenuJeu.h"

SDL_Texture* Bomber::imageProtection;
Mix_Chunk* Bomber::sonBombe;
Mix_Chunk* Bomber::sonBonus;
Mix_Chunk* Bomber::sonMegaVie;
Mix_Chunk* Bomber::sonLettre;

Bomber::Bomber() : Personnage()
{

    player = MenuJeu::getListeBomber().size() + 1;
    vie = VIE_MAX;
    setVitesseUp(false);
    setBombeRapide(false);
    setProtection(false);
    nombreBombes = NOMBRE_BOMBE_BASE;
    enDeplacement = false;
    hurt = false;
    amical = true;
    immunise = false;
    tmpImmunise = false;
    celebration = false;
    aRespawn = false;
    gameEnded = false;
    tempsMort = 0;
    megaVie = 3;
    score = 0;
    apparaitreProtection = true;
    apparaitreTmpProtection = false;
    caseSpawn = nullptr;
    distanceExplosion = DISTANCE_BASE;
    position.w = CASE_DIMENSION;
    position.h = CASE_DIMENSION;

    if(player==1)
        image = chargerSurface("../Images/player1.png");
    else
         image = chargerSurface("../Images/player2.png");


    frameManagerBas = {0,0,8,19,80,0,1};
    frameManagerHaut = {8,0,8,19,80,0,1};
    frameManagerDroite = {16,0,8,19,80,0,1};
    frameManagerGauche = {5,1,8,18,80,0,1};
    frameManagerHit = {17,1,1,18,200,0,1};
    frameManagerDead = {13,1,3,18,80,0,1};
    frameManagerWin = {16,1,1,18,0,0,1};
    frameManager = &frameManagerBas;

    counterFrame = {0,0,0};
    positionProtection = {0,0,CASE_DIMENSION,CASE_DIMENSION};

    if(player==1)
    {
        playerKeyboard = {SDL_SCANCODE_W,SDL_SCANCODE_S,SDL_SCANCODE_D,SDL_SCANCODE_A,SDL_SCANCODE_SPACE};
        sonHurt = Mix_LoadWAV("../Sons/P1Ouch.wav");
        sonMort = Mix_LoadWAV("../Sons/P1Death.wav");
        sonComplete = Mix_LoadWAV("../Sons/P1Complete.wav");

    }
    else
    {
        playerKeyboard = {SDL_SCANCODE_UP,SDL_SCANCODE_DOWN,SDL_SCANCODE_RIGHT,SDL_SCANCODE_LEFT,SDL_SCANCODE_RCTRL};
        sonHurt = Mix_LoadWAV("../Sons/P2Ouch.wav");
        sonMort = Mix_LoadWAV("../Sons/P2Death.wav");
        sonComplete = Mix_LoadWAV("../Sons/P2Complete.wav");
    }


    for(unsigned int i=0;i<5;i++)
        listeExtraLettre.push_back(false);
}

bool Bomber::isBombeRapide()
{
    return bombeRapide;
}

void Bomber::releaseBomber()
{
    if(imageProtection)
        SDL_DestroyTexture(imageProtection);
    imageProtection = nullptr;
    if(sonBombe)
        Mix_FreeChunk(sonBombe);
    sonBombe = nullptr;
    if(sonMegaVie)
        Mix_FreeChunk(sonMegaVie);
    sonMegaVie = nullptr;
    if(sonBonus)
        Mix_FreeChunk(sonBonus);
    sonBonus = nullptr;
    if(sonLettre)
        Mix_FreeChunk(sonLettre);
    sonLettre = nullptr;
}

void Bomber::initialiserBomber()
{
    imageProtection = chargerSurface("../Images/shield.png");
    sonLettre = Mix_LoadWAV("../Sons/GetLetter.wav");
    sonMegaVie = Mix_LoadWAV("../Sons/ExtraLife.wav");
    sonBombe = Mix_LoadWAV("../Sons/LightTheFuse.wav");
    sonBonus = Mix_LoadWAV("../Sons/GetBonus.wav");
}


SDL_Texture* Bomber::getImage()
{
    return image;
}
void Bomber::activerCelebration()
{
    celebration = true;
}
SDL_Rect* Bomber::getAdresseAntiPositionProtection()
{
    unsigned int v = 0;
    switch(direction)
    {
    case Direction::DROITE:
        v = 1;
        break;
    case Direction::GAUCHE:
        v = 2;
        break;
    default:
        break;
    }

    positionProtection.x = v*CASE_DIMENSION;
    return &positionProtection;
}

void Bomber::augmenterScore(unsigned int score)
{
    this->score += score;
}

void Bomber::setCaseSpawn(Case* spawn)
{
    caseSpawn = spawn;
}

void Bomber::setBombeRapide(bool bomb)
{
    bombeRapide = bomb;
    tempsExplosion = bombeRapide ? TEMPS_EXPLOSION_RAPIDE : TEMPS_EXPLOSION;
}

void Bomber::respawn()
{
    meurt = false;
    positionReal.x = casePersonnage->getPositionPixel().x;
    positionReal.y = casePersonnage->getPositionPixel().y;
    position.x = positionReal.x;
    position.y = positionReal.y;
    caseToGo->setReservee(false);
    caseToGo = casePersonnage;
    vitesse = VITESSE_BASE;
    nombreBombes = NOMBRE_BOMBE_BASE;
    enDeplacement = false;
    setBombeRapide(false);
    setProtection(false);
    setVitesseUp(false);
    enDeplacementVersCase = false;
    distanceExplosion = DISTANCE_BASE;
    vie = VIE_MAX;
    megaVie--;
    aRespawn = true;
    tempsMort = getTime();
    apparaitreProtection = true;
    immunise = true;
}
void Bomber::spawn()
{
    casePersonnage = caseSpawn;
    caseToGo = caseSpawn;
    positionReal.x = caseSpawn->getPositionPixel().x;
    positionReal.y = caseSpawn->getPositionPixel().y;
    position.x = positionReal.x;
    position.y = positionReal.y;
    caseSpawn->setBomber(this);
    Carte::ajouterEffet(new Effet(position.x,position.y));

}

void Bomber::setCase(Case* casePersonnage)
{
    this->casePersonnage->setBomber(nullptr);
    this->casePersonnage = casePersonnage;
    this->casePersonnage->setBomber(this);

     position.y = casePersonnage->getPositionPixel().y;
     position.x = casePersonnage->getPositionPixel().x;;
     positionReal.x = casePersonnage->getPositionPixel().x;
     positionReal.y = casePersonnage->getPositionPixel().y;

    if(casePersonnage->getTeleporter())
    {
        if(casePersonnage->getTeleporter()->isAccessible())
            casePersonnage->getTeleporter()->teleporter(this);
    }
}


void Bomber::prendreDegat(unsigned int degat)
{
    if(!immunise && !tmpImmunise && !meurt)
    {
        vie = vie - degat;
        Mix_PlayChannel(-1,sonHurt,0);
        hurt = true;
        timeHurt = getTime();
        tmpImmunise = true;
    }
}

unsigned int Bomber::getScore()
{
    return score;
}
void Bomber::setImmunise(bool imu)
{
    immunise = imu;
}

bool Bomber::isProtection()
{
    return protection;
}

void Bomber::setProtection(bool pr)
{
    protection = pr;
    immunise = pr;
}

bool Bomber::isVitesseUp()
{
    return vitesseUp;
}

void Bomber::setVitesseUp(bool vu)
{
    vitesseUp = vu;
    vitesse = vitesseUp ? VITESSE_UP : VITESSE_BASE;
}
bool Bomber::isImmunise()
{
    return immunise;
}

bool Bomber::isTmpImmunise()
{
    return tmpImmunise;
}

unsigned int Bomber::getMegaVie()
{
    return megaVie;
}

bool Bomber::isApparaitreTmpProtection()
{
    return apparaitreTmpProtection;
}

bool Bomber::isGameEnded()
{
    return gameEnded;
}

void Bomber::preparerNiveau()
{
    for(unsigned int i = 0;i<listeBonus.size();i++)
    {
        delete listeBonus.at(i);
        listeBonus.erase(listeBonus.begin() + i);
        i--;
    }
    for(unsigned int i = 0;i<listeBombes.size();i++)
    {
        delete listeBombes.at(i);
        listeBombes.erase(listeBombes.begin() + i);
        i--;
    }

    celebration = false;
    tmpImmunise = false;
    hurt = false;
    setVitesseUp(false);
    setProtection(false);

}

void Bomber::deplacer(Input* in)
{

    enDeplacement = true;
    calculEffetBonus();

    if(aRespawn)
    {
        if(getTime() - tempsMort > TEMPS_RESPAWN_PROTECTION)
        {
            aRespawn = false;
            apparaitreProtection = false;
            immunise = false;
        }
        else
        {
            if(getTime()- tempsMort > TEMPS_ALERTE_RESPAWN_PROTECTION)
            {
                if(getTime() - tempsTmpProtection > TEMPS_ALERTE_TMP_PROTECTION)
                {
                     tempsTmpProtection = getTime();
                     apparaitreProtection = !apparaitreProtection;
                }
            }
        }

    }

    if(meurt)
    {
        if(getTime() - tempsMort > TEMPS_BOMBER_MORT)
        {
            if(megaVie > 0)
                respawn();
            else
                setEnded();
        }
    }

    if(tmpImmunise && !hurt)
    {
       if(getTime() - timeHurt > TEMPS_TMP_PROTECTION)
       {
           tmpImmunise = false;
           apparaitreTmpProtection = false;
       }
       else
       {
           if(getTime() - tempsTmpProtection > TEMPS_ALERTE_TMP_PROTECTION)
           {
               tempsTmpProtection = getTime();
               apparaitreTmpProtection = !apparaitreTmpProtection;
           }
       }
    }

    if(hurt)
    {
        if(!compareFrameManager(*frameManager,frameManagerHit))
        {
            counterFrame.x = frameManagerHit.x;
            counterFrame.y = frameManagerHit.y;
            counterFrame.nbLigne = 0;
        }
            frameManager = &frameManagerHit;
        if(getTime() - timeHurt > TEMPS_SPRITE_HURT)
        {
            hurt = false;
            if(vie<=0)
            {
                Mix_PlayChannel(-1,sonMort,0);
                vie = 0;
                meurt = true;
                tempsMort = getTime();
                tmpImmunise = false;
            }
        }


        return;
    }
    if(meurt)
    {
          if(!compareFrameManager(*frameManager,frameManagerDead))
          {
              counterFrame.x = frameManagerDead.x;
              counterFrame.y = frameManagerDead.y;
              counterFrame.nbLigne = 0;
          }
        frameManager = &frameManagerDead;
        enDeplacementVersCase = false;
        enDeplacement = false;
        calculNextFrame();
        return;
    }

    collisionLettre();

    if(in->key[playerKeyboard.bomb])
    {
         poserBombe();
    }

    if(enDeplacementVersCase)
    {
        switch(direction)
        {
            case Direction::HAUT:
                  if(!compareFrameManager(*frameManager,frameManagerHaut))
                  {
                      counterFrame.x = frameManagerHaut.x;
                      counterFrame.y = frameManagerHaut.y;
                      counterFrame.nbLigne = 0;
                  }
                frameManager = &frameManagerHaut;
                break;
            case Direction::BAS:
                  if(!compareFrameManager(*frameManager,frameManagerBas))
                  {
                      counterFrame.x = frameManagerBas.x;
                      counterFrame.y = frameManagerBas.y;
                      counterFrame.nbLigne = 0;
                  }
                frameManager = &frameManagerBas;
                break;
            case Direction::DROITE:
                  if(!compareFrameManager(*frameManager,frameManagerDroite))
                  {
                      counterFrame.x = frameManagerDroite.x;
                      counterFrame.y = frameManagerDroite.y;
                      counterFrame.nbLigne = 0;
                  }
                frameManager = &frameManagerDroite;
                break;
            case Direction::GAUCHE:
                  if(!compareFrameManager(*frameManager,frameManagerGauche))
                  {
                      counterFrame.x = frameManagerGauche.x;
                      counterFrame.y = frameManagerGauche.y;
                      counterFrame.nbLigne = 0;
                  }
                frameManager = &frameManagerGauche;
                break;
            default:
                break;
        }
        deplacerDirection(direction);
    }
    else if(in->key[playerKeyboard.up])
    {

          if(!compareFrameManager(*frameManager,frameManagerHaut))
          {
              counterFrame.x = frameManagerHaut.x;
              counterFrame.y = frameManagerHaut.y;
              counterFrame.nbLigne = 0;
          }
        frameManager = &frameManagerHaut;
        deplacerDirection(Direction::HAUT);
    }
    else if(in->key[playerKeyboard.down])
    {
          if(!compareFrameManager(*frameManager,frameManagerBas))
          {
              counterFrame.x = frameManagerBas.x;
              counterFrame.y = frameManagerBas.y;
              counterFrame.nbLigne = 0;
          }
        frameManager = &frameManagerBas;
        deplacerDirection(Direction::BAS);
    }
    else if(in->key[playerKeyboard.left])
    {
          if(!compareFrameManager(*frameManager,frameManagerGauche))
          {
              counterFrame.x = frameManagerGauche.x;
              counterFrame.y = frameManagerGauche.y;
              counterFrame.nbLigne = 0;
          }
        frameManager = &frameManagerGauche;
        deplacerDirection(Direction::GAUCHE);
    }
    else if(in->key[playerKeyboard.right])
    {
          if(!compareFrameManager(*frameManager,frameManagerDroite))
          {
              counterFrame.x = frameManagerDroite.x;
              counterFrame.y = frameManagerDroite.y;
              counterFrame.nbLigne = 0;
          }
        frameManager = &frameManagerDroite;
        deplacerDirection(Direction::DROITE);
    }
    else
    {
        enDeplacementVersCase = false;
        enDeplacement = false;
        direction = Direction::BAS;
        counterFrame.nbLigne = 0;

        if(!celebration)
            frameManager = &frameManagerBas;
        else
        {
            if(!compareFrameManager(*frameManager,frameManagerWin))
            {
                frameManager = &frameManagerWin;
                Mix_PlayChannel(-1,sonComplete,0);
            }

        }


        counterFrame.x = frameManager->x;
        counterFrame.y = frameManager->y;
        return;
    }

    calculNextFrame();

}

void Bomber::setEnded()
{
    gameEnded = true;
    if(caseToGo->isReservee())
        caseToGo->setReservee(false);
    casePersonnage->setBomber(nullptr);
    positionReal.x = 0;
    positionReal.y = 0;
    position.x = positionReal.x;
    position.y = positionReal.y;
}

void Bomber::gagnerVie(unsigned int vie)
{
    this->vie += vie;
    if(this->vie > VIE_MAX)
        this->vie = VIE_MAX;
}

PlayerKeyboard Bomber::getPlayerKeyboard()
{
    return playerKeyboard;
}

void Bomber::setPlayerKeyboard(PlayerKeyboard pk)
{
    playerKeyboard = pk;
}

bool Bomber::poserBombe()
{
    if(!enDeplacementVersCase)
    {
        if(!casePersonnage->getBombe() && listeBombes.size() < nombreBombes)
        {
            Mix_PlayChannel(-1,sonBombe,0);
            listeBombes.push_back(new Bomb(this,casePersonnage));
            return true;
        }
    }

    return false;

}

int Bomber::getTempsExplosion()
{
    return tempsExplosion;
}

unsigned int Bomber::getDistanceExplosion()
{
    return distanceExplosion;
}

unsigned int Bomber::getNombreBombes()
{
    return nombreBombes;
}

void Bomber::setNombreBombes(unsigned int nb)
{
    nombreBombes = nb;
    if(nb>=NOMBRE_BOMBE_MAX)
        nombreBombes = NOMBRE_BOMBE_MAX;

}

void Bomber::setDistanceExplosion(unsigned int dis)
{
    distanceExplosion = dis;
    if(dis>=DISTANCE_MAX)
        distanceExplosion = DISTANCE_MAX;
}

vector<Bomb*> &Bomber::getListeBombes()
{
    return listeBombes;
}

void Bomber::setRespawn(bool rp)
{
    aRespawn = rp;
}


SDL_Texture* Bomber::getImageProtection()
{
    return imageProtection;
}

void Bomber::deplacerDirection(Direction direction)
{
    if(!enDeplacementVersCase)
    {
        this->direction = direction;
        Case* c = nullptr;

        switch(direction)
        {
        case Direction::HAUT :
            c = Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x);
            break;
        case Direction::BAS :
            c = Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x);
            break;
        case Direction::DROITE :
            c = Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1);
            break;
        case Direction::GAUCHE :
            c = Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1);
            break;
        default:
            break;
        }

        if(debugTeleporter())
        {
            setCase(casePersonnage);
            return;
        }

        if(c!=nullptr)
        {
            if(c->getType()==TypeCase::FOND)
            {
                if(!c->isReservee())
                {
                    if(!c->getBomber())
                    {
                          caseToGo = c;
                          enDeplacementVersCase = !(goToCase());

                           if(c->getBonus())
                            {
                                ajouterBonus(c->getBonus());
                            }
                           else if(c->getCoin())
                           {
                               if(!c->getCoin()->isPickUp())
                                c->getCoin()->pickedUp(this);
                           }
                    }
                    else
                    {
                        if(c->getBomber()->isEnDeplacementVersCase() && direction == c->getBomber()->direction)
                        {
                          caseToGo = c;
                          enDeplacementVersCase = !(goToCase());

                            if(c->getBonus())
                            {
                                ajouterBonus(c->getBonus());
                            }
                            else if(c->getCoin())
                            {
                                if(!c->getCoin()->isPickUp())
                                    c->getCoin()->pickedUp(this);
                            }
                        }
                    }
                }
            }
        }
    }
    else
        enDeplacementVersCase = !(goToCase());



}

void Bomber::collisionLettre()
{
    if(Ennemi::getAlienMode())
    {
        for(unsigned int i =0; i<Carte::getListeLettre().size();i++)
        {
          if(!((getPosition().x >= Carte::getListeLettre().at(i)->getAdressePosition()->x + CASE_DIMENSION)
           || (getPosition().x + CASE_DIMENSION <= Carte::getListeLettre().at(i)->getAdressePosition()->x)
           || (getPosition().y >= Carte::getListeLettre().at(i)->getAdressePosition()->y + CASE_DIMENSION)
           || (getPosition().y + CASE_DIMENSION <=  Carte::getListeLettre().at(i)->getAdressePosition()->y)))
            {
                Mix_PlayChannel(-1,sonLettre,0);
                MenuJeu::ajouterScore(Lettre::getScore(),Carte::getListeLettre().at(i)->getAdressePosition()->x,Carte::getListeLettre().at(i)->getAdressePosition()->y,this);
                listeExtraLettre.at(Carte::getListeLettre().at(i)->getIndice()) = true;
                bool up = true;
                for(unsigned int i = 0;i<5;i++)
                {
                    if(!listeExtraLettre.at(i))
                    {
                        up = false;
                        break;
                    }
                }
                if(up)
                {
                    megaVie+=1;
                    MenuJeu::ajouterScore(player,getPosition().x,getPosition().y,this);
                    Mix_PlayChannel(-1,sonMegaVie,0);
                       for(unsigned int i = 0;i<5;i++)
                        listeExtraLettre.at(i) = false;
                }
                Carte::retirerLettre(i);
                i--;

            }
        }
    }
}

bool Bomber::debugTeleporter()
{
    if(casePersonnage->getTeleporter())
    {
        if(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x) != nullptr)
        {
            if(!(Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x)->getType() != TypeCase::FOND ||
               Carte::getCase(casePersonnage->getPositionDamier().y+1,casePersonnage->getPositionDamier().x)->getBomber() != nullptr))
                return false;
        }
        if(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x) != nullptr)
        {
            if(!(Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x)->getType() != TypeCase::FOND ||
               Carte::getCase(casePersonnage->getPositionDamier().y-1,casePersonnage->getPositionDamier().x)->getBomber() != nullptr))
                return false;
        }
        if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1) != nullptr)
        {
            if(!(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1)->getType() != TypeCase::FOND ||
               Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x+1)->getBomber() != nullptr))
                return false;
        }
        if(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1) != nullptr)
        {
            if(!(Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1)->getType() != TypeCase::FOND ||
               Carte::getCase(casePersonnage->getPositionDamier().y,casePersonnage->getPositionDamier().x-1)->getBomber() != nullptr))
                return false;
        }
        return true;
    }
    return false;
}

bool Bomber::goToCase()
{

    if(caseToGo->getType()== TypeCase::CASSABLE || caseToGo->getType()==TypeCase::DUR)
        return true;

    if(caseToGo->getExplosion())
        prendreDegat(caseToGo->getExplosion()->getDegat());

    caseToGo->setReservee(true);

       if(aAtteintCase())
       {
            this->setCase(caseToGo);
            caseToGo->setReservee(false);
            this->caseToGo = casePersonnage;
            return true;
       }


    switch(direction)
    {
    case Direction::HAUT :
        positionReal.y-= vitesse;
        break;
    case Direction::BAS :
        positionReal.y+= vitesse;
        break;
    case Direction::DROITE :
        positionReal.x+= vitesse;
        break;
    case Direction::GAUCHE :
        positionReal.x-= vitesse;
        break;
    default:
        break;
    }
    return false;
}

bool Bomber::isApparaitreProtection()
{
    return apparaitreProtection;
}

void Bomber::setApparaitreProtection(bool protection)
{
    apparaitreProtection = protection;
}

vector<Bonus*> &Bomber::getListeBonus()
{
    return listeBonus;
}

vector<bool> &Bomber::getListeExtraLettre()
{
    return listeExtraLettre;
}

void Bomber::calculEffetBonus()
{

    for(unsigned int i=0;i<listeBonus.size();i++)
    {
        if(listeBonus.at(i)->isActif())
            listeBonus.at(i)->calculEffet();
        else
        {
            delete listeBonus.at(i);
            listeBonus.erase(listeBonus.begin() + i);
            i--;
        }
    }
}

void Bomber::ajouterBonus(Bonus* bonus)
{
    Mix_PlayChannel(-1,sonBonus,0);
    MenuJeu::ajouterScore(bonus->getScore(),bonus->getCase()->getPositionPixel().x,bonus->getCase()->getPositionPixel().y,this);
    Carte::retirerBonus(bonus);
    bonus->setBomber(this);
    for(unsigned int i=0;i<listeBonus.size();i++)
    {
        if(listeBonus.at(i)->equals(bonus))
            listeBonus.at(i)->setActif(false);
    }

    listeBonus.push_back(bonus);
    bonus->declencherEffet();
}


Bomber::~Bomber()
{

   for(unsigned int i = 0;i<listeBonus.size();i++)
    {
        delete listeBonus.at(i);
        listeBonus.erase(listeBonus.begin() + i);
        i--;
    }
    for(unsigned int i = 0;i<listeBombes.size();i++)
    {
        delete listeBombes.at(i);
        listeBombes.erase(listeBombes.begin() + i);
        i--;
    }
    if(sonBombe)
        Mix_FreeChunk(sonBombe);
    sonBombe = nullptr;
    if(sonBonus)
        Mix_FreeChunk(sonBonus);
    sonBonus = nullptr;
    if(sonHurt)
        Mix_FreeChunk(sonHurt);
    sonHurt = nullptr;
    if(sonMort)
        Mix_FreeChunk(sonMort);
    sonMort = nullptr;
    if(sonComplete)
        Mix_FreeChunk(sonComplete);
    sonComplete = nullptr;
    if(sonLettre)
        Mix_FreeChunk(sonLettre);
    sonLettre = nullptr;
    if(sonMegaVie)
        Mix_FreeChunk(sonMegaVie);
    sonMegaVie = nullptr;
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
    if(imageProtection)
        SDL_DestroyTexture(imageProtection);
    imageProtection = nullptr;

}
