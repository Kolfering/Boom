#ifndef BOUTONOPTION_H
#define BOUTONOPTION_H
#include "Bouton.h"

class BoutonOption : public Bouton {
public:
    ~BoutonOption();
    BoutonOption(Menu*);
    void onClic();
};


#endif // BOUTONOPTION_H
