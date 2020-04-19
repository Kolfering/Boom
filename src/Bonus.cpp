#include "Bonus.h"
#include "Utile.h"
#include "Carte.h"

/*******************************************************************/
SDL_Texture* Bonus::image = nullptr;
Bonus::Bonus(Case* caseBonus)
{
    this->bomber = nullptr;
    this->disponible = true;
    this->caseBonus = caseBonus;
    this->caseBonus->setBonus(this);
    tempsApparition = getTime();
    tempsAlerte = tempsApparition;
    apparaitre = true;
    actif = true;
    score = BONUS_SCORE;
}
Bonus::~Bonus()
{
}

SDL_Rect* Bonus::getAdresseAntiPosition()
{
    antiPosition.x = indice* CASE_DIMENSION;
    antiPosition.y = 0;
    antiPosition.w = CASE_DIMENSION;
    antiPosition.h = CASE_DIMENSION;
    return &antiPosition;
}
void Bonus::initialiserBonus()
{
    image = chargerSurface("../Images/Bonuses.png");
    BonusEclair::initialiserEclair();
}

void Bonus::releaseBonus()
{
    if(image)
        SDL_DestroyTexture(image);
    image = nullptr;
}

void Bonus::calculEffet(){}

void Bonus::setActif(bool actif)
{
    this->actif = actif;
}
bool Bonus::isDisponible()
{
    return disponible;
}

bool Bonus::isApparaitre()
{
    return apparaitre;
}

void Bonus::setBomber(Bomber* b)
{
    bomber = b;
}
void Bonus::calculAffichageBonus()
{
    if(getTime() - tempsApparition > TEMPS_BONUS_DISPONIBLE)
        disponible = false;

    else if(getTime() - tempsApparition > TEMPS_BONUS_ALERTE_EXTREME)
    {
        if(getTime() - tempsAlerte > TEMPS_FRAME_ALERTE_EXTREME)
        {
            tempsAlerte = getTime();
            apparaitre = !apparaitre;
        }
    }
    else if(getTime() - tempsApparition > TEMPS_BONUS_ALERTE)
    {
        if(getTime() - tempsAlerte > TEMPS_FRAME_ALERTE)
        {
            tempsAlerte = getTime();
            apparaitre = !apparaitre;
        }
    }
}

SDL_Texture* Bonus::getImage()
{
    return image;
}

Case* Bonus::getCase()
{
    return caseBonus;
}

unsigned int Bonus::getScore()
{
    return score;
}
std::string Bonus::getNom()
{
    return nom;
}

void Bonus::setCase(Case* c)
{
    caseBonus = c;
}
void Bonus::setDisponible(bool dispo)
{
    disponible = dispo;
}

Bomber* Bonus::getBomber()
{
    return bomber;
}
bool Bonus::isActif()
{
    return actif;
}
bool Bonus::equals(Bonus* bonusAComparer)
{
    if(disponible && bonusAComparer->isDisponible())
    {
        if(caseBonus->equals(bonusAComparer->caseBonus))
            return true;
        return false;
    }
    else if(bomber && bonusAComparer->getBomber())
    {
        if(nom == bonusAComparer->getNom())
            return true;
        return false;
    }
    return false;

}

/********************************************************************/
BonusGrandSoin::BonusGrandSoin(Case* caseBonus) : Bonus(caseBonus)
{
    indice = 6;
    nom = "Grand soin";

}

void BonusGrandSoin::declencherEffet()
{
    this->bomber->gagnerVie(VIE_MAX);
    actif = false;
}


/********************************************************************/
BonusVitesse::BonusVitesse(Case* caseBonus) : Bonus(caseBonus)
{
    indice = 8;
    nom = "Vitesse";
    tempsBonusActif = TEMPS_BONUS_TEMPORAIRE_ACTIF;
}

void BonusVitesse::declencherEffet()
{
    this->bomber->setVitesseUp(true);
    tempsDebutActivation = getTime();
}

void BonusVitesse::calculEffet()
{
    if(getTime() - tempsDebutActivation > tempsBonusActif)
    {
        this->bomber->setVitesseUp(false);
        actif = false;
    }

}
/**********************************************************************/
BonusPetitSoin::BonusPetitSoin(Case* caseBonus) : Bonus(caseBonus)
{
    indice = 5;
    nom = "Petit soin";
}

