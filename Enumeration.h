#ifndef ENUMERATION_H
#define ENUMERATION_H
#include <SDL.h>
#ifndef M_PI
#define M_PI 3.141592653589793
#endif

#if SDL_BYTEORDER == SDL_BIG_ENDIAN

    #define MASQUE_ROUGE 0xff000000
    #define MASQUE_VERT  0x00ff0000
    #define MASQUE_BLEU  0x0000ff00
    #define MASQUE_ALPHA 0x000000ff

#else

    #define MASQUE_ROUGE 0x000000ff
    #define MASQUE_VERT  0x0000ff00
    #define MASQUE_BLEU  0x00ff0000
    #define MASQUE_ALPHA 0xff000000

#endif


enum TypeCase {FOND=0,CASSABLE=1,DUR=2,COIN=3};
enum Direction {UNDEFINED=0,BAS=1,HAUT=2,DROITE=3,GAUCHE=4};
enum DirectionExplosion {EXPLOSION_MILLIEU=0,EXPLOSION_HAUT=1,EXPLOSION_BAS=1,EXPLOSION_GAUCHE=2,EXPLOSION_DROITE=2};
enum DirectionBordure {BORDURE_GAUCHE = 0,BORDURE_DROITE = 1,BORDURE_HAUT = 2,BORDURE_BAS = 3,BORDURE_COIN_BAS_GAUCHE = 4,BORDURE_COIN_BAS_DROITE = 5,BORDURE_COIN_HAUT_DROITE = 6,BORDURE_COIN_HAUT_GAUCHE = 7};
enum IDEnnemi {SOLDIER=0,SARGE=1,LIZZY=2,TAUR=3,GUNNER=4,THING=5,GHOST=6,SMOULDER=7,SKULLY=8,GIGGLER=9,HEADBOSS=10,ALIENBOSS=11};
enum IDMenu {MENU_PRINCIPAL,MENU_JEU,MENU_OPTION,MENU_FIN,MENU_MULTIPLAYER,MENU_GATHERNETWORK,MENU_JOINNETWORK,MENU_PREPARENETWORKGAME};

typedef struct
{
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode right;
    SDL_Scancode left;
    SDL_Scancode bomb;
    bool pad;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar & up & down & right & left & bomb & pad;
    }

}PlayerKeyboard;


typedef struct
{
    unsigned int x;
    unsigned int y;
    unsigned int nbLigne;

}CounterFrame;

typedef struct
{
    unsigned int x;
    unsigned int y;
    unsigned int nbFrame;
    unsigned int longueur;
    unsigned int tempsFrame;
    unsigned int tempsBefore;
    int increment;

}FrameManager;

#endif // ENUMERATION_H


