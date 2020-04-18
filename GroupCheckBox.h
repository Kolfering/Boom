#ifndef GROUPCHECKBOX_H
#define GROUPCHECKBOX_H
#include <vector>
#include "Widget.h"
class CheckBox;

using std::vector;

class GroupCheckBox : public Widget{
private:
    vector<CheckBox*> listeCheckBox;
public:
    GroupCheckBox(Menu*);
    ~GroupCheckBox();
    void ajouterCheckBox(CheckBox*);
    void retirerCheckBox(CheckBox*);
    bool contientCheckBox(CheckBox*);
    vector<CheckBox*> &getListeCheckBox();
    SDL_Texture* getImage();

};

#endif // GROUPCHECKBOX_H
