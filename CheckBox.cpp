#include "Widget.h"
#include "CheckBox.h"
#include "Utile.h"

CheckBox::CheckBox(Menu* menu, bool val) : Widget(menu)
{
    image[0] = chargerSurface("Images/boxvide.png");
    image[1] = chargerSurface("Images/boxremplie.png");
    int w,h;
    SDL_QueryTexture(image[0],nullptr,nullptr,&w,&h);
    position.w = w;
    position.h = h;
    value = val;
    calmTime = SDL_GetTicks();
    groupCheckBox = nullptr;
}

CheckBox::~CheckBox()
{
    SDL_DestroyTexture(image[0]);
    SDL_DestroyTexture(image[1]);
}

void CheckBox::setGroupCheckBox(GroupCheckBox* groupCheckBox)
{
    this->groupCheckBox = groupCheckBox;
}

bool CheckBox::getValue()
{
    return value;
}

void CheckBox::setValue(bool value)
{
    this->value = value;
}

void CheckBox::onClic()
{
    if(SDL_GetTicks() - calmTime > 300)
    {
        if(groupCheckBox)
        {
            if(!value)
            {
                for(unsigned int i = 0;i<groupCheckBox->getListeCheckBox().size();i++)
                {
                    if(groupCheckBox->getListeCheckBox().at(i) != this)
                        groupCheckBox->getListeCheckBox().at(i)->setValue(false);
                }
                value = true;
            }
        }
        else
            value = !value;

        calmTime = SDL_GetTicks();


    }
}

SDL_Texture* CheckBox::getImage()
{
    return value ? image[1] : image[0];
}

