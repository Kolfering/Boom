#include "BoutonKiss_JoinStart.h"
#include "Network_Client.h"
#include "Jeu.h"
#include "MenuPrepareNetworkGame.h"

BoutonKiss_JoinStart::BoutonKiss_JoinStart(kiss_button* kiss_b, kiss_entry* kiss_e, Network_Client* network_client) : BoutonKiss(kiss_b)
{
    kiss_entry_IPHost = kiss_e;
    this->network_client = network_client;
}

BoutonKiss_JoinStart::~BoutonKiss_JoinStart()
{

}

void BoutonKiss_JoinStart::onClick()
{
    network_client->initializeToServer(kiss_entry_IPHost->text);
}
