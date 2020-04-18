#ifndef VOLUMEBAR_H
#define VOLUMEBAR_H
#include "SDL.h"
#include "Widget.h"

class VolumeGrab : public Widget{
private:
    SDL_Texture* image;

public:
    VolumeGrab(Menu* menu);
    ~VolumeGrab();
    SDL_Texture* getImage();
    void setPosition(int,int);

};

class VolumeBar : public Widget{
private:
    SDL_Texture* image;
    VolumeGrab* volumeGrab;
    float value;
    void majVolumeGrab();

public:
    VolumeBar(Menu*,float = 50);
    ~VolumeBar();
    void onClic();
    void setPosition(int, int);
    float getValue();
    void setValue(float value);
    VolumeGrab* getVolumeGrab();
    SDL_Texture* getImage();


};




#endif
