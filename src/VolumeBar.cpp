#include "VolumeBar.h"
#include "Utile.h"

VolumeBar::VolumeBar(Menu* menu,float value) : Widget(menu)
{
    image = chargerSurface("../Images/volumeBar.png");
    SDL_QueryTexture(image,nullptr,nullptr,&position.w,&position.h);
    this->value = value;
    this->menu = menu;
    volumeGrab = new VolumeGrab(menu);
}

VolumeBar::~VolumeBar()
{
    SDL_DestroyTexture(image);
}

VolumeGrab* VolumeBar::getVolumeGrab()
{
    return volumeGrab;
}

float VolumeBar::getValue()
{
    return value;
}

void VolumeBar::setValue(float value)
{
    this->value = value;
    majVolumeGrab();
}

void VolumeBar::majVolumeGrab()
{
    int w,h,w2,h2;

    SDL_QueryTexture(image,nullptr,nullptr,&w,&h);
    SDL_QueryTexture(volumeGrab->getImage(),nullptr,nullptr,&w2,&h2);

    volumeGrab->setPosition(position.x + (value/100)*w - w2/2,position.y + h/2 - h2/2);
}

void VolumeBar::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
    majVolumeGrab();

}

void VolumeBar::onClic()
{
    int w,h,w2,h2;
    SDL_QueryTexture(volumeGrab->getImage(),nullptr,nullptr,&w,&h);
    volumeGrab->setPosition(Jeu::getPositionSouris().x - w/2,volumeGrab->getAdressePosition()->y);
    SDL_QueryTexture(image,nullptr,nullptr,&w2,&h2);
    value = ((volumeGrab->getAdressePosition()->x + w/2. - position.x) / w2)*100;

    if(value>97) value = 100;
    if(value<3) value = 0;

}


SDL_Texture* VolumeBar::getImage()
{
    return image;
}

VolumeGrab::VolumeGrab(Menu* menu) : Widget(menu)
{
    image = chargerSurface("../Images/volumeBall.png");
    SDL_QueryTexture(image,nullptr,nullptr,&position.w,&position.h);
}

VolumeGrab::~VolumeGrab()
{
    SDL_DestroyTexture(image);
}

void VolumeGrab::setPosition(int x,int y)
{
    position.x = x;
    position.y = y;
}



SDL_Texture* VolumeGrab::getImage()
{
    return image;
}
