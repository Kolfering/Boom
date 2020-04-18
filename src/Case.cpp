#include "Case.h"
#include "Enumeration.h"
#include "Utile.h"
#include "Bomb.h"
#include "Carte.h"
#include "Explosion.h"
#include "Bonus.h"
#include "Teleporter.h"
#include <SDL_image.h>
#include <sstream>
#include "MenuJeu.h"

unsigned int Case::case_dur;
unsigned int Case::case_cassable;
unsigned int Case::case_fond;
unsigned int Case::case_bordure;
SDL_Texture* Case::imageCassable = nullptr;
SDL_Texture* Case::imageFond = nullptr;
SDL_Texture* Case::imageDur = nullptr;
SDL_Texture* Case::imageBordure = nullptr;
Mix_Chunk* Case::sonCase = nullptr;
int Case::longueur_case = 32;
int Case::hauteur_case = 32;


Case::Case(TypeCase type,SDL_Rect position)
{
    counter = 0;
    this->positionPixel = position;
    this->type = type;
    this->bombe = nullptr;
    this->bomber = nullptr;
    this->ennemi = nullptr;
    this->bonus = nullptr;
    this->coin = nullptr;
    this->teleporter = nullptr;
    this->explosion = nullptr;
    cassee = false;
    explose = false;
    reservee = false;
    score = SCORE_CASE;
    antiPositionPixel.w = CASE_DIMENSION;
    antiPositionPixel.h = CASE_DIMENSION;

}

void Case::initialiserCase()
{
    std::stringstream ss;
    ss << "Images/blockPattern" << case_fond << ".png";
    std::string s = ss.str();
    imageFond = chargerSurface(s.c_str());
    std::stringstream ss2;
    ss2 << "Images/Border" << case_bordure << ".png";
    std::string s2 = ss2.str();
    imageBordure = chargerSurface(s2.c_str());
    imageCassable = chargerSurface("Images/breakableBlocks.png");
    imageDur = chargerSurface("Images/fixedBlocks.png");
    sonCase = Mix_LoadWAV("Sons/BlockDestruction.wav");
}

void Case::releaseCase()
{
    if(imageCassable)
        SDL_DestroyTexture(imageCassable);
    imageCassable = nullptr;
    if(imageDur)
        SDL_DestroyTexture(imageDur);
    imageDur = nullptr;
    if(imageFond)
        SDL_DestroyTexture(imageFond);
    imageFond = nullptr;
    if(imageBordure)
        SDL_DestroyTexture(imageBordure);
    imageBordure = nullptr;
    if(sonCase)
        Mix_FreeChunk(sonCase);
    sonCase = nullptr;
}

void Case::setExplosion(Explosion* expl)
{
    explosion = expl;
}

Explosion* Case::getExplosion()
{
    return explosion;
}

SDL_Texture* Case::getImageBordure()
{
    return imageBordure;
}
bool Case::isReservee()
{
    return reservee;
}

void Case::setReservee(bool reservee)
{
    this->reservee = reservee;
}


Coin* Case::getCoin()
{
    return coin;
}

void Case::setCoin(Coin* coin)
{
    this->coin = coin;
}
Teleporter* Case::getTeleporter()
{
    return teleporter;
}
void Case::setTeleporter(Teleporter* tp)
{
    teleporter = tp;
}
TypeCase Case::getType()
{
    return type;
}
SDL_Texture* Case::getImage()
{
    if(type==DUR)
        return imageDur;

    if(type==CASSABLE)
        return imageCassable;

    return imageFond;

}

void Case::execute()
{
     if(explose && !cassee)
        calculNextFrame(CASE_FRAME);
}

SDL_Rect* Case::getAdresseAntiPositionPixel()
{
   switch(type)
    {
    case FOND:
        antiPositionPixel.x = 0;
        antiPositionPixel.y = 0;
        break;
    case DUR:
        antiPositionPixel.x = case_dur*CASE_DIMENSION;
        antiPositionPixel.y = 0;
        break;
    case CASSABLE:
        antiPositionPixel.x = counter*CASE_DIMENSION;
        antiPositionPixel.y = case_cassable*CASE_DIMENSION;
        break;
    default:
        break;
    }
    return &antiPositionPixel;
}

Bonus* Case::getBonus()
{
    return bonus;
}

void Case::setBonus(Bonus* bonus)
{
    this->bonus = bonus;
}

SDL_Texture* Case::getImageFond()
{
    return imageFond;
}
SDL_Rect Case::getPositionPixel()
{
    return positionPixel;
}

void Case::setType(TypeCase type)
{
    this->type = type;
}


