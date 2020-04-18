#ifndef CARTE_H
#define CARTE_H
#include "Case.h"
#include "Teleporter.h"
#include "Effet.h"
#include "Bonus.h"
#include "Score.h"
#include "Ennemi.h"
#include "EnnemiAlienBoss.h"
#include "Bomber.h"
#include "Projectile.h"
#include "Coin.h"
#include "Lettre.h"
#include <vector>
#include <SDL_mixer.h>
#define CASE_LONGUEUR 15
#define CASE_HAUTEUR 13


class Carte {
private :
    static vector< vector<Case*> >tableau;
    static vector<Bonus*> listeBonus;
    static vector<Teleporter*> listeTeleporter;
    static vector<Effet*> listeEffet;
    static vector<Ennemi*> listeEnnemi;
    static vector<Projectile*> listeProjectile;
    static vector<Score*> listeScore;
    static vector<Coin*> listeCoin;
    static vector<Lettre*> listeLettre;
    Case* listeSpawn[4];
    bool listeEnnemiLoad[12];
    static bool noCoin;
    static bool noTeleporter;
    static Mix_Music* musique;
    static unsigned int temps;
    static unsigned int tempsEcoule;
    static bool boss;
    static bool hasNextLevel;

public :
    Carte(unsigned int);
    bool chargerCarte(int);
    static Case* getCase(unsigned int,unsigned int);
    static Case* getCaseByPixel(int,int);
    vector<Bonus*> &getListeBonus();
    vector<Teleporter*> &getListeTeleporter();
    vector<Effet*> &getListeEffet();
    vector<Score*> &getListeScore();
    vector<Projectile*> &getListeProjectile();
    vector<Coin*> &getListeCoin();
    static vector<Lettre*> &getListeLettre();
    Case** getListeSpawn();
    static Ennemi* getBoss();
    static vector<Ennemi*> &getListeEnnemi();
    static Teleporter* getTeleporterFrere(unsigned int);
    static void ajouterEnnemi(Ennemi*);
    static void ajouterEffet(Effet*);
    static void ajouterBonus(Bonus*);
    static void ajouterProjectile(Projectile*);
    static void retirerBonus(Bonus*);
    static void retirerEnnemi(unsigned int);
    static void retirerLettre(unsigned int);
    static bool isNoCoin();
    Mix_Music* getMusique();
    SDL_Texture* getCaseBordure();
    bool hasBos();
    void ajouterScore(Score*);
    void retirerScore(unsigned int);
    void retirerCoin(unsigned int);
    static void resetNextLevel();
    static unsigned int getTemps();
    static unsigned int getTempsEcoule();
    static bool getHasNextLevel();
    static void setTempsEcoule(unsigned int);
    static vector< vector<Case*> > &getTableau();
    ~Carte();

};


#endif // CARTE_H
