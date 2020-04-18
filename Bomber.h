#ifndef BOMBER_H
#define BOMBER_H
#include <vector>
#include <SDL_mixer.h>
#include "Case.h"
#include "Enumeration.h"
#include "Input.h"
#include "Personnage.h"
#include "InputJoystick.h"
#define NOMBRE_SPRITE_DEPLACEMENT 6
#define NOMBRE_SPRITE_MORT 5
#define NOMBRE_SPRITE_HURT 1
#define TEMPS_PAR_SPRITE 120
#define TEMPS_PAR_SPRITE_MORT 200
#define TEMPS_SPRITE_HURT 100
#define VIE_MAX 16
#define VITESSE_BASE 1.2
#define VITESSE_UP 2.4
#define DISTANCE_BASE 2
#define DISTANCE_MAX 4
#define NOMBRE_BOMBE_BASE 5
#define NOMBRE_BOMBE_MAX 8
#define TEMPS_EXPLOSION 6000
#define TEMPS_EXPLOSION_RAPIDE 3000
#define TEMPS_TMP_PROTECTION 1000
#define TEMPS_RESPAWN_PROTECTION 5000
#define TEMPS_ALERTE_RESPAWN_PROTECTION 2000
#define TEMPS_ALERTE_TMP_PROTECTION 120
#define TEMPS_BOMBER_MORT 4000


class Bomb;
class Bonus;

using std::vector;

class Bomber : public Personnage {
private :
    SDL_JoystickID joystickID;
    int timeHurt;
    unsigned int nombreBombes;
    unsigned int distanceExplosion;
    int tempsExplosion;
    unsigned int tempsTmpProtection;
    unsigned int score;
    unsigned int megaVie;
    unsigned int player;
    unsigned int tempsMort;
    bool enDeplacement;
    bool hurt;
    bool immunise;
    bool tmpImmunise;
    bool apparaitreProtection;
    bool apparaitreTmpProtection;
    bool celebration;
    bool aRespawn;
    bool gameEnded;
    bool bombeRapide;
    bool vitesseUp;
    bool protection;
    SDL_Texture* image;
    FrameManager frameManagerHaut;
    FrameManager frameManagerBas;
    FrameManager frameManagerDroite;
    FrameManager frameManagerGauche;
    FrameManager frameManagerHit;
    FrameManager frameManagerDead;
    FrameManager frameManagerWin;
    static SDL_Texture* imageProtection;
    SDL_Rect positionProtection;
    static Mix_Chunk* sonBombe;
    static Mix_Chunk* sonBonus;
    static Mix_Chunk* sonLettre;
    Mix_Chunk* sonHurt;
    Mix_Chunk* sonMort;
    Mix_Chunk* sonComplete;
    static Mix_Chunk* sonMegaVie;
    vector<Bomb*> listeBombes;
    vector<Bonus*> listeBonus;
    vector<bool> listeExtraLettre;
    vector<int> listeBonusHUD;
    bool goToCase();
    void deplacerDirection(Direction);
    bool debugTeleporter();
    void collisionLettre();
    bool poserBombe();
    void respawn();
    void setEnded();
    Case* caseSpawn;
    PlayerKeyboard playerKeyboard;



public :
    Bomber();
    virtual ~Bomber();
    void deplacer(Input* in);
    void prendreDegat(unsigned int);
    void gagnerVie(unsigned int);
    void setImmunise(bool);
    bool isImmunise();
    bool isApparaitreProtection();
    bool isApparaitreTmpProtection();
    void augmenterScore(unsigned int);
    bool isTmpImmunise();
    bool isGameEnded();
    void setApparaitreProtection(bool);
    void spawn();
    void setCaseSpawn(Case*);
    void setRespawn(bool);
    int getTempsExplosion();
    unsigned int getDistanceExplosion();
    unsigned int getNombreBombes();
    unsigned int getScore();
    void setNombreBombes(unsigned int);
    void setDistanceExplosion(unsigned int);
    void setBombeRapide(bool);
    void setProtection(bool);
    void setVitesseUp(bool);
    void calculEffetBonus();
    void setCase(Case*);
    void preparerNiveau();
    void activerCelebration();
    bool isProtection();
    bool isVitesseUp();
    bool isBombeRapide();
    SDL_Texture* getImage();
    unsigned int getMegaVie();
    SDL_Rect* getAdresseAntiPositionProtection();
    SDL_Texture* getImageProtection();
    vector<Bomb*> &getListeBombes();
    vector<Bonus*> &getListeBonus();
    vector<bool> &getListeExtraLettre();
    void ajouterBonus(Bonus*);
    static void releaseBomber();
    static void initialiserBomber();
    void setPlayerKeyboard(PlayerKeyboard);
    PlayerKeyboard getPlayerKeyboard();
    void setJoystickID(SDL_JoystickID);
    SDL_JoystickID getJoystickID();


    template<class Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar & megaVie & vie & score & listeExtraLettre & nombreBombes & distanceExplosion & bombeRapide & tempsExplosion;
    }





};

























#endif // BOMBER_H
