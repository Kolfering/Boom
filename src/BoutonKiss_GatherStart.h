#ifndef BOUTONKISS_GATHERSTART_H
#define BOUTONKISS_GATHERSTART_H

#include "BoutonKiss.h"
#include "Network_Server.h"

class BoutonKiss_GatherStart : public BoutonKiss{
private :
    kiss_entry* kiss_entry_NameGame;
    Network_Server* n_w;
public :
    BoutonKiss_GatherStart(kiss_button*,kiss_entry*);
    ~BoutonKiss_GatherStart();
    void onClick();

};



#endif // BOUTONKISS_GATHERSTART_H
