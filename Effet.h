#ifndef EFFET_H
#define EFFET_H
#include <SDL.h>
#include <SDL_mixer.h>
#define NB_FRAME_EFFET 4
#define TAILLE_FRAME_EFFET 32
#define TEMPS_EFFET_FRAME 100

class Effet{
private:
    static SDL_Texture* image;
    static Mix_Chunk* sonEffet;
    SDL_Rect position;
    SDL_Rect antiPosition;
    int counter;
    bool counterReverse;
    unsigned int tempsBefore;
    bool fini;

public:
    Effet(unsigned int x,unsigned int y);
    ~Effet();
    SDL_Rect* getAdressePosition();
    SDL_Rect* getAdresseAntiPosition();
    SDL_Texture* getImage();
    Mix_Chunk* getSon();
    bool isFini();
    void execute();
    void calculNextFrame();
    void static initialiserEffet();
    void static releaseEffet();

};


#endif // EFFET_H
