#ifndef BOUTONKISS_H
#define BOUTONKISS_H

#include "kiss_sdl.h"

class BoutonKiss{
protected :
    kiss_button* button;

public :
    BoutonKiss(kiss_button*);
    virtual ~BoutonKiss();
    virtual void onClick() = 0;
    kiss_button* getButton();

};


#endif // BOUTONKISS_H
