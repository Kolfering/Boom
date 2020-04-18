#ifndef MENUGATHERNETWORK_H
#define MENUGATHERNETWORK_H

#include "MenuTemplate.h"

class MenuGatherNetwork : public MenuTemplate {
private:
    kiss_array kiss_objects;
    kiss_button button;
    kiss_entry entryNameGame;
public:
    MenuGatherNetwork(Input*,InputMouse*,SDL_Window*,Menu*);
    ~MenuGatherNetwork();
    void handleEvents();

};


#endif // MENUGATHERNETWORK_H
