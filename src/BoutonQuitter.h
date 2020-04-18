#ifndef BOUTONQUITTER_H
#define BOUTONQUITTER_H
#include "Bouton.h"

class BoutonQuitter: public Bouton {
public:
    ~BoutonQuitter();
    BoutonQuitter(Menu*);
    void onClic();
};



#endif // BOUTONQUITTER_H
