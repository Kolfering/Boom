#ifndef PERSONNAGE_H
#define PERSONNAGE_H
#include "Case.h"
#include "Enumeration.h"
#include "Input.h"
#define TEMPS_MORT 2500

class Personnage{
protected:
    SDL_Rect position;
    SDL_Rect antiPosition;
    Position positionReal;
    unsigned int counter;
    int vie;
    float vitesse;
    bool mort;
    Case* casePersonnage;
    Case* caseToGo;
    FrameManager* frameManager;
    CounterFrame counterFrame;
    Direction direction;
    unsigned int timeBefore;
    unsigned int timeAfter;
    unsigned int timeMort;
    int taille;
    bool meurt;
    bool enDeplacementVersCase;
    bool amical;
    void calculNextFrame();
    bool aAtteintCase();


public:
    Personnage(Case*);
    Personnage();
    virtual ~Personnage();
    virtual SDL_Rect* getAdresseAntiPosition();
    void prendreDegat(float);
    virtual bool goToCase() = 0;
    virtual Case* getCase();
    virtual Case* getCaseToGo();
    virtual void setCase(Case*) = 0;
    virtual void setDirection(Direction);
    virtual void setVitesse(float);
    float getVitesse();
    bool isAmical();
    bool isMeurt();
    int getTaille();
    Direction getDirection();
    unsigned int getVie();
    virtual SDL_Texture* getImage() = 0;
    bool isEnDeplacementVersCase();
    void setPositionReal(float x, float y);
    SDL_Rect getPosition();
    SDL_Rect* getAdressePosition();


};

#endif // PERSONNAGE_H
