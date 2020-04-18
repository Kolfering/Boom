#ifndef BOUTONCONTINUE_H
#define BOUTONCONTINUE_H
#include "Bouton.h"

class BoutonContinue: public Bouton {
public:
    ~BoutonContinue();
    BoutonContinue(Menu*);
    void onClic();
};




#endif // BOUTONCONTINUE_H
