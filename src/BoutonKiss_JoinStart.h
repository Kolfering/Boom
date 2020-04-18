#ifndef BOUTONKISS_JOINSTART_H
#define BOUTONKISS_JOINSTART_H

#include "BoutonKiss.h"
#include "Network_Client.h"

class BoutonKiss_JoinStart : public BoutonKiss{
private :
    kiss_entry* kiss_entry_IPHost;
    Network_Client* network_client;
public :
    BoutonKiss_JoinStart(kiss_button*,kiss_entry*,Network_Client*);
    ~BoutonKiss_JoinStart();
    void onClick();

};


#endif // BOUTONKISS_JOINSTART_H
