#ifndef BOUTONRETOUR_H
#define BOUTONRETOUR_H
#include "Bouton.h"
#include "Menu.h"

class BoutonRetour: public Bouton {
private:
    IDMenu IDMenuRetour;
public:
    ~BoutonRetour();
    BoutonRetour(Menu*,IDMenu);
    void onClic();
};


#endif // BOUTONRETOUR_H
