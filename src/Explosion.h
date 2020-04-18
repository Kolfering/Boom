#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "Case.h"
#define EXPLOSION_FRAME 4
#define TEMPS_PAR_SPRITE_EXPLOSION 80
#define TAILLE_SPRITE_EXPLOSION 32
#define COEF_DEGAT 16
#define TIME_EXPLOSION_TRIGGER 25

class Bomb;
class Ennemi;

class Explosion{
private:
    static SDL_Texture* image;
    unsigned int ID;
    static unsigned int counterID;
    Case* casePositionExplosion;
    Bomb* bombe;
    SDL_Rect antiPosition;
    float degat;
    int counter;
    int timeBefore;
    int timeTrigger;
    DirectionExplosion directionExplosion;
    bool explosionFinie;
    void detectionPersonnage();
    void calculNextFrame();

public:
    Explosion(Bomb*,Case*,DirectionExplosion,float);
    ~Explosion();
    void execute();
    SDL_Rect* getAntiPositionAdresse();
    bool isExplosionFinie();
    Bomb* getBombe();
    float getDegat();
    Case* getCase();
    SDL_Texture* getImage();
    static void initialiserExplosion();
    static void releaseExplosion();

};



#endif // EXPLOSION_H
