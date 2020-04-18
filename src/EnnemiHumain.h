#ifndef ENNEMIHUMAIN_H
#define ENNEMIHUMAIN_H
#include "Ennemi.h"

class EnnemiHumain : public Ennemi{
public:
    EnnemiHumain(Case*);
    virtual ~EnnemiHumain();
    void deplacerEnnemi();
    bool attaque();
    virtual Mix_Chunk* getSonAtk() = 0;

};


#endif // ENNEMIHUMAIN_H
