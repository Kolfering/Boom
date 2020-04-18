#ifndef MENUJEU_H
#define MENUJEU_H
#include "Menu.h"
#include "Bomber.h"
#include "Carte.h"
#include "Bomb.h"
#define COLOR_POLICE_HUD {255,255,255}
#define COLOR_HURRY_TIMER {238,12,6}
#define COLOR_BG_TIMER {84,3,173}
#define COLOR_SCORE {252,222,14}
#define COLOR_BG_JOUEUR {106,110,159}
#define TEMPS_FIN_NIVEAU 3000
#define TEMPS_HURRY_STANDBY 1500
#define TEMPS_CALCUL_SCORE 3000
#define TEMPS_PREPARATION_NIVEAU 5000
#define TEMPS_ENTRE_POINT_SCORE 50
#define TEMPS_ENTRE_BIGPOINT_SCORE 200
#define TEMPS_ALIEN_MODE 30000
#define TEMPS_BEFORE_TRANSITION 3000
#define FPS 30.
#define FPS_RATE (1/FPS)*1000

class MenuJeu : public Menu{
private:
    SDL_Texture*  fond;
    SDL_Texture*  fondJoueur;
    SDL_Texture*  bordureHautBas;
    SDL_Texture*  bordureDroiteGauche;
    SDL_Texture*  bordureHautBasTimer;
    SDL_Texture*  bordureDroiteGaucheTimer;
    SDL_Texture*  numberLevel;
    SDL_Texture*  numberLevelBackGround;
    SDL_Texture*  timerBg;
    SDL_Texture*  tempsBonusLabel;
    SDL_Texture*  coeur;
    SDL_Texture*  scoreLabel;
    SDL_Texture*  gameOverLabel1;
    SDL_Texture*  getReadyLabel;
    SDL_Texture*  gameOverLabel2;
    SDL_Texture*  hurryUp;
    SDL_Texture*  extraGame;
    SDL_Texture*  gameOver;
    SDL_Texture*  bossEnergyBar;
    SDL_Texture*  bossEnergyBarBlack;
    SDL_Texture*  hudLetter;
    SDL_Texture*  bonusHUD;
    SDL_Texture*  player1Head;
    SDL_Texture*  player2Head;
    TTF_Font* policeBonus;
    SDL_Rect posHurry;
    SDL_Rect posExtra;
    SDL_Rect posGameOver;
    SDL_Rect posEnergyBar;
    SDL_Rect posEnergyBarBlack;
    SDL_Rect antiPosEnergyBarBlack;
    Mix_Chunk* sonHurry;
    Mix_Chunk* sonExtra;
    Mix_Chunk* sonGameOver;
    Mix_Chunk* sonComplete;
    Mix_Chunk* sonTimeBonus;
    static vector<Bomber*> listeBomber;
    int vitesseHury;
    int vitesseExtra;
    int vitesseGameOver;
    unsigned int nombreJoueur;
    unsigned int tempsHuryStandByAfter;
    unsigned int tempsExtraStandByAfter;
    unsigned int tempsGameOverStandByAfter;
    unsigned int tempsAlienModeActif;
    unsigned int tempsFinNiveau;
    unsigned int niveau;
    unsigned int tempsCalculScore;
    unsigned int tempsPreparationNiveau;
    unsigned int tempsPreparationNiveauAfter;
    unsigned int tempsEntrePointScore;
    unsigned int scoreAAfficher;
    unsigned int tempsGameOverTransition;
    static int tempsPause;
    static int tempsReprise;
    static int tempsSommePause;
    bool transitionOn;
    bool goBackToMenu;
    bool preparationNiveau;
    bool finNiveau;
    bool afficheScore;
    bool hurryMessage;
    bool extraMessage;
    bool gameOverMessage;
    bool pause;
    bool extraStop;
    bool hurryStop;
    bool gameOverStop;
    bool enCalculScore;
    bool sonHurryDone;
    bool sonExtraDone;
    bool sonGameOverDone;
    static Carte* carte;
    bool chargementFichierNiveau();
    void deplacerBomber();
    void afficherCaseCassable();
    void afficherCaseFixe();
    void afficherBomber();
    void afficherBombe();
    void afficherExplosion();
    void afficherBonus();
    void afficherTeleporter();
    void afficherEffet();
    void afficherEnnemi();
    void afficherScore();
    void afficherHud();
    void calculTemps();
    void afficherCaseBordure();
    void calculEnergyBar();
    void calculHury();
    void calculExtra();
    void calculGameOver();
    void afficherMessage();
    void afficherCoin();
    void afficherLettre();
    void afficherEnergyBar();
    bool preparerNiveau();
    void calculFinNiveau();
    void finirNiveau();
    void afficherProjectile();
    void gererCalculScore();
    bool chargerNiveau();
    void executeEnnemi();
    void executeProjectile();
    void executeBombe();
    void executeCoin();
    void executeTeleporter();
    void executeLetter();
    void executeEffet();
    void executeCase();
    void executeScore();
    void executeBonus();
    void afficherNumberLevel();
    void sauvegarderAutomatiquement();
    bool chargerPreferences();
    void handleEvents();
    unsigned int tempsAffichageTemps;
    std::string cheatMSG;
    bool cheatActivated;
    unsigned int niveauCheat;


public:
    MenuJeu(Input*,InputMouse*,SDL_Window*);
    ~MenuJeu();
    void executeMenu();
    void afficheMenu();
    void setPause();
    void chargerContinue();
    static int getTempsPause();
    static void ajouterScore(unsigned int,int,int,Bomber*);
    static vector<Bomber*> &getListeBomber();


};


#endif // MENUJEU_H
