#ifndef BOMB_H
#define BOMB_H
#include "Bomber.h"
#include "Case.h"
#include "Explosion.h"
#define BOMB_SPRITE 3
#define TAILLE_SPRITE 32
#define TEMPS_FRAME_NORMAL 50
#define TEMPS_FRAME_ATTENTION 50

class Bomb  {
private:
    unsigned int counter;
    Bomber* bomber;
    Case* caseBomb;
    vector<Explosion*> listeExplosions;
    int timePose;
    unsigned int timeBefore;
    bool aExplose;
    static SDL_Texture* image;
    static Mix_Chunk* sonExplosion;
    SDL_Rect antiPosition;
    void calculNextFrame();



public:
    Bomb(Bomber*,Case*);
    ~Bomb();
    static SDL_Texture* getImage();
    void calculExplosion();
    void execute();
    bool isExplose();
    Bomber* getBomber();
    Case* getCase();
    vector<Explosion*> &getListeExplosions();
    SDL_Rect* getAntiPositionAdresse();
    static void initialiserBombe();
    static void releaseBombe();

};


#endif // BOMB_H
