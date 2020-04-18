#ifndef CASE_H
#define CASE_H
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include "Enumeration.h"
#define CASE_DIMENSION 32
#define CASE_FRAME 4
#define TEMPS_CASE_FRAME 150
#define SCORE_CASE 10
#define CASE_DEPART_JEU_X CASE_DIMENSION*4
#define CASE_DEPART_JEU_Y CASE_DIMENSION

class Bomb;
class Bomber;
class Ennemi;
class Bonus;
class Teleporter;
class Explosion;
class Coin;

using std::string;

class Case {

private :
    SDL_Rect positionPixel;
    SDL_Rect positionDamier;
    SDL_Rect antiPositionPixel;
    TypeCase type;
    unsigned int counter;
    unsigned int timeBefore;
    unsigned int timeAfter;
    unsigned int score;
    bool cassee;
    bool explose;
    bool reservee;
    Bomb* bombe;
    Bomber* bomber;
    Ennemi* ennemi;
    Bonus* bonus;
    Coin* coin;
    Explosion* explosion;
    Teleporter* teleporter;
    static Mix_Chunk* sonCase;
    void calculNextFrame(unsigned int);
    void calculApparitionBonus();

public :
    ~Case();
    Case(TypeCase,SDL_Rect);
    SDL_Rect getPositionPixel();
    SDL_Rect* getAdressePositionPixel();
    SDL_Rect getPositionDamier();
    SDL_Rect* getAdressePositionDamier();
    SDL_Rect* getAdresseAntiPositionPixel();
    SDL_Texture* getImage();
    void setExplose(Bomber*);
    void setType(TypeCase);
    TypeCase getType();
    static SDL_Texture* getImageFond();
    bool isReservee();
    void setReservee(bool);
    Bomb* getBombe();
    Bomber* getBomber();
    Ennemi* getEnnemi();
    Bonus* getBonus();
    Coin* getCoin();
    Teleporter* getTeleporter();
    Explosion* getExplosion();
    void setTeleporter(Teleporter*);
    void setBonus(Bonus*);
    void setBomber(Bomber*);
    void setCoin(Coin*);
    void setBombe(Bomb*);
    void setEnnemi(Ennemi*);
    void setExplosion(Explosion*);
    void execute();
    bool equals(Case*);
    static void releaseCase();
    static void initialiserCase();
    static SDL_Texture* getImageBordure();


    static unsigned int case_fond;
    static unsigned int case_dur;
    static unsigned int case_cassable;
    static unsigned int case_bordure;
    static SDL_Texture* imageFond;
    static SDL_Texture* imageDur;
    static SDL_Texture* imageCassable;
    static SDL_Texture* imageBordure;
    static int hauteur_case;
    static int longueur_case;

};

#endif // CASE_H
