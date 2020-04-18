#ifndef BOUTONMULTIPLAYER_H
#define BOUTONMULTIPLAYER_H
#include "Bouton.h"

class BoutonMultiplayer : public Bouton {
public:
    ~BoutonMultiplayer();
    BoutonMultiplayer(Menu*);
    void onClic();
};


#endif // BOUTONMULTIPLAYER_H