void BonusPetitSoin::declencherEffet()
{
    this->bomber->gagnerVie(2);
    actif = false;
}
/**********************************************************************/
unsigned int BonusEclair::longueur = 1;
unsigned int BonusEclair::hauteur = 1;
BonusEclair::BonusEclair(Case* caseBonus) : Bonus(caseBonus)
{
    indice = 0;
    nom = "Eclair";
}

void BonusEclair::declencherEffet()
{
    tempsEclair = getTime();
}

void BonusEclair::initialiserEclair()
{
    longueur = 1;
    hauteur = 1;
}

void BonusEclair::calculEffet()
{

 if(getTime() - tempsEclair > VITESSE_ECLAIR)
 {
    if(Carte::getCase(hauteur,longueur)->getType() == CASSABLE)
        Carte::getCase(hauteur,longueur)->setExplose(bomber);

    hauteur++;
    tempsEclair = getTime();

 }


    if(hauteur > CASE_HAUTEUR)
    {
        longueur++;
        hauteur = 1;
    }


    if(longueur > CASE_LONGUEUR)
    {
        actif = false;
        longueur = 1;
        hauteur = 1;
    }

}


/**********************************************************************/
BonusBombeRapide::BonusBombeRapide(Case* caseBonus) : Bonus(caseBonus)
{
    indice = 3;
    nom = "Bombe rapide";
}

void BonusBombeRapide::declencherEffet()
{
    bomber->setBombeRapide(true);
    actif = false;
}
/**********************************************************************/
BonusTeteDeMort::BonusTeteDeMort(Case* caseBonus) : Bonus(caseBonus)
{
    indice = 1;
    nom = "Tete de mort";

}

void BonusTeteDeMort::declencherEffet()
{
    for(unsigned int i = 0;i<Carte::getListeEnnemi().size();i++)
        Carte::getListeEnnemi().at(i)->prendreDegat(bomber,0);

    actif = false;
}
/**********************************************************************/
BonusProtection::BonusProtection(Case* caseBonus) : Bonus(caseBonus)
{
    indice = 7;
    nom = "Protection";
    tempsBonusActif = TEMPS_BONUS_TEMPORAIRE_ACTIF;
}

void BonusProtection::declencherEffet()
{
    bomber->setRespawn(false);
    bomber->setProtection(true);
    bomber->setApparaitreProtection(true);
    tempsDebutActivation = getTime();
    tempsAlerte = TEMPS_BONUS_TEMPORAIRE_ACTIF*3/4.;
    tempsAlerteExtreme = TEMPS_BONUS_TEMPORAIRE_ACTIF*9/10.;
    tempsClignotement = 0;
}

void BonusProtection::calculEffet()
{
    if(getTime() - tempsDebutActivation > tempsBonusActif)
    {
        actif = false;
        bomber->setProtection(false);

    }
    else if(getTime() - tempsDebutActivation > tempsAlerteExtreme)
    {
        if(getTime() - tempsClignotement > TEMPS_FRAME_ALERTE_EXTREME)
        {
            tempsClignotement = getTime();
            bomber->setApparaitreProtection(!bomber->isApparaitreProtection());
        }
    }
    else if(getTime() - tempsDebutActivation > tempsAlerte)
    {
        if(getTime() - tempsClignotement > TEMPS_FRAME_ALERTE)
        {
            tempsClignotement = getTime();
            bomber->setApparaitreProtection(!bomber->isApparaitreProtection());
        }
    }

}
/**********************************************************************/
BonusDistanceExplosion::BonusDistanceExplosion(Case* caseBonus) : Bonus(caseBonus)
{
    indice = 4;
    nom = "Distance explosion";
}

void BonusDistanceExplosion::declencherEffet()
{
    bomber->setDistanceExplosion(bomber->getDistanceExplosion()+1);
    actif = false;
}
/**********************************************************************/
BonusNombreBombe::BonusNombreBombe(Case* caseBonus) : Bonus(caseBonus)
{
    indice = 2;
    nom = "Nombre bombe";
}

void BonusNombreBombe::declencherEffet()
{
    bomber->setNombreBombes(bomber->getNombreBombes()+1);
    actif = false;
}
/**********************************************************************/

