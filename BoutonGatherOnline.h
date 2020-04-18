#ifndef BOUTONGATHERONLINE_H
#define BOUTONGATHERONLINE_H
#include "Bouton.h"

class BoutonGatherOnline : public Bouton {
public:
    ~BoutonGatherOnline();
    BoutonGatherOnline(Menu*);
    void onClic();
};

#endif // BOUTONGATHERONLINE_H
