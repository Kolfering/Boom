#include "GroupCheckBox.h"
#include "CheckBox.h"

GroupCheckBox::GroupCheckBox(Menu* menu) : Widget(menu)
{
    displayable = false;
}

GroupCheckBox::~GroupCheckBox()
{
    for(unsigned int i = 0;i<listeCheckBox.size();i++)
    {
        listeCheckBox.erase(listeCheckBox.begin() + i);
        i--;
    }
}

SDL_Texture* GroupCheckBox::getImage()
{
    return nullptr;
}

void GroupCheckBox::ajouterCheckBox(CheckBox* checkBox)
{
    listeCheckBox.push_back(checkBox);
    checkBox->setGroupCheckBox(this);
}

void GroupCheckBox::retirerCheckBox(CheckBox* checkBox)
{
    for(unsigned int i = 0;i<listeCheckBox.size();i++)
    {
        if(checkBox == listeCheckBox.at(i))
        {
            listeCheckBox.at(i)->setGroupCheckBox(nullptr);
            listeCheckBox.erase(listeCheckBox.begin() + i);
            break;
        }
    }
}

std::vector<CheckBox*> &GroupCheckBox::getListeCheckBox()
{
    return listeCheckBox;
}

bool GroupCheckBox::contientCheckBox(CheckBox* checkBox)
{
    for(unsigned int i = 0;i<listeCheckBox.size();i++)
    {
        if(checkBox == listeCheckBox.at(i))
        {
            return true;
        }
    }
    return false;
}