bool Case::equals(Case* caseAComparer)
{
    if(positionDamier.x == caseAComparer->positionDamier.x && positionDamier.y == caseAComparer->positionDamier.y)
        return true;
    return false;
}

void Case::calculNextFrame(unsigned int nbFrame)
{
    timeAfter = getTime();
    if(timeAfter-timeBefore > TEMPS_CASE_FRAME)
    {
        counter++;

        if(counter>=nbFrame)
        {
            explose = false;
            cassee = true;
            type = FOND;
            counter = 0;
            calculApparitionBonus();
        }

        timeBefore = timeAfter;
    }
}

void Case::calculApparitionBonus()
{
    unsigned int nombre2 = genererNombreAleatoire(100);

    if(nombre2<= PROBABILITY_SUR_100)
    {
        unsigned int nombre3 = genererNombreAleatoire(100);
        switch(nombre3)
        {
            case 1 ... PROBABILITY_GRANDSOIN :
                Carte::ajouterBonus(new BonusGrandSoin(this));
                break;
            case PROBABILITY_GRANDSOIN + 1 ... (PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 1) :
                Carte::ajouterBonus(new BonusVitesse(this));
                break;
            case (PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 2) ... (PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 2) :
                Carte::ajouterBonus(new BonusBombeRapide(this));
                break;
            case (PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 3) ... (PROBABILITY_PETITSOIN + PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 3) :
                Carte::ajouterBonus(new BonusPetitSoin(this));
                break;
            case (PROBABILITY_PETITSOIN + PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 4) ... (PROBABILITY_TETE_DE_MORT + PROBABILITY_PETITSOIN + PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 4) :
                Carte::ajouterBonus(new BonusTeteDeMort(this));
                break;
            case (PROBABILITY_TETE_DE_MORT + PROBABILITY_PETITSOIN + PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 5) ... (PROBABILITY_ECLAIR + PROBABILITY_TETE_DE_MORT + PROBABILITY_PETITSOIN + PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 5) :
                Carte::ajouterBonus(new BonusEclair(this));
                break;
            case (PROBABILITY_ECLAIR + PROBABILITY_TETE_DE_MORT + PROBABILITY_PETITSOIN + PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 6) ... (PROBABILITY_PROTECTION + PROBABILITY_ECLAIR + PROBABILITY_TETE_DE_MORT + PROBABILITY_PETITSOIN + PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 6) :
                Carte::ajouterBonus(new BonusProtection(this));
                break;
            case (PROBABILITY_PROTECTION + PROBABILITY_ECLAIR + PROBABILITY_TETE_DE_MORT + PROBABILITY_PETITSOIN + PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 7) ... (PROBABILITY_EXPLOSION + PROBABILITY_PROTECTION + PROBABILITY_ECLAIR + PROBABILITY_TETE_DE_MORT + PROBABILITY_PETITSOIN + PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 7) :
                Carte::ajouterBonus(new BonusDistanceExplosion(this));
                break;
            case (PROBABILITY_EXPLOSION + PROBABILITY_PROTECTION + PROBABILITY_ECLAIR + PROBABILITY_TETE_DE_MORT + PROBABILITY_PETITSOIN + PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 8) ... (PROBABILITY_NOMBRE_BOMBE + PROBABILITY_EXPLOSION + PROBABILITY_PROTECTION + PROBABILITY_ECLAIR + PROBABILITY_TETE_DE_MORT + PROBABILITY_PETITSOIN + PROBABILITY_BOMBE_RAPIDE + PROBABILITY_VITESSE + PROBABILITY_GRANDSOIN + 8) :
                Carte::ajouterBonus(new BonusNombreBombe(this));
                break;
            default:
                break;
        }

    }
}

SDL_Rect* Case::getAdressePositionPixel()
{
    return &positionPixel;
}

Bomb* Case::getBombe()
{
    return bombe;
}

void Case::setExplose(Bomber* bomber)
{
    if(!explose)
    {
        explose = true;
        MenuJeu::ajouterScore(score,positionPixel.x,positionPixel.y,bomber);
        Mix_PlayChannel(-1,sonCase,0);
    }

}
void Case::setBombe(Bomb* bombe)
{
    this->bombe = bombe;
}

SDL_Rect Case::getPositionDamier()
{
    return positionDamier;
}

SDL_Rect* Case::getAdressePositionDamier()
{
    return &positionDamier;
}

Bomber* Case::getBomber()
{
    return bomber;
}
void Case::setBomber(Bomber* bomber)
{
    this->bomber = bomber;
}

Ennemi* Case::getEnnemi()
{
    return ennemi;
}

void Case::setEnnemi(Ennemi* ennemi)
{
    this->ennemi = ennemi;
}

Case::~Case()
{

}
