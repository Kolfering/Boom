#ifndef COIN_H
#define COIN_H
#include <SDL.h>
#include <SDL_mixer.h>
#include "Case.h"
#include "Enumeration.h"
#include "Bomber.h"
#define TEMPS_COIN 2000

class Coin{
private:
    static SDL_Texture* image;
    static Mix_Chunk* son;
    Case* caseCoin;
    unsigned int tempsPickUp;
    bool pickUp;
    bool toRemove;
    unsigned int score;
    SDL_Rect position;
    SDL_Rect antiPosition;
    CounterFrame counterFrame;
    FrameManager* frameManager;
    FrameManager frameManagerCoin;
    FrameManager frameManagerCoinPickUp;
    void calculNextFrame();

public:
    Coin(Case*);
    ~Coin();
    Case* getCase();
    void setCase(Case*);
    static void initialiserCoin();
    static void releaseCoin();
    void pickedUp(Bomber*);
    void execute();
    bool isToRemove();
    SDL_Rect* getAdressePosition();
    SDL_Rect* getAdresseAntiPosition();
    bool isPickUp();
    SDL_Texture* getImage();

};

#endif // COIN_H
