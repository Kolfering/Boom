#ifndef BONUS_H
#define BONUS_H
#include "Case.h"
#include "Bomber.h"
#define PROBABILITY_SUR_100 8
#define PROBABILITY_GRANDSOIN 10
#define PROBABILITY_VITESSE 10
#define PROBABILITY_PETITSOIN 15
#define PROBABILITY_PROTECTION 10
#define PROBABILITY_EXPLOSION 15
#define PROBABILITY_BOMBE_RAPIDE 15
#define PROBABILITY_ECLAIR 6
#define PROBABILITY_TETE_DE_MORT 4
#define PROBABILITY_NOMBRE_BOMBE 15
#define TEMPS_BONUS_DISPONIBLE 15000
#define TEMPS_BONUS_ALERTE 7500
#define TEMPS_BONUS_ALERTE_EXTREME 12000
#define TEMPS_FRAME_ALERTE 200
#define TEMPS_FRAME_ALERTE_EXTREME 50
#define TEMPS_BONUS_TEMPORAIRE_ACTIF 25000
#define VITESSE_ECLAIR 25
#define BONUS_SCORE 50


class Bonus{
protected:
    unsigned int tempsApparition; // Moment auquel le bonus spawn sur la carte
    unsigned int tempsDebutActivation; // Moment auquel le bonus est pris par un bomber
    unsigned int tempsBonusActif; // Temps d'efficacité du bonus dans le cas de bonus temporaires
    unsigned int tempsAlerte; //Temps utilisé pour gérer les effets des alertes
    unsigned int indice;
    bool disponible; // Disponible sur la map
    bool apparaitre; // Utilisé pour les alertes
    bool actif; // Actif sur le joueur
    unsigned int score;
    SDL_Rect antiPosition;
    std::string nom;
    static SDL_Texture* image;
    Bomber* bomber;
    Case* caseBonus;

public:
     Bonus(Case*);
     virtual ~Bonus();
     virtual void declencherEffet()=0;
     virtual void calculEffet();
     void setBomber(Bomber*);
     static void initialiserBonus();
     static void releaseBonus();
     SDL_Texture* getImage();
     bool isDisponible();
     bool isApparaitre();
     bool isActif();
     unsigned int getScore();
     std::string getNom();
     void setDisponible(bool);
     void setActif(bool);
     Bomber* getBomber();
     SDL_Rect* getAdresseAntiPosition();
     Case* getCase();
     void setCase(Case*);
     bool equals(Bonus*);
     void calculAffichageBonus();

};

class BonusPetitSoin : public Bonus{
public:
    BonusPetitSoin(Case*);
    void declencherEffet();

};

class BonusGrandSoin : public Bonus{
public:
    BonusGrandSoin(Case*);
    void declencherEffet();

};

class BonusVitesse : public Bonus{
public:
    BonusVitesse(Case*);
    void declencherEffet();
    void calculEffet();
};

class BonusProtection : public Bonus{
private:
    unsigned int tempsAlerte;
    unsigned int tempsAlerteExtreme;
    unsigned int tempsClignotement;
public:
    BonusProtection(Case*);
    void declencherEffet();
    void calculEffet();

};

class BonusEclair : public Bonus{
private:
    static unsigned int longueur;
    static unsigned int hauteur;
    unsigned int tempsEclair;

public:
    BonusEclair(Case*);
    void declencherEffet();
    void calculEffet();
    static void initialiserEclair();
};

class BonusTeteDeMort : public Bonus{
public:
    BonusTeteDeMort(Case*);
    void declencherEffet();
};

class BonusDistanceExplosion : public Bonus{
public:
    BonusDistanceExplosion(Case*);
    void declencherEffet();
};

class BonusBombeRapide : public Bonus{
public:
    BonusBombeRapide(Case*);
    void declencherEffet();

};

class BonusNombreBombe : public Bonus{
public:
    BonusNombreBombe(Case*);
    void declencherEffet();
};
#endif // BONUS_H
