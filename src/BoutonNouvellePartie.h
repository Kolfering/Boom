#ifndef BOUTONNOUVELLEPARTIE_H
#define BOUTONNOUVELLEPARTIE_H
#include "Bouton.h"
#include "Menu.h"

class BoutonNouvellePartie : public Bouton {
public:
    ~BoutonNouvellePartie();
    BoutonNouvellePartie(Menu*);
    void onClic();
};

#endif // BOUTONNOUVELLEPARTIE_H
