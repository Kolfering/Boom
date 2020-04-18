#ifndef ENTRYKISS_H
#define ENTRYKISS_H

#include "kiss_sdl.h"
#include "Input.h"

class EntryKiss{
protected :
    kiss_entry* entry;

public :
    EntryKiss(kiss_entry*);
    virtual ~EntryKiss();
    virtual void event(Input*, InputMouse*);
    kiss_entry* getEntry();

};

#endif // ENTRYKISS_H
