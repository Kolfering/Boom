#ifndef BOUTONJOINONLINE_H
#define BOUTONJOINONLINE_H

#include "Bouton.h"

class BoutonJoinOnline : public Bouton {
public:
    ~BoutonJoinOnline();
    BoutonJoinOnline(Menu*);
    void onClic();
};

#endif // BOUTONJOINONLINE_H
